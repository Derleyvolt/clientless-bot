#pragma once
#include <unordered_map>
#include <chrono>
#include "Aux_function.h"
#include "Movement.h"

class Item {
public:
	unsigned int id;
	unsigned int id_unique;

	Coord coord;
};

class Items {
public:
	Items(Aux_function& aux_arg);

	void  scan_packet(std::vector<unsigned char>& arr);
	void  set_enviroment(std::shared_ptr<Movement> movement_arg);
	void  get_initial_objects(std::vector<unsigned char>& arr);
	bool  pick_items(std::vector<unsigned int> arr_id, float distance = 7.f); // os itens que vêm primeiro têm prioridade..
	void  set_time_wait(unsigned char time);
	void  set_scannning_flag(bool f);
	void  start_scanning(std::vector<unsigned int> arr, float distance = 7.f);

	void  get_all_items(std::vector<unsigned int> items);

	unsigned int items_count() const;

	void  set_update_flag(bool f);
	bool  get_update_flag() const;

private:
	Aux_function& aux;
	std::unordered_map<unsigned int, std::chrono::steady_clock::time_point> black_list;
	std::unordered_map<unsigned int, Item> items_list;
	std::shared_ptr<Movement> movement;

	void   clear_black_list();
	bool   in_black_list(unsigned int id_unique);
	void   add_to_black_list(unsigned int id_unique);
	float  distance(Coord coord);
	void   pick(unsigned int id_unique, unsigned int id);
	// ganância, 1.5.5 .. vou por um limite de 30 itens
	void   get_all_items_unmanaged(std::vector<std::pair<unsigned int, unsigned int>> items);

	unsigned char time_to_wait;
	bool scanning_item;

	bool update_flag;
};

