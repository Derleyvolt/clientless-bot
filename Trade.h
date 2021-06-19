#pragma once
#include "Aux_function.h"
#include <set>

class Trade {
public:
	Trade(Aux_function& arg_aux);

	void  scan_packet(std::vector<unsigned char>& decode_data);
	void  set_player_info(std::pair<unsigned int, std::string> player_basic_info);
	void  set_update_flag(bool f);
	bool  get_update_flag() const;
	void  add_permission(unsigned int player_id);

	std::set<unsigned int>& get_permission_list();

private:
	Aux_function& aux;
	std::pair<unsigned int, std::string> player_basic_info;
	std::set<unsigned int> permission;

	bool update_flag;
};