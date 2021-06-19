#pragma once
#include <optional>
#include "Aux_function.h"
#include "Basic_actions.h"

// versões

#define v_153_CAPACITY_OFFSET   211
#define v_153_START_OFFSET      216

#define v_151_CAPACITY_OFFSET   203
#define v_151_START_OFFSET      208

#define v_155_CAPACITY_OFFSET   211
#define v_155_START_OFFSET      216

struct item_data {
	unsigned int   id             = 0;
	unsigned int   stack          = 0; // 2 bytes ou 4 bytes? ..
	unsigned char  slot_position  = 0;
};

class Inventory {
public:
	Inventory(Aux_function& arg_aux);

	void  set_enviroment(std::shared_ptr<Basic_actions> actions_arg);

	std::optional<item_data> search_item_by_id(unsigned int id);
	void           get_info();
	unsigned char  get_item_count() const;
	unsigned int   get_item_count_by_id(unsigned int id) const;
	unsigned char  get_capacity() const;
	unsigned char  get_count_empty_slots() const;
	void           scan_packet(std::vector<unsigned char>& decode_data);
	void           set_update_time(unsigned int time);
	unsigned int   get_update_time() const;
	void           start_update();
	int            get_first_empty_slot() const;

	void           set_update_flag(bool f);
	bool           get_update_flag() const;

	std::multimap<unsigned int, item_data> get_list_item() const;
	std::vector<item_data>          get_linear_list_item() const;
	std::vector<int>                list_to_empty_slots()  const;
	std::vector<int>                list_to_items_by_id(int item_id)  const;

	void drop_item_by_id(unsigned int id, unsigned int count);

private:
	void print_items();

	const int       capacity_offset  = v_153_CAPACITY_OFFSET;
	const int       start_offset     = v_153_START_OFFSET;
	unsigned char   capacity;
	Aux_function&   aux;
	bool            check_info;
	unsigned int    update_time;

	void update();

	std::multimap<unsigned int, item_data>  items;
	std::vector<item_data>                  arr_items;
	std::shared_ptr<std::thread>            t_thread;

	std::shared_ptr<Basic_actions>          basic_actions;

	bool                                    update_flag;
};

