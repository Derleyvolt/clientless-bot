// Emulator.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "MD5Hash.h"
#include "aux_function.h"
#include "player_structure.h"
#include "Logging_config.h"
#include <fstream>
#include <regex>

#pragma comment(lib,"ws2_32.lib")

bool init_wsadata() {
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0) {
		puts("Winsock falhou");
		return false;
	}

	return true;
}

char my_server[]  = "192.168.1.73";
char the_pw[]     = "177.54.157.91";
char central[]    = "177.54.148.246";
char pw_brasil[]  = "189.1.170.142";

//#define PW_PORT 29002
#define PW_PORT 29000

void start(Player_structure* p, Logging_config* lc) {
	if (p->config_socket(pw_brasil, PW_PORT)) {
		p->set_credentials(*lc);
		p->connect();
	}
}

std::vector<Logging_config> extract_boot_data() {
	std::string   line;
	std::ifstream myfile("config_bot.txt");

	std::regex   simple_regex("=\\s+(.+)");
	std::smatch  s;

	std::vector<std::string> data;

	if (myfile.is_open()) {
		while (std::getline(myfile, line)) {
			if (std::regex_search(line, s, simple_regex)) {
				//std::cout << s[1] << std::endl;
				data.push_back(s[1]);
			}
		}
		myfile.close();
	}
	else {
		std::cout << "Nao foi possivel abrir o arquivo";
		exit(1);
	}

	// as configurações conferem 15 linhas
	if (data.empty() || data.size() % 15 != 0) {
		std::cout << "Existe algo de errado com o arquivo de dados.. talvez a formatacao" << std::endl;
		exit(1);
	}

	unsigned int number_of_char = data.size() / 15;

	std::vector<Logging_config> arr_data;

	arr_data.resize(number_of_char);

	for (size_t i = 0; i < number_of_char; i++) {
		arr_data[i].login        = data[0];
		arr_data[i].senha        = data[1];
		arr_data[i].player_name  = data[2];
		arr_data[i].script       = data[3];
		arr_data[i].force        = std::stoi(data[4]);
		arr_data[i].fly_id       = std::stoi(data[5]);

		arr_data[i].items        = data[6].front()  - 48;
		arr_data[i].inventory    = data[7].front()  - 48;
		arr_data[i].healing      = data[8].front()  - 48;
		arr_data[i].trade        = data[9].front()  - 48;
		arr_data[i].party        = data[10].front() - 48;
		arr_data[i].movement     = data[11].front() - 48;

		std::regex get_number("\\d+");

		auto parser = std::sregex_iterator(data[12].begin(), data[12].end(), get_number);
		std::sregex_iterator rend;

		while (parser != rend) {
			arr_data[i].party_list_permission.insert(std::stoi(parser->str()));
			++parser;
		}

		parser = std::sregex_iterator(data[13].begin(), data[13].end(), get_number);

		while (parser != rend) {
			arr_data[i].trade_list_permission.insert(std::stoi(parser->str()));
			++parser;
		}

		parser = std::sregex_iterator(data[14].begin(), data[14].end(), get_number);

		while (parser != rend) {
			arr_data[i].commands_list_permission.insert(std::stoi(parser->str()));
			++parser;
		}

		data.erase(data.begin(), data.begin() + 15);
	}

	return arr_data;
}

int main() {
	if (!init_wsadata())
		return 0;

	auto account_data = extract_boot_data();

	Player_structure* player_object  = new Player_structure[account_data.size()];
	Logging_config*   logging        = new Logging_config[account_data.size()];

	std::vector<std::thread> arr_thread;

	for (size_t i = 0; i < account_data.size(); i++) {
		system("cls");
		//std::cout << "Login: ";
		////std::cin >> logging[i].login;
		//logging->login = "rato0"; //
		//std::cout << std::endl;
		//std::cout << "Senha: ";
		////std::cin >> logging[i].senha;
		//logging->senha = "derleyvolt";
		//
		//logging[i].player_name = "rato_sujo"; // rato00 id = 983408 .. rato0 id = 982656
		
		logging[i] = account_data[i];

		arr_thread.push_back(std::thread(start, &player_object[i], &logging[i]));
		Sleep(500);
	}

	//for (auto& k : arr_thread)
	//	k.join();

	Sleep(8888888888);

	delete[] player_object;

	system("pause");
	return 0;
}