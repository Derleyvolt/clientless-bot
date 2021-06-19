#include "stdafx.h"
#include <chrono>
#include "Player_structure.h"

std::map<unsigned int, std::wstring> items_name;

void Player_structure::send_request() {
	int recv_length = 0;

	while (true) {
		memset(recv_buf, 0, recv_length);

		if ((recv_length = recv(socket_descriptor, (char*)recv_buf, sizeof(recv_buf), NULL)) < 1) {
			std::cout << "Algum erro ocorreu" << std::endl;
			return;
		}

		dec_arr(recv_buf, recv_length);

		switch (recv_buf[0]) {
			case 0x1: {
				list packet(64);
				unsigned char key_1[16];
				memcpy(key_1, recv_buf + 3, recv_buf[2]);
				list hash_1(16);
				md5.hash_logging(credentials.login, credentials.senha, key_1, sizeof(key_1), hash_1);
				packet.resize(aux.serialize(packet, credentials.login, hash_1));

				dec_vector(packet);

				if (send(socket_descriptor, (char*)packet.data(), packet.size(), NULL) != packet.size())
					return;
				break;
			}

			case 0x2: {
				unsigned char key_2[16];
				unsigned char key_3[16];

				list random_hash(20);
				random_hash[0] = 0x2;
				random_hash[1] = 0x12;
				random_hash[2] = 0x10;

				memcpy(key_2, recv_buf + 3, recv_buf[2]);

				for (auto& i : key_3) i = rand() % 256;

				list hash_2(16);
				md5.hash(key_2, sizeof(key_2), hash_2);

				list hash_3(16);
				md5.hash(key_3, sizeof(key_3), hash_3);

				aux.crypt.CreateCrypt(hash_2);   
				aux.crypt.CreateDecrypt(hash_3);

				std::copy(key_3, key_3 + 16, random_hash.begin() + 3);

				dec_vector(random_hash);

				aux.crypt.Encode(random_hash);

				// random_hash é 20
				if (send(socket_descriptor, (char*)random_hash.data(), random_hash.size(), NULL) == random_hash.size())
					enter_world();
				else
					std::cout << "Ocorreu um erro e o personagem não conseguiu entrar no mundo..." << std::endl;
				return;
			}

			case 0x5: {
				std::cout << "Login ou senha incorreto!" << std::endl;
				return;
			}
		}
	}
}

void Player_structure::recv_thread() {
	int recv_length;
	std::vector<unsigned char> decode_data;

	while (true) {
		if (aux.recv_complete(recv_buf, recv_length) == 1)
			return;

		decode_data.resize(recv_length);
		std::copy(recv_buf, recv_buf + recv_length, decode_data.begin());
		mt.lock();
		data_queue.push(decode_data);
		mt.unlock();
		Sleep(1);
	}
}

void Player_structure::keep_alive() {
	while (true) {
		list packet = { 0x5A, 0x1, 0x5A };
		Sleep(60000);
		aux.crypt_send(packet);
	}
}

void Player_structure::connect() {
	send_request();
}

bool Player_structure::config_socket(const char ip_address[], unsigned short port) {
	SOCKADDR_IN  m_Addr;

	memset(&m_Addr, 0, sizeof(m_Addr));

	m_Addr.sin_addr.s_addr  = inet_addr(ip_address);
	m_Addr.sin_port         = htons(port);
	m_Addr.sin_family       = AF_INET;
	socket_descriptor       = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (::connect(socket_descriptor, (SOCKADDR*)&m_Addr, sizeof(m_Addr)) != 0) {
		puts("Erro ao se conectar");
		return 0;
	}

	aux.set_descriptor(socket_descriptor);

	items          = std::make_shared<Items>(aux);
	trade          = std::make_shared<Trade>(aux);
	party          = std::make_shared<Party>(aux);
	craft          = std::make_shared<Craft>(aux);
	script         = std::make_shared<Script>(aux);
	message        = std::make_shared<Message>(aux);
	general        = std::make_shared<General>(aux);
	healing        = std::make_shared<Healing>(aux);
	movement       = std::make_shared<Movement>(aux);
	inventory      = std::make_shared<Inventory>(aux);
	basic_actions  = std::make_shared<Basic_actions>(aux);

	items->set_enviroment(movement);
	basic_actions->set_enviroment(movement);
	craft->set_enviroment(inventory,   basic_actions);
	script->set_enviroment(inventory,  basic_actions, items, movement, craft, message, general);
	message->set_enviroment(inventory, basic_actions, movement, items, craft, party, healing, trade);
	inventory->set_enviroment(basic_actions);
	return 1;
}

void Player_structure::set_credentials(Logging_config arg_credentials) {
	credentials = arg_credentials;

	inventory->set_update_flag(credentials.inventory);
	items->set_update_flag(credentials.items);
	movement->set_update_flag(credentials.movement);
	trade->set_update_flag(credentials.trade);
	healing->set_update_flag(credentials.healing);
	party->set_update_flag(credentials.party);

	movement->set_fly(credentials.fly_id);

	party->get_permission_list() = credentials.party_list_permission;
	trade->get_permission_list() = credentials.trade_list_permission;
	message->get_permission_list() = credentials.commands_list_permission;
}

void debug(std::vector<unsigned char> arr_buf) {
	std::cout << "packet size: " << std::hex << arr_buf.size() - 4 << std::endl;
	for (auto& i : arr_buf)
		std::cout << std::hex << (int)i << " ";
	std::cout << std::endl;
}

void Player_structure::enter_world() {
	memset(recv_buf, 0, sizeof(recv_buf));
	int recv_length = 0;

	if (aux.recv_complete(recv_buf, recv_length) == 1)
		return;

	list decode_data(recv_length);
	std::copy(recv_buf, recv_buf + recv_length, decode_data.begin());

	aux.decrypt_unpack(decode_data, NULL, nullptr);

	list select_char(14), select_char_send(14);
	select_char[0] = 0x52;
	select_char[1] = 0xC;

	list account_id(decode_data.begin() + 2, decode_data.begin() + 6);

	std::fill(select_char.begin() + 10, select_char.end(), 0xFF);
	std::copy(account_id.begin(), account_id.end(), select_char.begin() + 2);

	select_char_send = select_char;
	aux.crypt_send(select_char_send);

	if (aux.recv_complete(recv_buf, recv_length) == 1)
		return;
	aux.decrypt_unpack(decode_data, recv_length, recv_buf);

	memset(recv_buf, 0, recv_length);
	recv_length = recv(socket_descriptor, (char*)recv_buf, sizeof(recv_buf), NULL);

	if (recv_length > 0)
		aux.decrypt_unpack(decode_data, recv_length, recv_buf);

	std::map<std::string, unsigned int> list_player_basic_info;

	std::cout << std::endl;

	while (decode_data[1] != 0x11) {
		if (decode_data[0] == 0x53)
			aux.get_player_basic_info(decode_data, list_player_basic_info);

		select_char_send = select_char;
		aux.crypt_send(select_char_send);

		if (aux.recv_complete(recv_buf, recv_length) == 1)
			return;

		aux.decrypt_unpack(decode_data, recv_length, recv_buf);
	}

	std::cout << std::endl;

	player_basic_info.second = credentials.player_name;

	if (credentials.player_name.empty()) {
		std::cout << "Digite o nick do personagem que vc quer logar: ";
		std::cin >> player_basic_info.second;
	}

	auto player_info = list_player_basic_info.find(player_basic_info.second);

	if (player_info == list_player_basic_info.end()) {
		std::cout << "Algum erro ocorreu ou esse personagem não existe na sua conta." << std::endl;
		return;
	}

	// só pra manter compatibilidade.. dos ajeito isso
	player_basic_info.second = player_info->first;
	player_basic_info.first  = player_info->second;

	list character_id = aux.reverse_bytes(player_basic_info.first);

	list choice_character(7);
	list enter_world(26);
	choice_character[0]  = 0x46;
	choice_character[1]  = 0x05;
	enter_world[0]       = 0x48;
	enter_world[1]       = 0x18;

	std::copy(character_id.begin(), character_id.end(), choice_character.begin() + 2);
	std::copy(character_id.begin(), character_id.end(), enter_world.begin() + 2);

	aux.crypt_send(choice_character);
	if (aux.recv_complete(recv_buf, recv_length) == 1)
		return;
	aux.decrypt_unpack(decode_data, recv_length, recv_buf);

	message->set_player_info(player_basic_info);
	trade->set_player_info(player_basic_info);
	craft->set_player_info(player_basic_info);
	general->set_player_info(player_basic_info);

	aux.crypt_send(enter_world);

	Sleep(500); //

	if (aux.recv_complete(recv_buf, recv_length) == 1) 
		return;
	aux.decrypt_unpack(decode_data, recv_length, recv_buf);

	movement->set_player_info(decode_data, this->player_basic_info.first);
	items->get_initial_objects(decode_data);
	inventory->start_update();
	script->set_player_info(this->player_basic_info, credentials);

	std::thread recv_thread(&Player_structure::recv_thread, this);
	std::thread keep_alive(&Player_structure::keep_alive,   this);
	recv_thread.detach();
	keep_alive.detach();

	system("cls");

	//script->start(); // inicia os scritps aqui... cuidado com os scritps que dependem da atualização do inventário..

	std::vector<unsigned char> arr_buf;
	while (true) {
		if (!data_queue.empty()) {
			aux.decrypt_unpack(arr_buf, data_queue.front().size(), data_queue.front().data());

			//debug(arr_buf);

			healing->scan_packet(arr_buf);
			party->scan_packet(arr_buf);
			trade->scan_packet(arr_buf);
			message->scan_packet(arr_buf);
			general->scan_packet(arr_buf);
			movement->scan_packet(arr_buf);
			items->scan_packet(arr_buf);
			inventory->scan_packet(arr_buf);
			craft->scan_packet(arr_buf);

			mt.lock();
			data_queue.pop();
			mt.unlock();
		}

		Sleep(1);
	}
}


/*
  commands

  kill all mobs in field: d 10855 10855
  create item: d 10800 id count
  add 1 chi: d 1992
  let me have 399 chi: d 1991 399
  get Your Level*1,000,000 coins in inventory: d 1988	
*/