#pragma once
#include "Aux_function.h"
#include "Movement.h"

/*

	Baú da fortuna                   = 0x623C
	Coral                            = ox6B50
	Pacote misterioso                = 0x7A8B
	Perg. Tigre lendário             = 0x6CF4
	Pacote do sol nascente           = ox6E7A
	Pacote da vida selvagem          = 0x7D5A
	Pacote cósmico                   = 0x815B
	Pacote de comemoração universal  = 0x8173
	Pergaminho da liberdade          = 0x7964

*/

class Basic_actions {
public:
	Basic_actions(Aux_function& arg_aux);

	void  set_enviroment(std::shared_ptr<Movement> movement_arg);

	void  select_npc(unsigned int unique_id);
	void  open_npc(unsigned int unique_id);
	void  equip_item(unsigned char type, unsigned char pos);
	void  mount(unsigned char pos);
	void  desmount();
	void  drop_coin(unsigned int coin);
	void  swap_items(unsigned char item_pos1, unsigned char item_pos2);
	void  sell_items(unsigned int item_id, unsigned int count, unsigned char pos, 
		             unsigned int price);
	void  send_basic_skills(unsigned int id, unsigned int target_id);
	void  send_basic_skills_ex(unsigned int id, unsigned int target_id);
	void  update_item(unsigned char position);
	void  normal_attack();
	void  use_item(unsigned int id, unsigned char position);
	void  rotate_item(unsigned int id, unsigned char position);
	void  drop_items(unsigned char position, unsigned int count);
	void  send_request_chest_item(unsigned short chest_quest_id);
	void  accept_recall(unsigned int player_id);

private:
	//friend class Movement;

	Aux_function& aux;
	std::shared_ptr<Movement> movement;
	//std::shared_ptr<Craft> dadsa;
};