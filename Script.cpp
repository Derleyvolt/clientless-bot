#include "stdafx.h"
#include "Script.h"

Script::Script(Aux_function& arg_aux) : aux(arg_aux) {

}

void Script::start() {
	// roletar_item();
	//forjar_item_no_espirito();
	//std::thread abrir_bau(&Script::opening_chest, this);
	//abrir_bau.detach();
	//std::thread abrir_bau(&Script::peger_items, this);
	//abrir_bau.detach();
	switch (basic_hash(this->config.script.c_str())) {
	case basic_hash("bau"):
	{
		std::thread abrir_bau(&Script::opening_chest, this);
		abrir_bau.detach();
		break;
	}
	case basic_hash("roletar"):
	{
		roletar_item();
		break;
	}
	case basic_hash("forjar_r8"):
	{
		forjar_item_no_espirito();
		break;
	}
	case basic_hash("forjar"):
	{
		forjar_item_fornalha();
		break;
	}
	case basic_hash("buffs"):
	{
		std::thread abrir_bau(&Script::meus_buffs, this);
		abrir_bau.detach();
		break;
	}
	case basic_hash("pegar_item"):
	{
		std::thread get_items(&Script::peger_items, this);
		get_items.detach();
		break;
	}

	default:
		break;
	}
}

void Script::forjar_item_no_espirito() {
	basic_actions->open_npc(-2146434412);
	basic_actions->select_npc(-2146434412);
	Sleep(100);

	basic_actions->send_basic_skills_ex(0x4F9, this->player_basic_info.first);

	craft->item.id             = 0x7E0A;   // 32277 peito merc
	craft->item.position       = 0;
	craft->item.recipe         = 0x15C8;  // 5513 machado branco
	craft->item.level          = 100;     // necessário, pois é usado como pattern na busca do pacote de forja e reforja
	craft->item.sub_item_1_id  = 11208;
	craft->item.sub_item_1_pos = 1;

	// essa informação é necessária, pois é usada pra encontrar os atributos do item
	craft->item.number_especial_attributes = 4;

	// atributos
	craft->attributes.push_back({ 0x2AA4, 0, 0x2AA3, 0, 0x2AA3, 0, 0x2AA3, 0 });

	// busca por esses itens no invetário caso eles cheguem a quantidade menor ou igual a 0
	// e os poe nas respectivas posições
	craft->items_recharge.push_back({ 0x7E0A, 0, 0 });
	craft->items_recharge.push_back({ 11208, 1, 0 });

	craft->start_spirit_crafting();
}

void Script::forjar_item_fornalha() {
	basic_actions->open_npc(-2146399292);
	basic_actions->select_npc(-2146399292);
	Sleep(100);

	basic_actions->send_basic_skills(0x4F9, this->player_basic_info.first);

	craft->item.id      = 32277;
	craft->item.recipe  = 1434;
	craft->item.count   = 1;
	craft->item.price   = 30000;
	craft->item.level   = 100;

	craft->attributes.push_back({ 0x2ABC, 0, 0x2ABC, 0 });

	craft->item.number_especial_attributes = 3;

	inventory->set_update_time(60000 * 5); // altera a atualização do inventário pra 5 minutos

	craft->start_item_crafting();
}

void Script::roletar_item() {
	basic_actions->open_npc(-2146397772);
	Sleep(100);
	basic_actions->send_basic_skills_ex(0x4F9, this->player_basic_info.first);

	craft->item.id        = 0x7E0A; // 20393 id anel
	craft->item.position  = 0;
	craft->item.level     = 100;
	craft->item.recipe    = 0x135D;

	// soma dos atributos
	craft->attributes_sum = 37;

	// a função craft->set_created_by_name deve ser setada com o nome do player que forjou o item caso
	// o item não tenha sido forjado pelo próprio personagem

	//craft->set_created_by_name("rato_sujo");

	craft->item.number_especial_attributes = 4;
	craft->start_rotate_crafting();
}

void Script::peger_items() {
	std::vector<unsigned int> list_items = { 3044, 0x4976, 0x497D };

	Sleep(3000);

	basic_actions->use_item(0x2C26, 0);
	basic_actions->select_npc(980934);
	Sleep(1000);
	basic_actions->send_basic_skills_ex(0x01C1, 980934);

	while (true) {
		items->get_all_items(list_items);
		Sleep(500);
	}
}

void Script::opening_chest() {

/*
	Baú da fortuna                   = 0x623C
	Coral                            = 0x6B50
	Pacote misterioso                = 0x7A8B
	Perg. Tigre lendário             = 0x6CF4
	Pacote do sol nascente           = 0x6E7A
	Pacote da vida selvagem          = 0x7D5A
	Pacote cósmico                   = 0x815B
	Pacote de comemoração universal  = 0x8173
	Pergaminho da liberdade          = 0x7964
*/

	/*
		jogar fora
		vale da lua arma = 0x5AD2
		amuleto de hp = 0x3D91
		amuleto de mp = 0x3D90
		diamante de def nv 1 = 0x5182
		pedra nevasca = 0x07D3
		bau de cristal nirvana arma = 0x6B70
		Simbolo cidade de gelo marcial = 0x5DA1
		pedra de xuan yuan = 0x315E
		pedra do tigre = 0x5381
		prova do campeao = 0x5021
		bau dos elementos = 0x6B72
		pedra verdejante = 0x07DD

		pedra do olho vermelho = 0x07CE
		reliquia da alma = 0x6B6D
		asa de sucesso = 0x623E
		insignia intrepida = 0x32A2
		pedra de nu wa = 0x315D
		vale acessorio = 0x5AD3
		mamute = 0x6E7B
		tigre de chamas = 0x6C6F
		galinha = 0x6FE3
		marca de casco = 0x7D60
		pluma espiritual = 0x6F5E
		lagosta colossal = 0x60C0
	*/

	general->set_update_flag(1);

	Sleep(3000); // tempo pro inventário atualizar

	Timer drop_items_timer;
	Timer chest_quest_timer;
	Timer list_quest_update_timer;

	std::vector<unsigned int> id_chest = { 0x623C, 0x6B50, 0x7A8B, 0x6CF4,
									       0x6E7A, 0x7D5A, 0x815B, 0x8173, 0x7964 };
	std::vector<unsigned int> drop_items = { 0x5AD2, 0x3D91, 0x3D90, 0x5182, 0x07D3,
											 0x6B70, 0x5DA1, 0x315E, 0x5381, 0x5021, 
		                                     0x6B72, 0x07DD, 0x07CE, 0x6B6D, 0x623E,
											 0x32A2, 0x315D, 0x5AD3, 0x6E7B, 0x6C6F, 
											 0x6FE3, 0x7D60, 0x6F5E };

	std::set<unsigned int> list_quest_update;

	for (int i : { 20191, 21385, 21503, 22053, 24240, 24385, 24921, 25529, 25544 }) {
		basic_actions->send_request_chest_item(i);
		Sleep(400);
	}

	while (true) {
		auto list_items = inventory->get_linear_list_item();

		for (auto& i : id_chest) {
			for (auto& k : list_items) {
				if (k.id == i) {
					basic_actions->use_item(i, k.slot_position);
					//std::cout << "bau: " << std::hex << i << " aberto" << std::endl;
					chest_quest_timer.reset();
					while (general->get_chest_id() == 0 && chest_quest_timer.elapsed() < 8000)
						Sleep(200);
					list_quest_update.insert(general->get_chest_id());
					general->clear_chest_id();
					break;
				}
			}
			Sleep(500);
		}

		for (auto& i : list_quest_update) {
			basic_actions->send_request_chest_item(i);
			Sleep(400);
		}

		if (drop_items_timer.elapsed() >= inventory->get_update_time() * 1.2) {
			for (auto& i : drop_items) {
				for (auto& k : list_items) {
					if (i == k.id) {
						// se esse item for empilhável e haver mts dele pode se tornar inviável o drop deles..
						// então talvez seja melhor jogar 999 dos itens, mesmo que não tenha essa
						basic_actions->drop_items(k.slot_position, k.stack);
						Sleep(800);
					}
				}
			}
			drop_items_timer.reset();
		}
	}
}

void Script::meus_buffs() {

	/*
	
		SKILLS EP = 124 / 194 / 121 / 122

		SKILLS WR = EVIL 167     |    GOD 422

		SKILLS WB = EVIL 513 / 517    GOD 512 / 516

		SKILL MG = EVIL 2263

		SKILLS MS = 1859

		SKILLS SK = 1343

		SKILLS RT = EVIL 2757 | 

		SKILLS TM = GOD 2801
	
	*/

	Sleep(2000);

	while (1) {
		switch (basic_hash(player_basic_info.second.c_str())) {
		case basic_hash("Hentay"):
		{
			basic_actions->send_basic_skills(124, player_basic_info.first);
			Sleep(3000);
			basic_actions->send_basic_skills(194, player_basic_info.first);
			Sleep(3000);
			basic_actions->send_basic_skills(121, player_basic_info.first);
			Sleep(3000);
			basic_actions->send_basic_skills(122, player_basic_info.first);
			Sleep(5000);
			break;
		}
		case basic_hash("DameAura"):
		{
			basic_actions->send_basic_skills(422, player_basic_info.first);
			Sleep(1500);
			break;
		}
		case basic_hash("Wegoe"):
		{
			basic_actions->send_basic_skills(512, player_basic_info.first);
			Sleep(1500);
			basic_actions->send_basic_skills(516, player_basic_info.first);
			Sleep(1500);
			break;
		}
		case basic_hash("Guindaste"):
		{
			basic_actions->send_basic_skills(2263, player_basic_info.first);
			Sleep(1500);
			break;
		}
		case basic_hash("AMACIANTE"):
		{
			basic_actions->send_basic_skills(1859, player_basic_info.first);
			Sleep(1500);
			break;
		}
		case basic_hash("essecah"):
		{
			basic_actions->send_basic_skills(1343, player_basic_info.first);
			Sleep(1500);
			break;
		}
		case basic_hash("RT"):
		{
			basic_actions->send_basic_skills(2757, player_basic_info.first);
			Sleep(1500);
			break;
		}
		case basic_hash("atormentada"):
		{
			basic_actions->send_basic_skills(2801, player_basic_info.first);
			Sleep(1500);
			break;
		}

		default:
			break;
		}
	}
}

//void Script::seguir_player_a_uma_certa_altura(float height) {
//	while (follow_record) {
//
//		if (!movement->on_air) {
//			this->follow_record = false;
//			while (!record_positions.empty()) record_positions.pop();
//			return;
//		}
//
//		if (!movement->follow) {
//			if (movement->stuck.first) {
//				movement->set_move_counter(movement->stuck.first);
//				movement->player_x = movement->stuck.second.x;
//				movement->player_y = movement->stuck.second.y;
//				movement->player_z = movement->stuck.second.z;
//
//				movement->stuck.first = 0;
//			}
//
//			unsigned int size = this->record_positions.size();
//
//			float cateto_1 = this->record_positions.back().first.x - player_x;
//			float cateto_2 = this->record_positions.back().first.y - player_y;
//
//			float distance = sqrt(pow(cateto_1, 2) + pow(cateto_2, 2));
//
//			if (distance > this->speed / 2) {
//				float c_hyp = this->speed / (2 * distance);
//				float c_cateto1 = cateto_1 * c_hyp;
//				float c_cateto2 = cateto_2 * c_hyp;
//
//				this->player_x += c_cateto1;
//				this->player_y += c_cateto2;
//
//				move(player_x, player_y, this->record_positions.back().first.z + 10, this->speed, FLY);
//			}
//			else if (record_positions.back().second == 0x23) {
//				this->player_x += cateto_1;
//				this->player_y += cateto_2;
//				this->last_player_height = record_positions.back().first.z;
//				move_stop(player_x, player_y, record_positions.back().first.z + 10, this->speed, FLY);
//				for (size_t i = 0; i < size; i++) record_positions.pop();
//			}
//			else {
//				this->player_x += cateto_1;
//				this->player_y += cateto_2;
//				move(player_x, player_y, record_positions.back().first.z + 10, this->speed, FLY);
//				for (size_t i = 0; i < size; i++) record_positions.pop();
//			}
//		}
//		Sleep(1);
//	}
//
//	while (!record_positions.empty()) record_positions.pop();
//}

void Script::set_player_info(std::pair<unsigned int, std::string> player_basic_info, 
	Logging_config log_config) {
	this->player_basic_info = player_basic_info;
	this->config            = log_config;
}

void Script::set_enviroment(std::shared_ptr<Inventory>      inventory_arg,
	                        std::shared_ptr<Basic_actions>  actions_arg, 
	                        std::shared_ptr<Items>          items_arg,
	                        std::shared_ptr<Movement>       movement_arg,
	                        std::shared_ptr<Craft>          craft_arg,
	                        std::shared_ptr<Message>        message_arg,
	                        std::shared_ptr<General>        general_arg) {
	this->inventory      = inventory_arg;
	this->basic_actions  = actions_arg;
	this->items          = items_arg;
	this->movement       = movement_arg;
	this->craft          = craft_arg;
	this->message        = message_arg;
	this->general        = general_arg;
}
