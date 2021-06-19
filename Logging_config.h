#pragma once
#include <string>
#include <set>

struct Logging_config {
	std::string   login;
	std::string   senha;
	std::string   player_name;
	std::string   script;
	unsigned int  fly_id  = 0;
	bool          force   = 1;

	// flags que servem pra habilitar a utilização das seguintes classes

	bool items      = 1;
	bool inventory  = 1;
	bool healing    = 0;
	bool trade      = 1;
	bool movement   = 1;
	bool party      = 1;

	std::set<unsigned int> party_list_permission;
	std::set<unsigned int> trade_list_permission;
	// quem está nessa lista tbm precisa estar nas outras listas? verei dps..
	std::set<unsigned int> commands_list_permission;
};

