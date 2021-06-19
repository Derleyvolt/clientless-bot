#pragma once
#include "Aux_function.h"

class Healing {
public:
	Healing(Aux_function& aux_arg);

	void          scan_packet(std::vector<unsigned char>& arr);
	unsigned int  get_health()  const;
	unsigned int  get_mana()    const;
	unsigned int  get_max_health()  const;
	unsigned int  get_max_mana()    const;
	void          set_update_flag(bool f);
	bool          get_update_flag() const;

private:
	Aux_function&  aux;
	unsigned int   health;
	unsigned int   max_health;
	unsigned int   mana;
	unsigned int   max_mana;

	bool update_flag;
};

