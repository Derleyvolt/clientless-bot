#include "stdafx.h"
#include "Items.h"

Items::Items(Aux_function& aux_arg) : aux(aux_arg) {
	this->scanning_item  = false;
	this->time_to_wait   = 20000;
	this->update_flag    = false;
}

void Items::scan_packet(std::vector<unsigned char>& arr) {
	int index;

	if (!this->update_flag) return;

	Item item;

	int size = arr.size();

	// get items
	for (index = 0; index < size - 7; index++) {
		if (index >= 3 && arr[index] == 0x22 && arr[index + 5] == 0xA &&
			arr[index + 1] == arr[index - 2] && arr[index + 3] == arr[index + 1]) {

			unsigned int id_unique = *reinterpret_cast<unsigned short*>(&arr[index + 7]);

			for (size_t i = 0; i < id_unique; i++) {
				item.id_unique = *reinterpret_cast<unsigned int*>(&arr[index + 9 + i * 25]);
				item.id  = 0;
				memmove(&item.id, &arr[index + 9 + i * 25 + 4], 3);
				//item.id  = *reinterpret_cast<unsigned int*>(&arr[index + 9 + i * 25 + 4]);

				item.coord.x = *reinterpret_cast<float*>(&arr[index + 9 + i * 25 + 8]);
				item.coord.z = *reinterpret_cast<float*>(&arr[index + 9 + i * 25 + 12]);
				item.coord.y = *reinterpret_cast<float*>(&arr[index + 9 + i * 25 + 16]);


				items_list.insert(std::make_pair(item.id_unique, item));
			}
		}
		else if (index >= 2 && arr[index] == 0x22 && arr[index + 3] == 0xA &&
			arr[index + 1] == arr[index + 2] + 1  && arr[index + 4] == 0) {

			unsigned int id_unique = *reinterpret_cast<unsigned short*>(&arr[index + 5]);

			for (size_t i = 0; i < id_unique; i++) {
				item.id_unique = *reinterpret_cast<unsigned int*>(&arr[index + 7 + i * 25]);
				item.id = 0;
				memmove(&item.id, &arr[index + 7 + i * 25 + 4], 3);
				//item.id = *reinterpret_cast<unsigned int*>(&arr[index + 7 + i * 25 + 4]);

				item.coord.x = *reinterpret_cast<float*>(&arr[index + 7 + i * 25 + 8]);
				item.coord.z = *reinterpret_cast<float*>(&arr[index + 7 + i * 25 + 12]);
				item.coord.y = *reinterpret_cast<float*>(&arr[index + 7 + i * 25 + 16]);

				items_list.insert(std::make_pair(item.id_unique, item));
			}
		}
	}
	// sumiu item
	for (index = 0; index < size - 7; index++) {
		if (index >= 3 && arr[index] == 0x22 && arr[index + 5] == 0x22 && 
			arr[index + 1] == arr[index - 2] && arr[index + 3] == arr[index + 1]) {

			unsigned int items_size = *reinterpret_cast<unsigned int*>(&arr[index + 7]);

			for (size_t i = 0; i < items_size; i++) {
				unsigned int id_unique = *reinterpret_cast<unsigned int*>(&arr[index + 11 + i * 4]);
				if (*(reinterpret_cast<unsigned char*>(&id_unique) + 3) == 0xC0) {
					std::cout << std::hex << id_unique << std::endl;
					items_list.erase(id_unique);
				}
			}
		}
		else if (index >= 2 && arr[index] == 0x22 && arr[index + 3] == 0x22 &&
                 arr[index + 1] == arr[index + 2] + 1) {

			unsigned int items_size = *reinterpret_cast<unsigned int*>(&arr[index + 5]);

			for (size_t i = 0; i < items_size; i++) {
				unsigned int id_unique = *reinterpret_cast<unsigned int*>(&arr[index + 9 + i * 4]);
				if (*(reinterpret_cast<unsigned char*>(&id_unique) + 3) == 0xC0) {
					std::cout << std::hex << id_unique << std::endl;
					items_list.erase(id_unique);
				}
			}
		}
	}
	// get items
	for (index = 0; index < size - 2; index++) {
		if (arr[index] == 0x22 && arr[index + 1] == 0x1C && arr[index + 2] == 0x1B) {
			item.coord.x = *reinterpret_cast<float*>(&arr[index + 13]);
			item.coord.z = *reinterpret_cast<float*>(&arr[index + 17]);
			item.coord.y = *reinterpret_cast<float*>(&arr[index + 21]);

			item.id_unique = *reinterpret_cast<unsigned int*>(&arr[index + 5]);
			item.id = 0;
			memmove(&item.id, &arr[index + 9], 3);
			//item.id  = *reinterpret_cast<unsigned int*>(&arr[index + 9]);

			items_list.insert(std::make_pair(item.id_unique, item));
		}
	}
	// item sumiu
	for (index = 0; index < size - 3; index++) {
		if (arr[index] == 0x22 && arr[index + 1] == 0xB && arr[index + 2] == 0xA && arr[index + 3] == 0x98) {
			items_list.erase(*reinterpret_cast<unsigned int*>(&arr[index + 5]));
		}
	}
	// item sumiu
	for (index = 0; index < size - 3; index++) {
		if (arr[index] == 0x22 && arr[index + 1] == 0x7 && arr[index + 2] == 0x6 && arr[index + 3] == 0x15)
			items_list.erase(*reinterpret_cast<unsigned int*>(&arr[index + 5]));
	}

	system("cls");

	std::cout << "qtd: " << items_list.size() << " " << std::endl;

	for (auto [id_unique, item] : items_list) {
		std::cout << id_unique << " " << item.id << std::endl;
	}

}

void Items::set_enviroment(std::shared_ptr<Movement> movement_arg) {
	this->movement = movement_arg;
}

void Items::get_initial_objects(std::vector<unsigned char>& arr) {
	int index;
	for (index = 0; index < (int)arr.size() - 5; index++) {
		if (arr[index] == 0x22 && arr[index + 1] == arr[index + 2] + 1 && arr[index + 3] == 0xA) {
			unsigned short list_size = *reinterpret_cast<unsigned short*>(&arr[index + 5]);

			//25 = size com a informação de cada item

			if (list_size * 25 + 5 != arr[index + 1])
				return;

			Item item;
			for (size_t i = 0; i < list_size; i++) {
				item.id_unique  = *reinterpret_cast<unsigned int*>(&arr[index + 7 + i * 25]);
				item.id         = *reinterpret_cast<unsigned int*>(&arr[index + 7 + i * 25 + 4]);

				item.coord.x = *reinterpret_cast<float*>(&arr[index + 7 + i * 25 + 8]);
				item.coord.z = *reinterpret_cast<float*>(&arr[index + 7 + i * 25 + 12]);
				item.coord.y = *reinterpret_cast<float*>(&arr[index + 7 + i * 25 + 16]);

				items_list.insert(std::make_pair(item.id_unique, item));
			}
		}
		else if (arr[index] == 0x22 && arr[index + 1] == arr[index + 3] && arr[index + 5] == 0xA) {
			unsigned short list_size = *reinterpret_cast<unsigned short*>(&arr[index + 7]);

			Item item;
			for (size_t i = 0; i < list_size; i++) {
				item.id_unique  = *reinterpret_cast<unsigned int*>(&arr[index + 9 + i * 25]);
				item.id         = *reinterpret_cast<unsigned int*>(&arr[index + 9 + i * 25 + 4]);

				item.coord.x = *reinterpret_cast<float*>(&arr[index + 9 + i * 25 + 8]);
				item.coord.z = *reinterpret_cast<float*>(&arr[index + 9 + i * 25 + 12]);
				item.coord.y = *reinterpret_cast<float*>(&arr[index + 9 + i * 25 + 16]);

				items_list.insert(std::make_pair(item.id_unique, item));
			}
		}
	}

	//debug items packet

	//system("cls");

	//std::cout << "qtd: " << items_list.size() << " " << std::endl;

	//for (auto [id_unique, item] : items_list) {
	//	std::cout << id_unique << " " << item.id << std::endl;
	//}
}

void Items::pick(unsigned int id_unique, unsigned int id) {
	std::vector<unsigned char> arr = { 0x22, 0x0B, 0x0A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	memmove(arr.data() + 5, &id_unique, sizeof(id_unique));
	memmove(arr.data() + 9, &id, sizeof(id));
	aux.crypt_send(arr);
}

bool Items::pick_items(std::vector<unsigned int> arr_id, float distance) {
	clear_black_list();

	std::multimap<unsigned int, Item> item_in_radius;

	for (const auto i : items_list)
		if (this->distance(i.second.coord) <= distance && !black_list.count(i.first))
			item_in_radius.insert(std::make_pair(i.second.id, i.second));

	float dist = 1000.f;
	Item item;

	if (!arr_id.empty()) {
		for (const auto c : item_in_radius) {
			for (const auto i : arr_id) {
				if (i == c.first) {
					float b = this->distance(c.second.coord);
					if (b < dist) {
						dist = b;
						item = c.second;
					}
					break;
				}
			}
		}

		if (dist != 1000.f) {
			pick(item.id_unique, item.id);
			add_to_black_list(item.id_unique);
			return 1;
		}

		//for (const auto i : arr_id) {
		//	if (item_in_radius.count(i)) {
		//		auto r = item_in_radius.find(i);

		//		pick(r->second.id_unique, r->second.id);
		//		add_to_black_list(r->second.id_unique);
		//		return 1;
		//	}
		//}

		return 0;
	}
	else {
		for (const auto c : item_in_radius) {
			float b = this->distance(c.second.coord);
			if (b < dist) {
				dist = b;
				item = c.second;
			}
		}

		if (dist != 1000.f) {
			pick(item.id_unique, item.id);
			add_to_black_list(item.id_unique);
			return 1;
		}

		return 0;
	}
}

void Items::set_time_wait(unsigned char time) {
	this->time_to_wait = time;
}

void Items::set_scannning_flag(bool f) {
	this->scanning_item = f;
}

void Items::start_scanning(std::vector<unsigned int> arr, float distance) {
	while (scanning_item) {
		if (pick_items(arr, distance))
			Sleep(700);
		Sleep(1);
	}
}

void Items::get_all_items(std::vector<unsigned int> items) {
	if (this->items_list.empty())
		return;

	clear_black_list();

	int limit = 10;
	std::vector<std::pair<unsigned int, unsigned int>> items_to_catch;

	for (auto& i : items) {
		if (limit <= 0) break;
		for (auto& k : this->items_list) {
			if (i == k.second.id && !in_black_list(k.first)) {
				if (distance(k.second.coord) <= 10) {
					items_to_catch.push_back(std::make_pair(k.first, k.second.id));
					add_to_black_list(k.first);
					limit--;
					if (limit <= 0)
						break;
				}
			}
		}
	}

	this->get_all_items_unmanaged(items_to_catch);
}

unsigned int Items::items_count() const {
	return items_list.size();
}

void Items::set_update_flag(bool f) {
	this->update_flag = f;
}

bool Items::get_update_flag() const {
	return this->update_flag;
}

void Items::clear_black_list() {
	using namespace std::chrono;

	time_point time = steady_clock::now();

	std::vector<unsigned int> id_unique;

	for (const auto i : black_list)
		if (duration_cast<milliseconds>(time - i.second).count() >= time_to_wait)
			id_unique.push_back(i.first);

	if (id_unique.empty()) return;

	for (const auto i : id_unique)
		black_list.erase(i);
}

bool Items::in_black_list(unsigned int id_unique) {
	return black_list.count(id_unique);
}

void Items::add_to_black_list(unsigned int id_unique) {
	black_list.insert(std::make_pair(id_unique, std::chrono::steady_clock::now()));
}

float Items::distance(Coord coord) {
	float c_1 = coord.x - movement->player_x;
	float c_2 = coord.y - movement->player_y;
	float c_3 = coord.z - movement->player_z;
	return sqrt(pow(c_1, 2) + pow(c_2, 2) + pow(c_3, 2));
}

void Items::get_all_items_unmanaged(std::vector<std::pair<unsigned int, unsigned int>> items) {
	// B8 00 01 00 00 00 8C ED 10 C0 E4 0B 00 00 

	if (items.empty())
		return;

	std::vector<unsigned char> arr;
	// 3 bytes do header
	// 6 bytes do sub-header
	// 8 bytes por item

	arr.resize(items.size() * 8 + 3 + 6);

	arr[0] = 0x22;
	arr[1] = items.size() * 8 + 1 + 6;
	arr[2] = items.size() * 8 + 6;
	arr[3] = 0xB8;
	arr[5] = (unsigned char)items.size();

	for (size_t i = 0; i < items.size(); i++) {
		memmove(&arr[i * 8 + 9],   &items[i].first,  sizeof(items[i].first)); // id_unique
		memmove(&arr[i * 8 + 13], &items[i].second, sizeof(items[i].second));
	}

	aux.crypt_send(arr);
}