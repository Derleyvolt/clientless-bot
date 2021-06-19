#pragma once
#include <vector>
#include <set>
#include "Aux_function.h"

class Party {
public:
	Party(Aux_function& arg_aux);

	void  scan_packet(std::vector<unsigned char>& decode_data);
	void  add_permission(unsigned int player_id);
	void  clear_permission_list();
	void  transfer_leadership(unsigned int player_id) const;
	void  leave_party() const;

	void set_update_flag(bool f);
	bool get_update_flag() const;

	std::set<unsigned int>& get_permission_list();

private:
	void  analyze_request(std::vector<unsigned char> data_decode, unsigned int type);
	void  accept_request(unsigned int player_id, unsigned int unknown);
	void  accept_invite(unsigned int  player_id);


	unsigned int packet_index;
	Aux_function& aux;
	std::set<unsigned int> permission;

	bool update_flag;
};