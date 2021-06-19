#include "stdafx.h"
#include "Message.h"

Message::Message(Aux_function& arg_aux) : aux(arg_aux) {
	permission.insert(1265);
	permission.insert(1264);
	permission.insert(980915);
	permission.insert(980928);
}

void Message::scan_packet(std::vector<unsigned char>& arr) {
	int index;
	bool find = 0;

	unsigned char end_line      = 0x69; // 0x5F 
	unsigned char long_message  = 0;

	for (index = 0; index < (int)arr.size() - 2; index++)
		if (arr[index] == 0x50) {
			if ((int)arr.size() - index >= index + 3 && arr[index + 3] == 0x2 && arr[index + 1] == 0x80) {
				long_message = 2;
				find = 1;
				break;
			}
			if (arr[index + 2] == 0x2) {
				find = 1;
				break;
			}
		}
		else if (arr[index] == 0x60) {
			if ((index + 1 + arr[index + 1]) < (int)arr.size()) { // 1.5.3 o "end line" é 0x69, em 1.5.1 é 0x5F
				//if (arr[index + 2] > 0 && arr[index + 2 + arr[index + 2]] == end_line) {
				//	find = 1;
				//	break;
				//}

				if (arr[index + 1 + arr[index + 1]] == end_line) {
					find = 1;
					break;
				}
			}
		}
	
	if (!find) return;

	// verificação irrelevante, por enquanto..
	if (arr[index] == 0x50) {
		list player_talk;

		if ((int)arr.size() < index + 9 ) return;

		if (long_message)
			player_talk = list(arr.begin() + index + 5, arr.begin() + index + 9);
		else
			player_talk = list(arr.begin() + index + 4, arr.begin() + index + 8);
		std::reverse(player_talk.begin(), player_talk.end());
		int player_message_id = *(int*)player_talk.data();

		//permissão de execução de comandos temporariamente comentado
		if (!this->permission.count(player_message_id) && !this->permission.empty())
			return;

		int message_size = arr[index + long_message + 8];
		list player_message(arr.begin() + index + 9 + long_message, arr.begin() + index + 9 + long_message + message_size);
		std::string s;
		for (const auto i : player_message)
			if (i != 0)
				s.append(1, (char)i);
		std::cout << player_message_id << ": " << s << std::endl;

		if (player_message.size() < 1) return;

		execute_message(s, 0x50, player_message_id);
	}
	else if (/*arr[index] == 0x60*/ false) {
		std::string s;
		unsigned char sender_name_size  = arr[4];
		unsigned char recive_name_size_offset = index + 4 + sender_name_size + 5;
		unsigned char receive_name_size = arr[recive_name_size_offset];
		unsigned int  player_message_id = aux.reverse_bytes_ex(
			*reinterpret_cast<unsigned int*>(&arr[4 + sender_name_size + 1]));

		/*if (!permission.count(player_message_id))
			return;*/
		unsigned char msg_size_offset = recive_name_size_offset + receive_name_size + 5;
		unsigned char message_size = arr[msg_size_offset];
		std::vector<unsigned char> message(arr.begin() + msg_size_offset + 1, arr.begin() +
			msg_size_offset + message_size);
		for (const auto i : message)
			if (i != 0)
				s.append(1, (char)i);
		std::cout << player_message_id << ": " << s << std::endl;

		execute_message(s, 0x60, player_message_id);
	}
}

void Message::send_private_message(unsigned int target_id, std::wstring message, bool is_friend) {
	//unsigned char unk_offset = 6;
	//unsigned char offset     = unk_offset + message.size() * 2;
	//std::vector<unsigned char> msg(18 + offset);
	//msg[0] = 0x60;
	//msg[1] = 16 + offset;

	//if (is_friend)
	//	msg[2] = 0x8; // 0x7 manda msg no pm, 0x8 manda msg no chat de amigo
	//else 
	//	msg[2] = 0x7;

	//msg[4] = 0x2;
	//msg[5] = 0x61;

	//this->player_basic_info.first = aux.reverse_bytes_ex(this->player_basic_info.first);
	//memcpy(&msg[7], &this->player_basic_info.first, sizeof(this->player_basic_info.first));

	//msg[11] = 0x2;
	//msg[12] = 0x61;

	//target_id = aux.reverse_bytes_ex(target_id);
	//memcpy(&msg[14], &target_id, sizeof(target_id));

	//msg[18] = message.size() * 2;

	//memmove(&msg[19], message.data(), message.size() * 2);
	//aux.crypt_send(msg);

	unsigned char unk_offset  = 6;
	unsigned char offset      = unsigned char(unk_offset + message.size() * 2);
	std::vector<unsigned char> msg(22 + offset);
	msg[0] = 0x60;
	msg[1] = 20 + offset;

	if (is_friend)
		msg[2] = 0x8; // 0x7 manda msg no pm, 0x8 manda msg no chat de amigo
	else
		msg[2] = 0x7;

	msg[4] = 0x4;
	msg[5] = 0x61;
	msg[6] = 0x0;
	msg[7] = 0x61;
	msg[8] = 0x0;

	this->player_basic_info.first = aux.reverse_bytes_ex(this->player_basic_info.first);
	memcpy(&msg[9], &this->player_basic_info.first, sizeof(this->player_basic_info.first));

	msg[13] = 0x4;
	msg[14] = 0x61;
	msg[15] = 0x0;
	msg[16] = 0x61;
	msg[17] = 0x0;

	target_id = aux.reverse_bytes_ex(target_id);
	memcpy(&msg[18], &target_id, sizeof(target_id));

	msg[22] = message.size() * 2;

	memmove(&msg[23], message.data(), message.size() * 2);
	aux.crypt_send(msg);

}

void Message::send_party_message(std::wstring message) {
	unsigned int static_size = 14;

	std::vector<unsigned char> msg(static_size + message.size() * 2);
	msg[0] = 0x4F;
	msg[1] = static_size + message.size() * 2 - 2;
	msg[2] = 0x2;

	unsigned int player_id = aux.reverse_bytes_ex(this->player_basic_info.first);
	memmove(&msg[4], &player_id, sizeof(player_id));

	msg[12] = message.size() * 2;
	
	memmove(&msg[13], message.data(), message.size() * 2);
	aux.crypt_send(msg);
}

void Message::set_enviroment(std::shared_ptr<Inventory> inventory_arg,
	std::shared_ptr<Basic_actions> basic_actions_arg, std::shared_ptr<Movement> movement_arg, 
	std::shared_ptr<Items>   items_arg, std::shared_ptr<Craft> craft_arg, std::shared_ptr<Party> party_arg,
	std::shared_ptr<Healing> healing_arg, std::shared_ptr<Trade> trade_arg) {
	this->inventory      = inventory_arg;
	this->basic_actions  = basic_actions_arg;
	this->movement       = movement_arg;
	this->items          = items_arg;
	this->craft          = craft_arg;
	this->party          = party_arg;
	this->healing        = healing_arg;
	this->trade          = trade_arg;
}

void Message::set_player_info(std::pair<unsigned int, std::string> player_basic_info) {
	this->player_basic_info = player_basic_info;
}

void Message::add_permission(unsigned int player_id) {
	this->permission.insert(player_id);
}

std::set<unsigned int>& Message::get_permission_list() {
	return this->permission;
}

void Message::execute_message(std::string message, unsigned char type, unsigned int player_id) {
	std::string player_name = this->player_basic_info.second;
	std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

	auto skip_space = [message](unsigned int start_to_search) -> int {
		for (size_t i = start_to_search; i < message.size(); i++)
			if (message[i] != 0x20)
				return i;
		return -1;
	};

	auto find_space = [message](unsigned int start_to_search) -> int {
		for (size_t i = start_to_search; i < message.size(); i++)
			if (message[i] == 0x20)
				return i;
		return -1;
	};

	auto is_number  = [message](std::string s) -> bool {
		for (auto& i : s)
			if (std::isalpha(i))
				return false;
		return true;
	};

	auto send_inventory = [](unsigned int player_id, std::multimap<unsigned int, item_data> arr,
		Message& m, bool type) {
			for (auto i : arr) {
				std::wstring msg = L"test";
				if (msg.empty())
					msg += L"Unknown";
				msg += L" id: ";
				msg += std::to_wstring(i.second.id);
				msg += L" qts: ";
				msg += std::to_wstring(i.second.stack);
				msg += L" pos: ";
				msg += std::to_wstring(i.second.slot_position);
				m.send_party_message(msg);
				Sleep(1100);
			}
	};

	//constexpr auto basic_hash = [](const char arr[]) -> unsigned int {
	//	unsigned int r = 0;
	//	const unsigned int magic_number = 23;

	//	int i = 0;

	//	while (arr[i] != 0) {
	//		r += arr[i];
	//		i++;
	//	}

	//	return r * magic_number;
	//};

	std::vector<std::string> arguments;

	if (type == 0x50) {
		std::transform(message.begin(), message.end(), message.begin(), ::tolower);

		unsigned int pos    = message.find(player_name + ":");
		unsigned int pos_1  = message.find("all:");

		unsigned int space_pos;

		if (pos == 0)
			space_pos = player_name.size() + 1; // start_pos está após os :, ou seja, aponta pro space character
		else if (pos_1 == 0)
			space_pos = std::string("all:").size();
		else
			return;

		unsigned int k = 0;

		while ((k = skip_space(space_pos)) != -1) {
			unsigned int a = find_space(k);

			if (a != -1) {
				arguments.push_back(std::string(message.begin() + k, message.begin() + a));
				space_pos = a;
			}
			else {
				arguments.push_back(std::string(message.begin() + k, message.end()));
				break;
			}
		}
	}
	else if (type == 0x60) {

	}

	if (arguments.size() < 1) return;

	switch (basic_hash(arguments[0].c_str())) {

	// permissões

	case basic_hash("add_group_permission"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			party->add_permission(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("add_trade_permission"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			trade->add_permission(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("add_command_permission"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			this->add_permission(std::stoi(arguments[1]));
		break;
	}

	// habilitar funcionalidades

	case basic_hash("toggle_trade"): {
		if (arguments.size() >= 1) {
			if (trade->get_update_flag())
				trade->set_update_flag(false);
			else
				trade->set_update_flag(true);
		}
		break;
	}
	case basic_hash("toggle_movement"): {
		if (arguments.size() >= 1) {
			if (movement->get_update_flag())
				movement->set_update_flag(false);
			else 
				movement->set_update_flag(true);
		}
		break;
	}
	case basic_hash("toggle_items"): {
		if (arguments.size() >= 1) {
			if (items->get_update_flag())
				items->set_update_flag(false);
			else
				items->set_update_flag(true);
		}
		break;
	}
	case basic_hash("toggle_healing"): {
		if (arguments.size() >= 1) {
			if (healing->get_update_flag())
				healing->set_update_flag(false);
			else
				healing->set_update_flag(true);
		}
		break;
	}
	case basic_hash("toggle_inventory"): {
		if (arguments.size() >= 1) {
			if (inventory->get_update_flag())
				inventory->set_update_flag(false);
			else
				inventory->set_update_flag(true);
		}
		break;
	}
	case basic_hash("toggle_party"): {
		if (arguments.size() >= 1) {
			if (party->get_update_flag())
				party->set_update_flag(false);
			else
				party->set_update_flag(true);
		}
		break;
	}

    // buffs

	case basic_hash("buff_me"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			basic_actions->send_basic_skills(std::stoi(arguments[1]), player_id);
		break;
	}
	case basic_hash("auto_buf"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			basic_actions->send_basic_skills(std::stoi(arguments[1]), player_basic_info.first);
		break;
	}
	case basic_hash("drop"): {
		if (arguments.size() >= 3 && is_number(arguments[1]) && is_number(arguments[2]))
			basic_actions->drop_items(std::stoi(arguments[2]), std::stoi(arguments[1]));;
		break;
	}
	case basic_hash("dialog"): {
		if (arguments.size() >= 2 && is_number(arguments[1])) {
			basic_actions->select_npc(std::stoi(arguments[1]));
			basic_actions->open_npc(std::stoi(arguments[1]));
		}
		break;
	}
    
	// movimento

	case basic_hash("mount"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			basic_actions->mount(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("move_vert"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			//movement->move_vertial(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("fly"): {
		if (arguments.size() >= 1)
			movement->get_fly();
		break;
	}
	case basic_hash("set_fly_id"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			movement->set_fly(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("follow"): {
		if (arguments.size() >= 1) {
			if (movement->follow) {
				movement->follow.set_target_id(0);
				break;
			}
			else if (movement->get_update_flag()) {
				movement->follow.set_target_id(player_id);
				std::thread t_thread(&Movement::move_on_air, &*movement);
				t_thread.detach();
			}
		}
		break;
	}
	case basic_hash("follow_ground"): {
		if (arguments.size() >= 1) {
			if (movement->follow) {
				movement->follow.set_target_id(0);
				break;
			}
			movement->follow.set_target_id(player_id);
			std::thread t_thread(&Movement::move_on_ground, &*movement);
			t_thread.detach();
		}
		break;
	}
	case basic_hash("set_speed"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			movement->set_speed(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("get_speed"): {
		if (arguments.size() >= 1)
			send_party_message(std::to_wstring(movement->get_speed()));
		break;
	}
	case basic_hash("desmount"): {
		if (arguments.size() >= 1)
			basic_actions->desmount();
		break;
	}


	case basic_hash("get_count_item_by_id"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			send_party_message(std::to_wstring(inventory->get_item_count_by_id(std::stoi(arguments[1]))));
		break;
	}
	case basic_hash("get_empty_slots"): {
		if (arguments.size() >= 1)
			send_party_message(std::to_wstring(inventory->get_count_empty_slots()));
		break;
	}
	case basic_hash("get_inventory"): {
		if (arguments.size() >= 1)
			inventory->get_info();
		break;
	}
	case basic_hash("get_inventory_pos"): {
		if (arguments.size() >= 2 && is_number(arguments[1])) {
			auto p = inventory->get_list_item();

			for (const auto i : p) {
				if (i.second.slot_position == std::stoi(arguments[1])) {
					std::wstring msg = L"";
					//if (msg.empty())
					//	msg += L"Unknown";
					msg += L" id: ";
					msg += std::to_wstring(i.second.id);
					msg += L" qts: ";
					msg += std::to_wstring(i.second.stack);
					send_party_message(msg);
					break;
				}
			}
		}
		break;
	}
	case basic_hash("clear"): {
		if (arguments.size() >= 1)
			system("cls");
		break;
	}
	case basic_hash("use"): // accept_recall
	{
		if (arguments.size() >= 2 && is_number(arguments[1])) {
			int item_id = std::stoi(arguments[1]);
			auto item_list = inventory->get_linear_list_item();
			for (auto a : item_list) {
				if (item_id == a.id)
					basic_actions->use_item(item_id, a.slot_position);
			}
		}
			
		break;
	}
	case basic_hash("recall"):
	{
		if (arguments.size() >= 1)
			basic_actions->accept_recall(player_id);
		break;
	}
	case basic_hash("leader"): {
		if (arguments.size() >= 1)
			party->transfer_leadership(player_id);
		break;
	}
	case basic_hash("clear_group_permission"): {
		if (arguments.size() >= 1)
			party->clear_permission_list();
		break;
	}
	case basic_hash("items_size"): {
		if (arguments.size() >= 1)
			std::cout << "list size: " << std::dec << items->items_count() << std::endl;
		break;
	}
	case basic_hash("quit_group"): {
		if (arguments.size() >= 1)
			party->leave_party();
		break;
	}
	case basic_hash("atk"): {
		if (arguments.size() >= 1) {
			Sleep(2000);
			basic_actions->normal_attack();
		}
		break;
	}
	case basic_hash("get_item_loop"): {
		if (arguments.size() >= 1) {
			std::vector<unsigned int> list_items;

			if (arguments.size() == 1) {
				items->set_scannning_flag(true);
				std::thread t_thread(&Items::start_scanning, &*items, list_items, 7.f);
				t_thread.detach();
			}
			else {
				for (size_t i = 1; i < arguments.size(); i++) {
					if (is_number(arguments[i])) {
						list_items.push_back(std::stoi(arguments[i]));
					}
					else if (!list_items.empty()) {
						items->set_scannning_flag(true);
						std::thread t_thread(&Items::start_scanning, &*items, list_items, 7.f);
						t_thread.detach();
						break;
					}
					else
						break;
				}

				items->set_scannning_flag(true);
				std::thread t_thread(&Items::start_scanning, &*items, list_items, 7.f);
				t_thread.detach();
			}
		}

		break;
	}
	case basic_hash("item_set_time_wait"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			items->set_time_wait(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("swap_item"): {
		if (arguments.size() >= 3 && is_number(arguments[1]) && is_number(arguments[2]))
			basic_actions->swap_items(std::stoi(arguments[1]), std::stoi(arguments[2]));
		break;
	}
	case basic_hash("get_item_loop_stop"): {
		items->set_scannning_flag(false);
		break;
	}
	case basic_hash("drop_coin"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			basic_actions->drop_coin(std::stoi(arguments[1]));
		break;
	}
	case basic_hash("get_coord"): {
		if (arguments.size() >= 1) {
			std::cout << "x: " << movement->player_x << "y: "
				<< movement->player_y << "z: " << movement->player_z << std::endl;
		}
		break;
	}

    // forjas

	case basic_hash("forge"): {
		if (arguments.size() >= 1) {
			// id dos itens necessários para forjar o item
			craft->item.id              = 32270; // 32277 peito merc
			craft->item.position        = 0;
			craft->item.recipe          = 5580;  // 5513 machado branco
			craft->item.level           = 100;   // necessário, pois é usado como pattern na busca do pacote de forja e reforja
			craft->item.sub_item_1_id   = 11208;
			craft->item.sub_item_1_pos  = 1;

			// essa informação é necessária, pois é usada pra encontrar os atributos do item
			craft->item.number_especial_attributes = 3;

			// atributos
			craft->attributes.push_back({ 0x2ABC, 0, 0x2ABC, 0, 0x2ABC, 0 });
			craft->attributes.push_back({ 0x214B, 0, 0x214B, 0, 0x214B, 0 });
			craft->attributes.push_back({ 0x2ABA, 0, 0x2ABA, 0, 0x2ABA, 0 });

			// busca por esses itens no invetário caso eles cheguem a quantidade menor ou igual a 0
			// e os poe nas respectivas posições
			craft->items_recharge.push_back({ 32270, 0, 0 });
			craft->items_recharge.push_back({ 11208, 1, 0 });

			craft->start_spirit_crafting();
		}
		break;
	}
	case basic_hash("forge2"): {
		if (arguments.size() >= 1) {
			basic_actions->open_npc(-2146399292);
			basic_actions->select_npc(-2146399292);
			Sleep(100);

			//basic_actions->send_basic_skills(0x4F9, this->player_basic_info.first);

			craft->item.id       = 32270; // 20393 id anel
			craft->item.recipe   = 1488;  // 3045 recipe anel
			craft->item.count    = 1;
			craft->item.price    = 60000;
			craft->item.level    = 100;
			//craft->item_atrib.atb_id.push_back(0x2253); // 0x2252 3 % cast.. 0x2253 6% cast
			//craft->item_atrib.atb_count.push_back(0);

			craft->attributes.push_back({ 0x21A4, 0, 0x21A4, 0 });
			//craft->attributes.push_back({ 0x2252, 0 });

			craft->item.number_especial_attributes = 4;

			craft->start_item_crafting();
		}
		break;
	}
	case basic_hash("rotate"): {
		if (arguments.size() >= 1) {
			craft->item.id        = 32277; // 20393 id anel
			craft->item.position  = 0;
			craft->item.level     = 100;
			craft->item.recipe    = 0x13A8;

			// soma dos atributos
			craft->attributes_sum = 24;

			// a função craft->set_created_by_name deve ser setada com o nome do player que forjou o item caso
			// o item não tenha sido forjado pelo próprio personagem

			//craft->set_created_by_name("rato_sujo");

			craft->item.number_especial_attributes = 3;

			craft->start_rotate_crafting();
		}
		break;
	}
	case basic_hash("forge2_cancel"): {
		if (arguments.size() >= 1) {
			craft->craft_flag = false;
		}
		break;
	}
	case basic_hash("recuar_estado"): {
		if (arguments.size() >= 1) {
			Timer time;
			while (time.elapsed() < 3000) {
				basic_actions->normal_attack();
				Sleep(50);
			}
		}
		break;
	}
	case basic_hash("equip"): {
		if (arguments.size() >= 1) {
			basic_actions->equip_item(0, 0);
		}
		break;
	}							

	case basic_hash("drop_item"): {
		if (arguments.size() >= 3 && is_number(arguments[1]) && is_number(arguments[2]))
			inventory->drop_item_by_id(std::stoi(arguments[1]), std::stoi(arguments[2]));
		break;
	}
	case basic_hash("get_items"): {
		if (arguments.size() >= 2 && is_number(arguments[1]))
			items->get_all_items({ (unsigned int)std::stoi(arguments[1]) });
		break;
	}

	default:
		break;
	}
}

//list items = 0 80 x 22 80 x 80 padrão ?
//
//list items sumiu = 0 21 22[21 - 2][21 - 3] 22 0 size
//list items apareceu = 0 80 x 22 80[x - 3] 80 x a