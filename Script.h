#pragma once
#include "Aux_function.h"
#include "Inventory.h"
#include "Basic_actions.h"
#include "Items.h"
#include "Movement.h"
#include "craft.h"
#include "Message.h"
#include "general.h"
#include "Logging_config.h"
#include <array>

// R8 id dos atributos
// nivel def principal  = 2AA4
// nivel def secundário = 2AA3
// adaga recipe = 5580
// id adaga = 32270

class Script {
public:
	Script(Aux_function& arg_aux);

	void start(); // essa função é chamada assim que o personagem nasce no mundo

	void  opening_chest();

	void  meus_buffs();

	void  forjar_item_no_espirito();
	void  forjar_item_fornalha();
	void  roletar_item();
	void  peger_items();

	void  set_player_info(std::pair<unsigned int, std::string> player_basic_info, 
		Logging_config log_config);
	void  set_enviroment(std::shared_ptr<Inventory>      inventory_arg,
		                 std::shared_ptr<Basic_actions>  actions_arg,
		                 std::shared_ptr<Items>          items_arg, 
		                 std::shared_ptr<Movement>       movement_arg, 
		                 std::shared_ptr<Craft>          craft_arg,
		                 std::shared_ptr<Message>        message_arg,
		                 std::shared_ptr<General>        general_arg);

private:
	std::shared_ptr<Items>                items;
	std::shared_ptr<Movement>             movement;
	std::shared_ptr<Inventory>            inventory;
	std::shared_ptr<Basic_actions>        basic_actions;
	std::shared_ptr<Craft>                craft;
	std::shared_ptr<Message>              message;
	std::shared_ptr<General>              general;
	Logging_config                        config;

	Aux_function&                         aux;
	std::pair<unsigned int, std::string>  player_basic_info;
};

