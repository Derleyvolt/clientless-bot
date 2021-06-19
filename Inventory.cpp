#include "stdafx.h"
#include "Inventory.h"
#include <iostream>

Inventory::Inventory(Aux_function& arg_aux) : aux(arg_aux) {
	this->capacity     = 0;
	this->check_info   = false;
	this->update_time  = 60000;
	this->update_flag  = false;
}

void Inventory::set_enviroment(std::shared_ptr<Basic_actions> actions_arg) {
	this->basic_actions = actions_arg;
}

bool find_values(std::vector<unsigned char>& arr, unsigned int value, int& address) {
	int size = arr.size();
	for (int i = 0; i < size - 4; i++) {
		if (*(unsigned int*)&arr[i] == value) {
			address = i;
			return true;
		}
	}
	return false;
}

void Inventory::print_items() {
	std::cout << "quantidade de itens: " << items.size() << std::endl;

	for (size_t i = 0; i < arr_items.size(); i++) {
		if (arr_items[i].id > 0)
			std::cout << std::dec << "item id: " << arr_items[i].id << " qts : "
				<< arr_items[i].stack << " slot: " << (int)arr_items[i].slot_position << std::endl;
		else
			std::cout << std::dec << "item id: " << arr_items[i].id << " qts : " 
			<< arr_items[i].stack << " slot: " << i << std::endl;
	}
}

void Inventory::scan_packet(std::vector<unsigned char>& decode_data) {
	if (!check_info) return;

	int index = 0;
	bool flag = 0;

	for (; index < (int)decode_data.size() - 6; index++) {
		if (decode_data[index]     == 0x22 && decode_data[index + 1] == 0x80 &&
			decode_data[index + 3] == 0x80 && decode_data[index + 5] == 0x32) {
			//std::cout << index << std::endl;
			flag = 1;
			break;
		}
	}
	
	if (!flag)
		return;

	if (index + capacity_offset >= decode_data.size()) {
		check_info = false;
		return;
	}

	items.clear();
	this->arr_items.clear();

	this->capacity = decode_data[index + capacity_offset];
	this->arr_items.resize(this->capacity);

	item_data item;
	unsigned int slot_position = this->capacity,
		         start_index   = index + start_offset;

	while (slot_position) {
		if (*reinterpret_cast<int*>(&decode_data[start_index]) != 0xFFFFFFFF) {
			memset(&item, 0, sizeof(item));

			item.id               = *reinterpret_cast<unsigned int*>(&decode_data[start_index]);
			item.stack            = *reinterpret_cast<unsigned int*>(&decode_data[start_index + 8]);
			item.slot_position    = this->capacity - slot_position;

			this->items.insert(std::make_pair(item.id, item));
			this->arr_items[this->capacity - slot_position] = item;
			start_index += 12;
		}
		else {
			start_index += 4;
		}
			
		slot_position -= 1;
	}

	//print_items();
}

std::multimap<unsigned int, item_data> Inventory::get_list_item() const {
	return items;
}

std::vector<item_data> Inventory::get_linear_list_item() const {
	return this->arr_items;
}

std::vector<int> Inventory::list_to_empty_slots() const {
	std::vector<int> arr;
	for (int i = 0; i < arr_items.size(); i++) {
		if (arr_items[i].id == 0) {
			arr.push_back(i);
		}
	}
	return arr;
}

std::vector<int> Inventory::list_to_items_by_id(int item_id) const {
	std::vector<int> arr;
	for (int i = 0; i < arr_items.size(); i++) {
		if (arr_items[i].id == item_id) {
			arr.push_back(i);
		}
	}
	return arr;
}

void Inventory::drop_item_by_id(unsigned int id, unsigned int count) {
	if (items.count(id)) {
		auto r = items.find(id);
		if (r->second.stack < count)
			count = r->second.stack;
		basic_actions->drop_items(r->second.slot_position, count);
	}
}

void Inventory::set_update_time(unsigned int time) {
	this->update_time = time;
}

unsigned int Inventory::get_update_time() const {
	return this->update_time;
}

void Inventory::start_update() {
	if (!update_flag)  
		return;
	this->t_thread = std::make_shared<std::thread>(&Inventory::update, this);
	this->t_thread->detach();
}

int Inventory::get_first_empty_slot() const {
	for (size_t i = 0; i < arr_items.size(); i++)
		if (arr_items[i].id == 0)
			return i;
	return -1;
}

void Inventory::set_update_flag(bool f) {
	this->update_flag = f;
}

bool Inventory::get_update_flag() const {
	return this->update_flag;
}

void Inventory::update() {
	while (update_flag) {
		this->get_info();
		Sleep(this->update_time);
	}
}

unsigned char Inventory::get_item_count() const {
	return static_cast<unsigned char>(items.size());
}

std::optional<item_data> Inventory::search_item_by_id(unsigned int id) {
	if (this->items.count(id))
		return this->items.find(id)->second;
	return std::nullopt;
}

unsigned int Inventory::get_item_count_by_id(unsigned int id) const {
	unsigned int stack = 0;
	if (items.empty()) return stack;

	unsigned int count = items.count(id);

	if (count > 0) {
		auto r = items.find(id);

		for (size_t i = 0; i < count; ++i) {
			stack += r->second.stack;
			r++;
		}

		return stack;
	}

	return stack;
}

unsigned char Inventory::get_capacity() const {
	return this->capacity;
}

unsigned char Inventory::get_count_empty_slots() const {
	if (check_info || items.empty()) return -1;
	return unsigned char(this->capacity - items.size());
}

// 0x22, 0x06, 0x05, 0x27, 00, 01, 01, 00
void Inventory::get_info() {
	std::vector<unsigned char> get_Inventory = { 0x22, 0x06, 0x05, 0x27, 00, 00, 01, 01 };
	check_info = true;
	aux.crypt_send(get_Inventory);
}