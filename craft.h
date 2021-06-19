#pragma once
#include <chrono>
#include "Aux_function.h"
#include "Inventory.h"
#include "Basic_actions.h"

// essa estrutura é utilizada por toda função de forja e reforja, mas nem todos os campos são essenciais às funções de forja e reforja
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

// struct relacionada às ações de recharge
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
	// essa função funciona apenas para match com o pacote referente ao espírito da forja
	void  scan_packet(std::vector<unsigned char>& arr_buf);
	void  set_player_info(std::pair<unsigned int, std::string> player_basic_info);
	void  set_enviroment(std::shared_ptr<Inventory> inventory_arg, std::shared_ptr<Basic_actions> actions_arg);
	void  find_pattern(std::vector<unsigned char>& arr_buf);
	void  set_created_by_name(std::string s);

	item_craft                              item;
	std::vector<std::vector<unsigned int>>  attributes;

	// isso só é diferente de -1 quando a função de roletar estiver sendo usada, do contrário sempre será -1
	int                                     attributes_sum;  

	// variável vinculada com a função recharge_materials
	std::vector<item_recharge>              items_recharge; 
	
	// 
	bool  craft_flag;

	// essa lista de atributos especiais serve pra tratar um border-case relacionado
	// à variação do tamanho pacote do item quando algum desses atributos aparece..
	std::vector<int> special_atributes = { 0x48E3, 0x48E5, 0x49A8, 0x69AD, 0x48E7 };

	//48E3 = deus do frenesi
	//48E5 = formação quadrada
	//49A8 = golpe do trovão
	//69AD = purificação
	//48E7 = buraco negro

private:
	bool  find_match(std::vector<unsigned int> atbs, std::vector<unsigned int> atb_count);
	void  crafting_spirit();
	void  crafting_accessories();
	void  rotate_equip();
	void  create_item(int count);      // forja item na fornalha, utiliza item_craft, mas não a estrutura inteira
	bool  scan_by_add(); 
	void  get_item_spirit();  // pacote enviado pro espírito da forja pra pegar o item com novo atributo
	void  create_spirit();    // pacote enviado pra forjar o item no espírito da forja, utiliza item_craft 
	void  set_atributes_name();

	// essa função depende da atualização do inventário
	void  recharge_materials();

	//std::shared_ptr<Message>              message;
	std::shared_ptr<std::thread>          thread_crafting;
	std::shared_ptr<Inventory>            inventory;
	std::shared_ptr<Basic_actions>        basic_actions;
	Aux_function&                         aux;
	std::pair<unsigned int, std::string>  player_basic_info;

	// tree contendo o id associado ao nome do item
	std::map<unsigned int,  std::string>  atributes_name;
	// variável utilizada pra buscar por padrão no pacote de forja dos itens
	// essa variável só deve ser alterada quando a função de roletar itens for utilizada
	std::string                           created_by_name;

	// flag setada quando arr_atrib está com os pacotes que contém os atributos
	bool                                  buffer_find_flag;
	// flag setada quando algum item for vendido durante o funcionamento de algum craft
	bool                                  sell_find_flag;
	// essa variável conterá o pacote referente aos atributos do item e afins
	std::vector<unsigned char>            arr_atrib;
};

