#include "stdafx.h"
#include "Healing.h"

using namespace std;

Healing::Healing(Aux_function& aux_arg) : aux(aux_arg) {
	this->update_flag = false;
}

void Healing::scan_packet(std::vector<unsigned char>& arr) {
	for (int i = 0; i < (int)arr.size() - 22; i++) {
		if (*(unsigned int*)&arr[i] == 0x26262722 && arr[i + 5] == 0x69) {
			this->health      = *(unsigned int*)&arr[i + 9];
			this->max_health  = *(unsigned int*)&arr[i + 13];
			this->mana        = *(unsigned int*)&arr[i + 17];
			this->max_mana    = *(unsigned int*)&arr[i + 21];

			cout << "HP: " << this->health << endl;
			cout << "MP: " << this->mana << endl;
		}
	}
}

unsigned int Healing::get_health() const {
	return this->health;
}

unsigned int Healing::get_mana() const {
	return this->mana;
}

unsigned int Healing::get_max_health() const {
	return this->max_health;
}

unsigned int Healing::get_max_mana() const {
	return this->max_mana;
}

void Healing::set_update_flag(bool f) {
	this->update_flag = f;
}

bool Healing::get_update_flag() const {
	return this->update_flag;
}
