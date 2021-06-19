#pragma once
#include <iostream>
#include <memory>
#include <set>
#include "Inventory.h"
#include "Aux_function.h"
#include "Basic_actions.h"
#include "Movement.h"
#include "Items.h"
#include "craft.h"
#include "Party.h"
#include "Healing.h"
#include "Trade.h"

class Message {
public:
	Message(Aux_function& arg_aux);

	void  scan_packet(std::vector<unsigned char>& arr);
	void  send_private_message(unsigned int target_id, std::wstring message, bool is_friend = false);
	void  send_party_message(std::wstring message);
	void  set_enviroment(std::shared_ptr<Inventory> inventory_arg,  std::shared_ptr<Basic_actions> basic_actions_arg, 
		                 std::shared_ptr<Movement>  movement_arg,   std::shared_ptr<Items> items_arg, 
		                 std::shared_ptr<Craft>     craft_arg,      std::shared_ptr<Party> party_arg,
		                 std::shared_ptr<Healing>   healing_arg,    std::shared_ptr<Trade> trade_arg);
	void  set_player_info(std::pair<unsigned int, std::string> player_basic_info);
	void  add_permission(unsigned int player_id);

	std::set<unsigned int>& get_permission_list();

private:
    std::set<unsigned int>                permission;


	std::shared_ptr<Inventory>            inventory;
	std::shared_ptr<Basic_actions>        basic_actions;
	std::shared_ptr<Movement>             movement;
	std::shared_ptr<Healing>              healing;
	std::shared_ptr<Items>                items;
	std::shared_ptr<Craft>                craft;
	std::shared_ptr<Party>                party;
	std::shared_ptr<Trade>                trade;
	std::pair<unsigned int, std::string>  player_basic_info;

	Aux_function& aux;
	void  execute_message(std::string message_arg, unsigned char type, unsigned int player_id);
};

