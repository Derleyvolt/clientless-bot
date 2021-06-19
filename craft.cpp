#include "stdafx.h"
#include "craft.h"

Craft::Craft(Aux_function& arg_aux) : aux(arg_aux) {
	this->craft_flag          = false;
	this->buffer_find_flag    = false;
	this->sell_find_flag      = false;
	this->attributes_sum      = -1;
	set_atributes_name();
}

void Craft::set_atributes_name() {
	// RANK 8
	this->atributes_name.insert({ 0x2AB4, "HP" }); // armadura
	this->atributes_name.insert({ 0x2ABA, "DEX" }); // armadura
	this->atributes_name.insert({ 0x2ABB, "CON" }); // armadura
	this->atributes_name.insert({ 0x2AC3, "DEF MAGICA" }); // armadura
	this->atributes_name.insert({ 0x2AC4, "DEF FISICA" }); // armadura
	this->atributes_name.insert({ 0x2ABF, "ESQ" }); // armadura
	this->atributes_name.insert({ 0x2AB8, "INT" }); // armadura
	this->atributes_name.insert({ 0x2AB9, "FOR" }); // armadura
	this->atributes_name.insert({ 0x214B, "INTERVALO" }); // armadura
	this->atributes_name.insert({ 0x2AB6, "MP" }); // armadura
	this->atributes_name.insert({ 0x2AC2, "CRITICO" }); // armadura
	this->atributes_name.insert({ 0x2AC5, "RED DANO FISICO" }); // armadura
	this->atributes_name.insert({ 0x2AC6, "RED DANO ELEMENTAL" }); // armadura
	this->atributes_name.insert({ 0x2ABC, "RED FISIC %" }); // armadura
	this->atributes_name.insert({ 0x2ABD, "RED MAGICA %" }); // armadura
	this->atributes_name.insert({ 0x21A4, "INTERVALO ARMA" }); // arma
	this->atributes_name.insert({ 0x7D0, "NIVEL ATK PRINCIPAL" }); // arma
	this->atributes_name.insert({ 0x2AA4, "NIVEL DEF PRINCIPAL" }); // arma
	this->atributes_name.insert({ 0x48E3, "DEUS FRENESI" }); // arma
	this->atributes_name.insert({ 0x2AA3, "NIVEL DEF SECUNDARIO" }); // arma
	this->atributes_name.insert({ 0x2AAC, "DEF FISICA" }); // arma
	this->atributes_name.insert({ 0x2AA2, "NIVEL DE ATK PRINCIPAL" }); // arma
	this->atributes_name.insert({ 0x2AA0, "CON ARMA" }); // arma
	this->atributes_name.insert({ 0x2AAD, "DEF MAGICA ARMA" }); // arma
	this->atributes_name.insert({ 0x2AA1, "NIVEL DE ATK SECUNDARIO" }); // arma
	this->atributes_name.insert({ 0x2AAF, "REDUCAO DOS ELEMENTOS" }); // arma
	this->atributes_name.insert({ 0x2AAE, "REDUCAO DANO FISICO" }); // arma
	this->atributes_name.insert({ 0x2AA6, "CRITICO ARMA" }); // arma
	this->atributes_name.insert({ 0x48E5, "FORMACAO QUADRADA" });
	this->atributes_name.insert({ 0x49A8, "GOLPE DO TROVAO" });
	this->atributes_name.insert({ 0x69AD, "PURIFICACAO" });
	this->atributes_name.insert({ 0x48E7, "BURACO NEGRO" });
}

void Craft::set_created_by_name(std::string s) {
	this->created_by_name = s;
}

void debugs(std::vector<unsigned char> arr_buf) {
	std::cout << "packet size: " << std::dec << arr_buf.size() << std::endl;
	for (auto& i : arr_buf)
		std::cout << std::hex << (int)i << " ";
	std::cout << std::endl;
}

void Craft::create_spirit() {
	std::vector<unsigned char> packet = { 0x22, 0x80, 0x9D, 0x80, 0x9B,
	0x25, 0, 0x4A, 0, 0, 0, 0x91 };
	packet.resize(160);
	packet[159] = 7;

	memmove(&packet[151], &item.id,       sizeof(item.id));
	memmove(&packet[155], &item.position, sizeof(item.position));
	memmove(&packet[19],  &item.recipe,   sizeof(item.recipe));

	memmove(&packet[23], &item.sub_item_1_id, sizeof(item.sub_item_1_id));
	memmove(&packet[27], &item.sub_item_2_id, sizeof(item.sub_item_2_id));
	memmove(&packet[31], &item.sub_item_3_id, sizeof(item.sub_item_3_id));
	memmove(&packet[35], &item.sub_item_4_id, sizeof(item.sub_item_4_id));
	memmove(&packet[39], &item.sub_item_5_id, sizeof(item.sub_item_5_id));
	memmove(&packet[43], &item.sub_item_6_id, sizeof(item.sub_item_6_id));
	memmove(&packet[47], &item.sub_item_7_id, sizeof(item.sub_item_7_id));

	memmove(&packet[87],  &item.sub_item_1_pos, sizeof(item.sub_item_1_pos));
	memmove(&packet[91],  &item.sub_item_2_pos, sizeof(item.sub_item_2_pos));
	memmove(&packet[95],  &item.sub_item_3_pos, sizeof(item.sub_item_3_pos));
	memmove(&packet[99],  &item.sub_item_4_pos, sizeof(item.sub_item_4_pos));
	memmove(&packet[103], &item.sub_item_5_pos, sizeof(item.sub_item_5_pos));
	memmove(&packet[107], &item.sub_item_6_pos, sizeof(item.sub_item_6_pos));
	memmove(&packet[111], &item.sub_item_7_pos, sizeof(item.sub_item_7_pos));

	aux.crypt_send(packet);
}

void Craft::create_item(int count) {
	// alguns bytes talvez mudem de verão pra versão ou de fornalha pra fornalha..
	/*25 00 0C 00 00 00 0C 00 00 00 A0 00 00 00 D9 0B 00 00 01 00 00 00*/
	// outra versão 1.5.5
	/*25 00 0C 00 00 00 0C 00 00 00 00 00 00 00 D0 05 01 00 01 00 00 00*/
	std::vector<unsigned char> packet = { 0x22, 0x17, 0x16, 0x25, 0, 0x0C, 0, 0, 0, 0xC, 0, 0, 0, 0, 0, 0, 0,
										  0xFF, 0xFF, 0x01, 0x0, 0x01, 0, 0, 0 };
	memmove(&packet[17], &item.recipe, 2); // preenchendo 2 bytes apenas
	memmove(&packet[21], &count, 4);
	aux.crypt_send(packet);
}

void Craft::get_item_spirit() {
	std::vector<unsigned char> packet = { 0x22, 0x4, 0x3, 0x84, 0, 0 };
	aux.crypt_send(packet);
}

bool find_value(std::vector<unsigned char>& arr, unsigned int value) {
	int size = arr.size();
	for (int i = 0; i < size - 4; i++) {
		if (*(unsigned int*)&arr[i] == value)
			return true;
	}
	return false;
}

bool Craft::find_match(std::vector<unsigned int> atbs, std::vector<unsigned int> atb_count) {
	int counter  = 0;
	auto atb_    = atbs;
	for (auto& r : attributes) {
		for (size_t i = 0; i < r.size() / 2; i++) {
			for (size_t k = 0; k < atb_.size(); k++) {
				if (atb_[k] == 0)
					continue;
				if (r[i * 2] == atb_[k] && r[i * 2 + 1] <= atb_count[k]) {
					counter++;
					if (counter == r.size() / 2)
						return true;
					atb_[k] = 0;
					break;
				}
			}
		}
		counter  = 0;
		atb_     = atbs;
	}
	return false;
}

//bool Craft::find_match(std::vector<unsigned int> atbs, std::vector<unsigned int> atb_count) {
//	unsigned int counter = 0;
//	for (size_t i = 0; i < item_atrib.atb_id.size(); i++) {
//		for (size_t k = 0; k < atbs.size(); k++) {
//			if (item_atrib.atb_id[i] == atbs[k] && atb_count[k] >= item_atrib.atb_count[i] ) {
//				counter++;
//				if (counter == item_atrib.atb_id.size())
//					return true;
//				atbs[k] = atb_count[k] = 0;
//				break;
//			}
//		}
//	}
//	return false;
//}

bool Craft::scan_by_add() {
	unsigned int number_atb = this->item.number_especial_attributes;
	std::vector<unsigned int> atbs(number_atb);
	std::vector<unsigned int> atb_count(number_atb);

	if (arr_atrib.size() < number_atb * 8)
		return false;

	for (size_t i = 0; i < number_atb; i++) {
		atbs[i]       = *(unsigned int*)&arr_atrib[i * 8];
		atb_count[i]  = *(unsigned int*)&arr_atrib[4 + i * 8];
	}

	for (size_t i = 0; i < number_atb; i++) {
		if (atributes_name.count(atbs[i]))
			std::cout << std::hex << atributes_name[atbs[i]] << std::endl;
		else
			std::cout << std::hex << atbs[i] << std::endl;
		std::cout << std::dec << atb_count[i] << std::endl;
	}

	if (this->attributes_sum != -1)
		return this->attributes_sum <= std::accumulate(atb_count.begin(), atb_count.end(), 0);
	return find_match(atbs, atb_count);
}

void Craft::start_spirit_crafting() {
	// se craft_flag estiver setada, significa que uma thread já está rodando..
	create_spirit();
	if (craft_flag) return;
	this->craft_flag        = true;                   // flag que indica que tem algum craft rodando..
	this->buffer_find_flag  = false;                  // flag que indica se há buffer com atributos
	this->thread_crafting   = std::make_shared<std::thread>(&Craft::crafting_spirit, this);
	this->thread_crafting->detach();
}

void Craft::start_item_crafting() {
	//this->create_item(100);
	if (craft_flag) return;
	this->craft_flag        = true;                 
	this->buffer_find_flag  = false;      
	this->thread_crafting   = std::make_shared<std::thread>(&Craft::crafting_accessories, this);
	this->thread_crafting->detach();
}

void Craft::start_rotate_crafting() {
	basic_actions->rotate_item(item.recipe, item.position);
	if (craft_flag) return;
	this->craft_flag        = true;
	this->buffer_find_flag  = false;
	this->thread_crafting   = std::make_shared<std::thread>(&Craft::rotate_equip, this);
	this->thread_crafting->detach();
}

void Craft::recharge_materials() {
	auto list_items  = inventory->get_linear_list_item();
	bool swap_flag   = false;

	if (list_items.empty())
		return;

	for (size_t i = 0; i < items_recharge.size(); i++) {
		if (list_items[items_recharge[i].position].id == 0) {
			for (auto& k : list_items) {
				if (k.id == items_recharge[i].id && k.stack > items_recharge[i].count_to_recharge) {
					basic_actions->swap_items(k.slot_position, items_recharge[i].position);
					swap_flag = true;
					break;
				}
			}

			if (!swap_flag) {
				std::cout << "o material " << items_recharge[i].id << " acabou" << std::endl;
				return;
			}

			swap_flag = false;
		}
		else if (list_items[items_recharge[i].position].id == items_recharge[i].id) {
			if (list_items[items_recharge[i].position].stack <= items_recharge[i].count_to_recharge) {
				for (auto& k : list_items) {
					if (k.id == items_recharge[i].id && k.stack > items_recharge[i].count_to_recharge) {
						Sleep(200);
						basic_actions->swap_items(k.slot_position, items_recharge[i].position);
						swap_flag = true;
						break;
					}
				}

				if (!swap_flag) {
					std::cout << "o material " << items_recharge[i].id << " acabou" << std::endl;
					return;
				}

				swap_flag = false;
			}
			else
				continue;
		}
		else {
			for (auto& k : list_items) {
				if (k.id == items_recharge[i].id && k.stack > items_recharge[i].count_to_recharge) {
					basic_actions->swap_items(k.slot_position, items_recharge[i].position);
					swap_flag = true;
					break;
				}
			}

			if (!swap_flag) {
				std::cout << "o material " << items_recharge[i].id << " nao existe" << std::endl;
				return;
			}

			swap_flag = false;
		}	
	}
}

void Craft::crafting_spirit() {
	Timer resend_packet_timer;
	Timer rechage_materials_timer;

	// esperando o inventário atualizar, no dessa thread ter sido invocada da classe script
	Sleep(3000); 
	recharge_materials(); // reorganizo tudo primeiro

	while (craft_flag) {
		if (this->buffer_find_flag) {
			resend_packet_timer.reset();
			get_item_spirit();
			if (this->scan_by_add()) {
				Beep(1000, 1000);
				Sleep(1000);
				get_item_spirit(); // garantir que ela pegue o item
				this->craft_flag = false;
				std::cout << this->player_basic_info.second << " TIROU UM ITEM" << std::endl;
				break;
			}
			else {
				this->buffer_find_flag = false;
				Sleep(200);
				this->create_spirit();
				std::cout << "SEND FORGE PACKET ************************************" << std::endl;
			}
		}

		if (resend_packet_timer.elapsed() >= 3000) {
			resend_packet_timer.reset();
			this->create_spirit();
			std::cout << "RE_SEND PACKET *************   " << this->player_basic_info.second << std::endl;
		}

		// o time de chamada do recharge precisa ser maior que o time de atualização do inventário
		if (rechage_materials_timer.elapsed() >= inventory->get_update_time() * 2) {
			rechage_materials_timer.reset();
			recharge_materials();
		}

		Sleep(10);
	}

	this->buffer_find_flag = false;
	this->attributes.clear();
	this->item = item_craft();
}

//void Craft::crafting_accessories() {
//	Timer update_time_timer;
//	Timer resend_craft_packet;
//
//	Timer drop_coin_timer;
//
//	Sleep(5000); // desnecessário
//
//	//inventory->get_info();
//
//	//Sleep(3000);
//	item.position = inventory->get_first_empty_slot();
//
//	if (item.position == -1) {
//		std::cout << "O inventario está cheio" << std::endl;
//		this->craft_flag = false;
//	}
//
//	while (craft_flag) {
//		if (this->buffer_find_flag) {
//			resend_craft_packet.reset();
//			if (this->scan_by_add()) {
//				Beep(1000, 1000);
//				inventory->get_info();
//				Sleep(4000);
//				unsigned char new_position = inventory->get_first_empty_slot();
//				if (new_position == -1) {
//					std::cout << "O inventario esta cheio" << std::endl;
//					this->craft_flag = false;
//					continue;
//				}
//
// 				basic_actions->swap_items(item.position, new_position);
//				this->buffer_find_flag = false;
//				continue;
//			}
//			else {
//				this->buffer_find_flag = false;
//				basic_actions->sell_items(item.id, item.count, item.position, item.price);
//				Sleep(200);
//				this->create_item(1);
//				std::cout << "SEND FORGE PACKET ************************************" << std::endl;
//			}
//		}
//
//		if (update_time_timer.elapsed() >= 1000) {
//			update_time_timer.reset();
//			basic_actions->update_item(item.position);
//		}
//
//		if (drop_coin_timer.elapsed() >= 1680000) {
//			drop_coin_timer.reset();
//			basic_actions->drop_coin(10000000);
//		}
//
//		if (resend_craft_packet.elapsed() >= 3000) {
//			resend_craft_packet.reset();
//			this->create_item(1);
//		}
//
//		Sleep(10);
//	}
//
//	// limpa as variáveis
//	this->craft_flag       = false; // desnecessário?
//	this->buffer_find_flag = false;
//	this->attributes.clear();
//	this->item = item_craft();
//}

//void Craft::crafting_accessories() {
//	Sleep(5000);
//
//	std::vector<int> list_item = inventory->list_to_empty_slots();
//
//	if (list_item.empty()) {
//		std::cout << "O inventario esta vazio" << std::endl;
//		this->craft_flag = false;
//		return;
//	}
//
//	item.position = list_item.front();
//
//	Timer update_time_timer;
//	Timer resend_craft_packet;
//	Timer drop_coin_timer;
//
//	while (craft_flag) {
//		if (this->buffer_find_flag) {
//			resend_craft_packet.reset();
//			update_time_timer.reset();
//			if (this->scan_by_add()) {
//				Beep(1000, 1000);
//				list_item.erase(list_item.begin());
//				if (list_item.empty()) {
//					std::cout << "O inventario esta vazio" << std::endl;
//					this->craft_flag = false;
//					continue;
//				}
//				item.position = list_item.front();
//				this->buffer_find_flag = false;
//				continue;
//			}
//			else {
//				this->buffer_find_flag = false;
//				this->sell_find_flag   = false;
//				basic_actions->sell_items(item.id, item.count, item.position, item.price);
//				// enquanto o item não foi vendido, envia vender novamente..
//				Timer time_out_sell_timer;
//				while (this->sell_find_flag == false) {
//					Sleep(200);
//					basic_actions->sell_items(item.id, item.count, item.position, item.price);
//					if (time_out_sell_timer.elapsed() >= 1000)
//						break;
//				}
//				this->create_item(1);
//				Sleep(200);
//				basic_actions->update_item(item.position);
//				Timer time_out_create_timer;
//				// enquanto as informações sobre o item não chegaram, enviar update_item..
//				while (!buffer_find_flag) {
//					Sleep(200);
//					basic_actions->update_item(item.position);
//					// se enviou o pacote de atualização várias vezes e não obteve resposta
//					// então, por algum motivo, o item não foi criado... break e tenta criar lá fora.
//					if (time_out_create_timer.elapsed() >= 5000)
//						break;
//				}
//				std::cout << this->player_basic_info.second << " SEND FORGE PACKET **************" << std::endl;
//			}
//		}
//
//		if (update_time_timer.elapsed() >= 5000) {
//			update_time_timer.reset();
//			basic_actions->update_item(item.position);
//			std::cout << this->player_basic_info.second << " UPDATING ITEM.." << std::endl;
//		}
//
//		if (drop_coin_timer.elapsed() >= 1680000) {
//			drop_coin_timer.reset();
//			basic_actions->drop_coin(10000000);
//		}
//
//		if (resend_craft_packet.elapsed() >= 5000) {
//			resend_craft_packet.reset();
//			this->create_item(1);
//		}
//
//		Sleep(10);
//	}
//
//	// limpa as variáveis
//	this->craft_flag        = false; // desnecessário?
//	this->buffer_find_flag  = false;
//	this->sell_find_flag    = false;
//	this->attributes.clear();
//	this->item = item_craft();
//}

void Craft::crafting_accessories() {
	Sleep(60000);

	std::vector<int> list_item          = inventory->list_to_empty_slots();
	std::vector<int> list_to_old_items  = inventory->list_to_items_by_id(item.id);
	
	// junta dois vectors
	list_item.reserve(list_item.size() + list_to_old_items.size());
	list_item.insert(list_item.end(), list_to_old_items.begin(), list_to_old_items.end()); 

	sort(list_item.begin(), list_item.end());

	if (list_item.empty())
		std::cout << "O inventario esta vazio" << std::endl;

	Timer drop_coin_timer;

	std::vector<int> temp;

	while (craft_flag) {
		this->create_item(100);
		Sleep(list_item.size() * 200);

		auto iterator = list_item.begin();

		while (iterator != list_item.end()) {
			basic_actions->update_item(*iterator); // *iterator = posicão do item a ser atualizado
			Sleep(300);
			if (buffer_find_flag) {
				if (scan_by_add()) {
					buffer_find_flag = false;
					Beep(1000, 1000);
					auto iter = find(list_item.begin(), list_item.end(), item.position);
					if (iter != list_item.end()) {
						temp.push_back(*iter);
						iterator = list_item.erase(iter);
					}
					else
						iterator++;
					continue;
				}
				else {
					basic_actions->sell_items(item.id, item.count, item.position, item.price), Sleep(100);
					Sleep(100);
				}
			}
			iterator++;
			std::cout << "ITEM ANALISADO" << std::endl;
		}

		if (list_item.empty()) {
			std::cout << "O inventario esta vazio" << std::endl;
			break;
		}

		if (drop_coin_timer.elapsed() >= 5000) {
			drop_coin_timer.reset();
			basic_actions->drop_coin(100000000);
		}

		std::cout << "comecando a forjar em 3 segundos" << std::endl;
		Sleep(3000);
	}

	//while (craft_flag) {
	//	if (this->buffer_find_flag) {
	//		resend_craft_packet.reset();
	//		update_time_timer.reset();
	//		if (this->scan_by_add()) {
	//			Beep(1000, 1000);
	//			list_item.erase(list_item.begin());
	//			if (list_item.empty()) {
	//				std::cout << "O inventario esta vazio" << std::endl;
	//				this->craft_flag = false;
	//				continue;
	//			}
	//			item.position = list_item.front();
	//			this->buffer_find_flag = false;
	//			continue;
	//		}
	//		else {
	//			this->buffer_find_flag = false;
	//			this->sell_find_flag   = false;
	//			basic_actions->sell_items(item.id, item.count, item.position, item.price);
	//			Sleep(200);
	//			basic_actions->update_item(item.position);
	//		}
	//	}

	//	Sleep(10);
	//}

	// limpa as variáveis
	this->craft_flag        = false; // desnecessário?
	this->buffer_find_flag  = false;
	this->sell_find_flag    = false;
	this->attributes.clear();
	this->item = item_craft();
}

void Craft::rotate_equip() {
	Timer resend_rotate_packet;
	Timer time_elapsed;

	while (craft_flag) {
		if (this->buffer_find_flag) {
			// start = steady_clock::now();
			resend_rotate_packet.reset();
			if (this->scan_by_add()) {
				Beep(1000, 1000);
				this->craft_flag = false;
				std::cout << this->player_basic_info.second << " TIROU UM ITEM" << std::endl;
				break;
			}
			else {
				this->buffer_find_flag = false;
				Sleep(200);
				basic_actions->rotate_item(item.recipe, item.position);

				std::cout << "TEMPO DECORRIDO: " << time_elapsed.elapsed() << std::endl;
				time_elapsed.reset();
			}
		}

		if (resend_rotate_packet.elapsed() >= 5000) {
			resend_rotate_packet.reset();
			basic_actions->rotate_item(item.recipe, item.position);
			std::cout << "PACOTE REENVIADO ************************************" << std::endl;
		}

		Sleep(10);
	}

	this->buffer_find_flag = false;
	this->attributes.clear();
	this->attributes_sum   = -1;
	this->set_created_by_name(player_basic_info.second);
	this->item = item_craft();
}

void Craft::scan_packet(std::vector<unsigned char>& arr_buf) {
	if (!this->craft_flag) return;
	find_pattern(arr_buf);
}

void Craft::set_player_info(std::pair<unsigned int, std::string> player_basic_info) {
	this->player_basic_info  = player_basic_info;
	this->set_created_by_name(player_basic_info.second);
}

void Craft::set_enviroment(std::shared_ptr<Inventory> inventory_arg, 
	std::shared_ptr<Basic_actions> actions_arg) {
	this->inventory      = inventory_arg;
	this->basic_actions  = actions_arg;
	//this->message        = message_arg;
}

void Craft::find_pattern(std::vector<unsigned char>& arr_buf) {
	int size = arr_buf.size();

	if (size < 10) return;

	std::vector<unsigned char> name_match;

	for (size_t i = 0; i < this->created_by_name.size(); i++) {
		name_match.push_back(this->created_by_name[i]);
		name_match.push_back(0);
	}

	// BUSCA PELA ASSINATURA DO ITEM VENDIDO

	int sell_item_packet = 0xB5050622;

	if (this->sell_find_flag == false) {
		for (int i = 0; i < size - 26; i++) {
			if (*(unsigned int*)(&arr_buf[i]) == sell_item_packet) {
				if (arr_buf[i + 6] == item.position) {
					this->sell_find_flag = true;
					break;
				}
			}
		}
	}

	// -------------------------------------

	int counter = 0;
	for (int i = 0; i < size - 30; i++) {
		if (*(unsigned int*)(&arr_buf[i]) == item.id) {
			if (arr_buf[i+20] != item.level)
				continue;
			//if (i > 0)
			//	item.position = arr_buf[i - 1];
			for (int k = i; k < size; k++) { 
				if (name_match[counter] == arr_buf[k]) {
					counter++;
					if (counter == name_match.size()) {
						for (int j = k; j < size-4; j++) {
							if (*(unsigned int*)(&arr_buf[j]) == item.number_especial_attributes && arr_buf[j+4] > 0) {
								if (j-k > 100) return;

								item.position = arr_buf[i - 1];

								auto iter = find(special_atributes.begin(),
									             special_atributes.end(), *(unsigned int*)&arr_buf[j + 4]);

								arr_atrib = std::vector<unsigned char>(arr_buf.begin() + j + 4, arr_buf.end());

								if (iter != special_atributes.end())
									arr_atrib.erase(arr_atrib.begin() + 8, arr_atrib.begin() + 12);

								this->buffer_find_flag = true;
								return;
							}
						}
						return;
					}
				}
				else
					counter = 0;
			}
			return;
		}
	}
	return;
}

//start + 146 = item_id;
//start + 150 = main_item_position;
//start + 14 = recipe
//
//start + 18 = 1_sub_item
//start + 22 = 2_sub_item
//start + 26 = 3_sub_item
//start + 30 = 4_sub_item
//start + 34 = 5_sub_item
//start + 38 = 6_sub_item
//start + 42 = 7_sub_item
//
//start + 82 = 1_sub_item_position;
//start + 86 = 2_sub_item_position;
//start + 90 = 3_sub_item_position;
//start + 94 = 4_sub_item_position;
//start + 98 = 5_sub_item_position;
//start + 102 = 6_sub_item_position;
//start + 106 = 7_sub_item_position;