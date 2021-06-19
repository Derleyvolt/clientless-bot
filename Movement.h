#pragma once
#include "Aux_function.h"

#define FLY      0x61
#define GROUND   0x21
#define FALLING  0x26
#define JUMPING  0x28

#define DEFAULT_FLY_SPEED     7
#define DEFAULT_GROUND_SPEED  4.8

struct Coord {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};

class Stuck {
public:
	Stuck() : coord({ 0, {} }) {
	}

	operator bool() const {
		return coord.first;
	}

	Coord get_coord() const;

private:
	friend class Movement;

	std::pair<unsigned short, Coord> coord;
};

class Follow {
public:
	Follow() : mt(std::make_shared<std::mutex>()), target_id(0) {
	}

	// retorna true se ainda houver um player a ser seguido
	operator bool() const {
		return target_id;
	}

	bool           have_coord()        const; // retorna true se target_coord não for empty
	unsigned int   get_target_id()     const;
	Coord          get_target_coord()  const;
	unsigned char  get_walk_type()     const;
	void           set_target_id(unsigned int id);
	void           clean_up();

private:
	friend class Movement;

	void set_target_coord(Coord coord, unsigned char type);
	std::shared_ptr<std::mutex>      mt;
	unsigned int                     target_id;
	std::pair<Coord, unsigned char>  target_coord;
};

class Movement {
public:
	Movement(Aux_function& aux_arg);

	void  set_player_info(std::vector<unsigned char>& decode_data, unsigned int player_id);
	/*void  set_enviroment(std::shared_ptr<Basic_actions> basic_actions_arg);*/

	void  move(float dir_x, float dir_y, float dir_z, unsigned short speed_arg, unsigned char type, 
		       unsigned short time = 500);
	void  move_stop(float dir_x, float dir_y, float dir_z, unsigned short speed_arg, unsigned char type);

	void  move_jump();
	void  move_on_ground();
	void  move_vertial(float height);
	void  get_fly();

	void          set_fly(unsigned int id);
	void          set_last_height(float z); // isso deve ser setado para tornar possível do bot aterrisar 
	void          set_speed(float speed);
	bool          get_flying_flag()  const;
	float         get_speed()        const;
	void          set_move_counter(unsigned int move_counter);
	unsigned int  get_move_counter(unsigned int move_counter) const;
	void          scan_packet(std::vector<unsigned char>& decode_data);
	void          move_on_air();

	void move_on_air_ex(Coord dest);

	void  check_and_fix_stuck();
	void  set_update_flag(bool f);
	bool  get_update_flag() const;

	// coordenadas do player
	float player_x;
	float player_y;
	float player_z;
	
	Follow follow;
private:
	void  set_flying_flag(bool flag);
	void  move_fall();
	void  set_coord(Coord coord);
	void  fly(unsigned int fly_id);

	/*std::shared_ptr<Basic_actions> basic_actions;*/
	Aux_function& aux;

	unsigned int  fly_id;
	float         speed; // arrumar um jeito de obter essa informação
	Stuck         stuck;
	bool          flying;
	float         last_player_height;
	unsigned int  player_id;
	unsigned int  move_counter;

	bool update_flag;
};

