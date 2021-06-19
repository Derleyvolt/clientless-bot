#pragma once
#include "Aux_function.h"

class General {
public:
	General(Aux_function& aux_arg);

	void  scan_packet(std::vector<unsigned char>& arr);
	void  set_player_info(std::pair<unsigned int, std::string> player_basic_info);

	unsigned int  get_chest_id() const;
	void          clear_chest_id();

	void  chest_functions(std::vector<unsigned char>& arr);
	void  set_update_flag(bool f);
	bool  get_update_flag() const;

private:
	Aux_function& aux;

	void captured_items(std::vector<unsigned char>& arr); // items pêgos 
	void get_chest_quest(std::vector<unsigned char>& arr);

	unsigned int chest_quest_id;
	bool         update_flag;

	std::pair<unsigned int, std::string> player_basic_info;
};

