#include "stdafx.h"
#include "Trade.h"
#include <assert.h>

Trade::Trade(Aux_function& arg_aux) : aux(arg_aux) {
	this->update_flag = false;
}

void Trade::scan_packet(std::vector<unsigned char>& decode_data) {
	if (!update_flag)
		return;

	int index;
	int size = decode_data.size();
	for (index = 0; index < size - 1; ++index) {
		if (decode_data[index] == 0x8F && (decode_data[index + 1] == 0xA3 || decode_data[index + 1] == 0xAB))
			break;
	}

	if (index == size - 1) 
		return;

	if (decode_data[index + 1] == 0xA3) {
		unsigned int counter = *reinterpret_cast<unsigned int*>(&decode_data[index + 4]);
		std::vector<unsigned char> trade_packet = { 0x8F, 0xA3, 0x08, 0, 0, 0 };

		//assert(index + 15 < decode_data.size()-1);

		if (index + 15 > decode_data.size() - 1)
			return;

		unsigned int player_id = aux.reverse_bytes_ex(*(unsigned int*)&decode_data[index + 15]);

		if (!this->permission.count(player_id) && !this->permission.empty())
			return;

		trade_packet.resize(11);
		memmove(&trade_packet[4], &counter, sizeof(counter));

		aux.crypt_send(trade_packet);
	} else if (decode_data[index + 1] == 0xAB && (size - index) >= 23) {
		unsigned int counter = *reinterpret_cast<unsigned int*>(&decode_data[index + 7]);

		//assert(index + 15 < decode_data.size()-1);

		if (index + 15 > decode_data.size() - 1)
			return;

		if (*reinterpret_cast<unsigned short*>(&decode_data[index + 6]) != 0x4C) 
			return;

		std::vector<unsigned char> trade_packet = { 0x8F, 0xAA, 0x0C, 0, 0, 0 };

		auto bigendinan_player_id = aux.reverse_bytes(this->player_basic_info.first);

		trade_packet.resize(0xF);
		memmove(&trade_packet[3], &counter, sizeof(counter));
		std::copy(bigendinan_player_id.begin(), bigendinan_player_id.end(), trade_packet.begin() + 7);

		auto confirm = trade_packet;

		aux.crypt_send(trade_packet);

		confirm[1] = 0xAC;

		aux.crypt_send(confirm);
	}
}

void Trade::set_player_info(std::pair<unsigned int, std::string> player_basic_info) {
	this->player_basic_info = player_basic_info;
}

void Trade::set_update_flag(bool f) {
	this->update_flag = f;
}

bool Trade::get_update_flag() const {
	return this->update_flag;
}

void Trade::add_permission(unsigned int player_id) {
	this->permission.insert(player_id);
}

std::set<unsigned int>& Trade::get_permission_list() {
	return this->permission;
}
