#include "stdafx.h"
#include "Party.h"

Party::Party(Aux_function& arg_aux) : aux(arg_aux) {
	this->packet_index = 0;
	this->update_flag  = false;

	add_permission(980915);
	add_permission(982656);
	add_permission(1280);
	add_permission(1264);
	add_permission(1248);
	add_permission(1266);
	add_permission(1296);
	add_permission(980928);
}

void Party::analyze_request(std::vector<unsigned char> decode_data, unsigned int type) {
	unsigned int player_id  = *reinterpret_cast<unsigned int*>(&decode_data[packet_index + 3 + 2]);

	if (type == 0x390C0D22 && (permission.count(player_id) || permission.empty())) {
		unsigned int unk = *reinterpret_cast<unsigned int*>(&decode_data[packet_index + 7 + 2]);
		accept_request(player_id, unk);
	}
	
	if (type == 0x94060722 && (permission.count(player_id) || permission.empty()))
		accept_invite(player_id);
		
	this->packet_index = 0;
}

void Party::add_permission(unsigned int player_id) {
	permission.insert(player_id);
}

// 22 03 02 1E 00   quit group

void Party::leave_party() const {
	std::vector<unsigned char> arr = { 0x22, 0x3, 0x2, 0x1E, 0x0 };
	aux.crypt_send(arr);
}

void Party::set_update_flag(bool f) {
	this->update_flag = f;
}

bool Party::get_update_flag() const {
	return this->update_flag;
}

std::set<unsigned int>& Party::get_permission_list() {
	return this->permission;
}

//22 07 06 48 00 F2 04 00 00

void Party::clear_permission_list() {
	permission.clear();
}

void Party::transfer_leadership(unsigned int player_id) const {
	if (!permission.count(player_id) && !permission.empty())
		return;

	std::vector<unsigned char> arr(9);
	arr[0] = 0x22;
	arr[1] = 0x07;
	arr[2] = 0x06;
	arr[3] = 0x48;
	memmove(&arr[5], &player_id, sizeof(player_id));
	aux.crypt_send(arr);
}

// 22 08 07 42 00 F2 04 00 00 01

void Party::accept_invite(unsigned int player_id) {
	std::vector<unsigned char> invite(10);
	invite[0] = 0x22;
	invite[1] = 0x08;
	invite[2] = 0x07;
	invite[3] = 0x42;
	invite[9] = 0x01;
	memmove(&invite[5], &player_id, sizeof(player_id));
	aux.crypt_send(invite);
}

void Party::scan_packet(std::vector<unsigned char>& decode_data) {
	if (!update_flag)
		return;

	for (int i = 0; i < (int)decode_data.size() - 3; i++) {
		if (*(unsigned int*)&decode_data[i] == 0x390C0D22) {
			this->packet_index = i;
			analyze_request(decode_data, 0x390C0D22);
			continue;
		}

		if (*(unsigned int*)&decode_data[i] == 0x94060722) {
			this->packet_index = i;
			analyze_request(decode_data, 0x94060722);
			break;
		}
	}
}

void Party::accept_request(unsigned int player_id, unsigned int unknown) {
	// 0x22, 0x0B, 0x0A, 0x1C, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x8D, 0xFB, 0x23, 0x5E
	std::vector<unsigned char> accept = { 0x22, 0x0B, 0x0A, 0x1C };
	accept.resize(13);

	memmove(&accept[5], &player_id, sizeof(player_id));
	memmove(&accept[9], &unknown,   sizeof(unknown));
	aux.crypt_send(accept);
}