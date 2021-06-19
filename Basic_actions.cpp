#include "stdafx.h"
#include "Basic_actions.h"

Basic_actions::Basic_actions(Aux_function& arg_aux) : aux(arg_aux) {

}

void Basic_actions::select_npc(unsigned int unique_id) {
	std::vector<unsigned char> arr = { 0x22, 0x07, 0x06, 0x2, 0x00, 0xCC, 0xCC, 0xCC, 0xCC };
	memmove(arr.data() + 5, &unique_id, sizeof(unique_id));
	aux.crypt_send(arr);
}

void Basic_actions::sell_items(unsigned int item_id, unsigned int count, unsigned char pos, unsigned int price) {
	//25 00 02 00 00 00 14 00 00 00 01 00 00 00 C8 2B 00 00 11 00 00 00 0A 00 00 00 00 00 00 00
	//25 00 02 00 00 00 14 00 00 00 01 00 00 00 A6 4F 00 00 00 00 00 00 01 00 00 00 36 88 00 00
	std::vector<unsigned char> arr = { 0x22, 0x1F, 0x1E, 0x25, 0x0, 0x2, 0, 0, 0, 0x14, 0, 0, 0, 1 };
	arr.resize(33);
	memmove(&arr[17], &item_id,  sizeof(item_id));
	memmove(&arr[21], &pos,      sizeof(pos));
	memmove(&arr[25], &count,    sizeof(count));
	memmove(&arr[29], &price,    sizeof(price));
	aux.crypt_send(arr);
}

void Basic_actions::send_basic_skills(unsigned int id, unsigned int target_id) {
	// 29 00 35 02 00 00 00 01 C2 F7 0E 00 
	std::vector<unsigned char> arr = { 0x22, 0x0D, 0x0C, 0x29, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0 };
	memmove(&arr[5], &id,  sizeof(id));
	memmove(&arr[11], &target_id, sizeof(target_id));
	aux.crypt_send(arr);
}

void Basic_actions::send_basic_skills_ex(unsigned int id, unsigned int target_id) {
	// 29 00 F8 04 00 00 01 01 C5 F7 0E 00 26 00 26 00 20 08 5B 
	// 29 00 35 02 00 00 00 01 C2 F7 0E 00 
	std::vector<unsigned char> arr = { 0x22, 0x014, 0x013, 0x29, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	memmove(&arr[5], &id, sizeof(id));
	memmove(&arr[11], &target_id, sizeof(target_id));
	aux.crypt_send(arr);
}

// pede as informações do item em position
void Basic_actions::update_item(unsigned char position) {
	std::vector<unsigned char> arr = { 0x22, 0x05, 0x04, 0x09, 00, 00, 0x00 };
	arr[6] = position;
	aux.crypt_send(arr);
}

void Basic_actions::normal_attack() {
	// 22 04 03 03 00 00
	std::vector<unsigned char> arr = { 0x22, 0x04, 0x03, 0x03, 00, 01 };
	aux.crypt_send(arr);
}

void Basic_actions::use_item(unsigned int id, unsigned char position) {
	//22 B A 28 00 00 01 1A 00 1A 8A 00 00
	std::vector<unsigned char> arr = { 0x22, 0xB, 0xA, 0x28, 0x0, 0x0, 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	arr[7] = position;
	memmove(&arr[9], &id, sizeof(id));
	aux.crypt_send(arr);
}

void Basic_actions::rotate_item(unsigned int id, unsigned char position) {
	// 22 13 12 25 00 47 00 00 00 08 00 00 00 5D 13 00 00 03 00 00 00
	std::vector<unsigned char> arr = { 0x22, 0x13, 0x12, 0x25, 00, 0x47, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00 };
	arr.resize(21, 0);
	memmove(&arr[13], &id, sizeof(id));
	memmove(&arr[17], &position, sizeof(position));
	aux.crypt_send(arr);
}

void Basic_actions::drop_items(unsigned char position, unsigned int count) {
	// 22 08 07 0E 00 03 01 00 00 00
	std::vector<unsigned char> arr = { 0x22, 0x8, 0x7, 0xE, 0x0, 0xFF, 0x0, 0x0, 0x0, 0x0 };
	arr[5] = position;
	memmove(arr.data() + 6, &count, sizeof(count));
	aux.crypt_send(arr);
}

void Basic_actions::send_request_chest_item(unsigned short chest_quest_id) {
	// 22 0A 09 31 00 03 00 00 00 01 B9 63
	std::vector<unsigned char> arr = { 0x22, 0x0A, 0x09, 0x31, 0x00, 0x03, 0x00, 0x00, 
		                               0x00, 0xFF, 0xFF, 0xFF };
	arr[9] = 1;
	memmove(arr.data() + 10, &chest_quest_id, sizeof(chest_quest_id));
	aux.crypt_send(arr);
}

void Basic_actions::accept_recall(unsigned int player_id) {
	// 0x22, 0x09, 0x08, 82 00 00 01 10 24 05 00
	std::vector<unsigned char> arr = { 0x22, 0x09, 0x08, 0x82, 00, 00, 01, 0x10, 0x24, 0x05, 00 };
	memmove(arr.data() + 7, &player_id, sizeof(player_id));
	aux.crypt_send(arr);
}

void Basic_actions::open_npc(unsigned int unique_id) {
	std::vector<unsigned char> arr = { 0x22, 0x07, 0x06, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC };
	memmove(arr.data() + 5, &unique_id, sizeof(unique_id));
	aux.crypt_send(arr);
}

void Basic_actions::equip_item(unsigned char type, unsigned char pos) {
	// 0xC = voo, 0 = arma..

	std::vector<unsigned char> arr = { 0x22, 0x05, 0x04, 0x11, 0x0, 0x0, 0x0 };
	arr[5] = pos;
	arr[6] = type;
	aux.crypt_send(arr);
}

void Basic_actions::mount(unsigned char pos) {
	std::vector<unsigned char> arr = { 0x22, 0x7, 0x6, 0x64, 0 };
	arr.resize(9);

	memmove(arr.data() + 5, &pos, sizeof(pos));
	aux.crypt_send(arr);
}

void Basic_actions::desmount() {
	std::vector<unsigned char> arr = { 0x22, 0x03, 0x02, 0x65, 0x00 };
	aux.crypt_send(arr);
}

void Basic_actions::set_enviroment(std::shared_ptr<Movement> movement_arg) {
	this->movement = movement_arg;
}

void Basic_actions::drop_coin(unsigned int coin) {
	std::vector<unsigned char> arr = { 0x22, 0x07, 0x06, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00 };
	memmove(arr.data() + 5, &coin, sizeof(4));
	aux.crypt_send(arr);
}

void Basic_actions::swap_items(unsigned char item_pos1, unsigned char item_pos2) {
	std::vector<unsigned char> arr = { 0x22, 0x05, 0x04, 0xC, 0x00, 0x00, 0x00};
	arr[5] = item_pos1;
	arr[6] = item_pos2;
	aux.crypt_send(arr);
}
