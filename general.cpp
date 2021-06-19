#include "stdafx.h"
#include "general.h"

General::General(Aux_function& aux_arg) : aux(aux_arg) {
	this->chest_quest_id  = 0;
	this->update_flag     = false;
}

void General::scan_packet(std::vector<unsigned char>& arr) {
	chest_functions(arr);
}

void General::set_player_info(std::pair<unsigned int, std::string> player_basic_info) {
	this->player_basic_info = player_basic_info;
}

unsigned int General::get_chest_id() const {
	return this->chest_quest_id;
}

void General::clear_chest_id() {
	this->chest_quest_id = 0;
}

void General::chest_functions(std::vector<unsigned char>& arr) {
	this->captured_items(arr);
	this->get_chest_quest(arr);
}

void General::set_update_flag(bool f) {
	this->update_flag = f;
}

bool General::get_update_flag() const {
	return this->update_flag;
}

void General::captured_items(std::vector<unsigned char>& arr) {
	for (int i = 0; i < (int)arr.size() - 20; i++) {
		if (*(unsigned int*)&arr[i] == 0x9C141522) {
			unsigned int item        = *(unsigned int*)&arr[i + 5];
			unsigned int item_count  = *(unsigned int*)&arr[i + 13];
			std::cout << this->player_basic_info.second << " obteve: " << 
				        item << "  " << item_count << std::endl;
		}
	}
}

void General::get_chest_quest(std::vector<unsigned char>& arr) {
	for (int i = 0; i < (int)arr.size() - 11; i++) {
		if (*(unsigned int*)&arr[i] == 0x6A141522) {
			this->chest_quest_id = *(unsigned short*)&arr[i + 10];
			return;
		}
	}
}
