#pragma once
#include <chrono>
#include "Aux_function.h"
#include "Inventory.h"
#include "Basic_actions.h"

// essa estrutura � utilizada por toda fun��o de forja e reforja, mas nem todos os campos s�o essenciais �s fun��es de forja e reforja
struct item_craft {
	unsigned int id        = 0;
	unsigned int position  = 0;
	unsigned int recipe    = 0;
	unsigned int price     = 0;
	unsigned int count     = 0;
	unsigned int level     = 0;

	unsigned int sub_item_1_id  = 0;
	unsigned int sub_item_2_id  = 0;
	unsigned int sub_item_3_id  = 0;
	unsigned int sub_item_4_id  = 0;
	unsigned int sub_item_5_id  = 0;
	unsigned int sub_item_6_id  = 0;
	unsigned int sub_item_7_id  = 0;

	unsigned int sub_item_1_pos = 0;
	unsigned int sub_item_2_pos = 0;
	unsigned int sub_item_3_pos = 0;
	unsigned int sub_item_4_pos = 0;
	unsigned int sub_item_5_pos = 0;
	unsigned int sub_item_6_pos = 0;
	unsigned int sub_item_7_pos = 0;

	unsigned char  number_especial_attributes = 0;
};

struct atrib {
	std::vector<int> atb_id;
	std::vector<int> atb_count;
};

// struct relacionada �s a��es de recharge
struct item_recharge {
	unsigned int id;
	unsigned int position;
	unsigned int count_to_recharge;
};

class Craft {
public:
	Craft(Aux_function& arg_aux);
	void  start_spirit_crafting();
	void  start_item_crafting();
	void  start_rotate_crafting();
	// essa fun��o funciona apenas para match com o pacote referente ao esp�rito da forja
	void  scan_packet(std::vector<unsigned char>& arr_buf);
	void  set_player_info(std::pair<unsigned int, std::string> player_basic_info);
	void  set_enviroment(std::shared_ptr<Inventory> inventory_arg, std::shared_ptr<Basic_actions> actions_arg);
	void  find_pattern(std::vector<unsigned char>& arr_buf);
	void  set_created_by_name(std::string s);

	item_craft                              item;
	std::vector<std::vector<unsigned int>>  attributes;

	// isso s� � diferente de -1 quando a fun��o de roletar estiver sendo usada, do contr�rio sempre ser� -1
	int                                     attributes_sum;  

	// vari�vel vinculada com a fun��o recharge_materials
	std::vector<item_recharge>              items_recharge; 
	
	// 
	bool  craft_flag;

	// essa lista de atributos especiais serve pra tratar um border-case relacionado
	// � varia��o do tamanho pacote do item quando algum desses atributos aparece..
	std::vector<int> special_atributes = { 0x48E3, 0x48E5, 0x49A8, 0x69AD, 0x48E7 };

	//48E3 = deus do frenesi
	//48E5 = forma��o quadrada
	//49A8 = golpe do trov�o
	//69AD = purifica��o
	//48E7 = buraco negro

private:
	bool  find_match(std::vector<unsigned int> atbs, std::vector<unsigned int> atb_count);
	void  crafting_spirit();
	void  crafting_accessories();
	void  rotate_equip();
	void  create_item(int count);      // forja item na fornalha, utiliza item_craft, mas n�o a estrutura inteira
	bool  scan_by_add(); 
	void  get_item_spirit();  // pacote enviado pro esp�rito da forja pra pegar o item com novo atributo
	void  create_spirit();    // pacote enviado pra forjar o item no esp�rito da forja, utiliza item_craft 
	void  set_atributes_name();

	// essa fun��o depende da atualiza��o do invent�rio
	void  recharge_materials();

	//std::shared_ptr<Message>              message;
	std::shared_ptr<std::thread>          thread_crafting;
	std::shared_ptr<Inventory>            inventory;
	std::shared_ptr<Basic_actions>        basic_actions;
	Aux_function&                         aux;
	std::pair<unsigned int, std::string>  player_basic_info;

	// tree contendo o id associado ao nome do item
	std::map<unsigned int,  std::string>  atributes_name;
	// vari�vel utilizada pra buscar por padr�o no pacote de forja dos itens
	// essa vari�vel s� deve ser alterada quando a fun��o de roletar itens for utilizada
	std::string                           created_by_name;

	// flag setada quando arr_atrib est� com os pacotes que cont�m os atributos
	bool                                  buffer_find_flag;
	// flag setada quando algum item for vendido durante o funcionamento de algum craft
	bool                                  sell_find_flag;
	// essa vari�vel conter� o pacote referente aos atributos do item e afins
	std::vector<unsigned char>            arr_atrib;
};

