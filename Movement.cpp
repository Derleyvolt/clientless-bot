#include "stdafx.h"
#include "Movement.h"

Movement::Movement(Aux_function& aux_arg) : aux(aux_arg) {
	this->flying              = false;
	this->move_counter        = 0;
	this->last_player_height  = 0;
	this->update_flag         = false;
}

void Movement::move(float dir_x, float dir_y, float dir_z, unsigned short speed_arg, 
	unsigned char type, unsigned short time) {
	list move_packet(36);

	unsigned short speed = unsigned short(speed_arg * 256 + 0.5);

	move_packet[0] = 0x22;
	move_packet[1] = 0x22;
	move_packet[2] = 0x21;
	move_packet[3] = 0x00;
	move_packet[4] = 0x00;

	memcpy(move_packet.data() + 5,  &dir_x, 4);
	memcpy(move_packet.data() + 9,  &dir_z, 4);
	memcpy(move_packet.data() + 13, &dir_y, 4);

	memcpy(move_packet.data() + 17, &dir_x, 4);
	memcpy(move_packet.data() + 21, &dir_z, 4);
	memcpy(move_packet.data() + 25, &dir_y, 4);

	memcpy(move_packet.data() + 29, &time, 2);
	memcpy(move_packet.data() + 31, &speed, 2);
	memcpy(move_packet.data() + 33, &type, 1);
	memcpy(move_packet.data() + 34, &move_counter, 2);

	move_counter += 1;

	aux.crypt_send(move_packet);
	Sleep(time);
}

void Movement::move_stop(float dir_x, float dir_y, float dir_z, unsigned short speed_arg, unsigned char type) {
	list move_packet(25);

	unsigned short speed = speed_arg * 256 + 0.5;

	unsigned char  direction    = 0x88;
	unsigned short direction_1  = 0x1EF;

	move_packet[0]  = 0x22;
	move_packet[1]  = 0x17;
	move_packet[2]  = 0x16;
	move_packet[3]  = 0x07;
	move_packet[4]  = 0x00;
	move_packet[24] = 0x0;

	memcpy(move_packet.data() + 5,  &dir_x, 4);
	memcpy(move_packet.data() + 9,  &dir_z, 4);
	memcpy(move_packet.data() + 13, &dir_y, 4);

	memcpy(move_packet.data() + 17, &speed, 2);
	memcpy(move_packet.data() + 19, &direction, 1);
	memcpy(move_packet.data() + 20, &type, 1);

	memcpy(move_packet.data() + 21, &move_counter, 2);
	memcpy(move_packet.data() + 23, &direction_1, 2);
	aux.crypt_send(move_packet);

	move_counter++;
	Sleep(100);
}

void Movement::move_jump() {
	check_and_fix_stuck();
	move_stop(player_x, player_y, player_z + 2, speed, JUMPING);
	player_z += 2;
}

// usada pra follow
void Movement::move_on_air() {
	bool move_stopped = false;

	while (this->follow) {
		if (!this->flying) {
			this->follow.set_target_id(0);
			this->follow.clean_up();
			return;
		}

		if (this->follow.have_coord()) {
			check_and_fix_stuck();

			move_stopped = true;

			float speed     = get_speed();

			float cateto_1  = this->follow.get_target_coord().x - player_x;
			float cateto_2  = this->follow.get_target_coord().y - player_y;
			float cateto_3  = this->follow.get_target_coord().z + 5 - player_z;
			float distance  = sqrt(pow(cateto_1, 2) + pow(cateto_2, 2) + pow(cateto_3, 2));

			if (distance > speed / 2) {
				float c_hyp      = speed / (2 * distance);
				float c_cateto1  = cateto_1 * c_hyp;
				float c_cateto2  = cateto_2 * c_hyp;
				float c_cateto3  = cateto_3 * c_hyp;

				this->player_x += c_cateto1;
				this->player_y += c_cateto2;
				this->player_z += c_cateto3;

				move(player_x, player_y, this->player_z, speed, FLY);
			}
			else if (this->follow.get_walk_type() == 0x23) {
				this->player_x += cateto_1;
				this->player_y += cateto_2;
				this->player_z += cateto_3;
				this->last_player_height = this->follow.get_target_coord().z;
				move_stop(player_x, player_y, this->player_z, speed, FLY);
				this->follow.clean_up();
			}
			else {
				this->player_x += cateto_1;
				this->player_y += cateto_2;
				this->player_z += cateto_3;
				move(player_x, player_y, this->player_z, speed, FLY);
				//this->follow.clean_up();
			}
		}
		else if (move_stopped) {
			move_stop(player_x, player_y, this->player_z, speed, FLY);
			move_stopped = false;
		}
		Sleep(1);
	}

	this->follow.clean_up();
}

void Movement::move_on_air_ex(Coord dest) {
	bool move_stopped = false;

	while (1) {
		if (this->follow.have_coord()) {
			check_and_fix_stuck();

			move_stopped = true;

			float speed = get_speed();

			float cateto_1 = dest.x - player_x;
			float cateto_2 = dest.y - player_y;
			float cateto_3 = dest.z + 5 - player_z;
			float distance = sqrt(pow(cateto_1, 2) + pow(cateto_2, 2) + pow(cateto_3, 2));

			if (distance > speed / 2) {
				float c_hyp = speed / (2 * distance);
				float c_cateto1 = cateto_1 * c_hyp;
				float c_cateto2 = cateto_2 * c_hyp;
				float c_cateto3 = cateto_3 * c_hyp;

				this->player_x += c_cateto1;
				this->player_y += c_cateto2;
				this->player_z += c_cateto3;

				move(player_x, player_y, this->player_z, speed, FLY);
			}
			else if (this->follow.get_walk_type() == 0x23) {
				this->player_x += cateto_1;
				this->player_y += cateto_2;
				this->player_z += cateto_3;
				this->last_player_height = dest.z;
				move_stop(player_x, player_y, this->player_z, speed, FLY);
				this->follow.clean_up();
			}
			else {
				this->player_x += cateto_1;
				this->player_y += cateto_2;
				this->player_z += cateto_3;
				move(player_x, player_y, this->player_z, speed, FLY);
				//this->follow.clean_up();
			}
		}
		else if (move_stopped) {
			move_stop(player_x, player_y, this->player_z, speed, FLY);
			move_stopped = false;
		}
		Sleep(1);
	}

	this->follow.clean_up();
}

float Movement::get_speed() const {
	return this->speed;
}

void Movement::move_on_ground() {
	while (this->follow) {
		if (this->follow.have_coord()) {
			check_and_fix_stuck();

			float speed = get_speed();

			float cateto_1 = this->follow.get_target_coord().x - player_x;
			float cateto_2 = this->follow.get_target_coord().y - player_y;
			float cateto_3 = this->follow.get_target_coord().z - player_y;

			float distance = sqrt(pow(cateto_1, 2) + pow(cateto_2, 2));

			if (distance > speed / 2) {
				float c_hyp      = speed / (2 * distance);
				float c_cateto1  = cateto_1 * c_hyp;
				float c_cateto2  = cateto_2 * c_hyp;

				this->player_x += c_cateto1;
				this->player_y += c_cateto2;
				move(player_x, player_y, player_z, speed, 0);
			}
			else if (this->follow.get_walk_type() == 0x19) {
				player_x += cateto_1;
				player_y += cateto_2;
				player_z += cateto_3;
				move_stop(player_x, player_y, player_z, speed, GROUND);
				this->follow.clean_up();
			}
			else {
				player_x += cateto_1;
				player_y += cateto_2;
				player_z += cateto_3;
				move_stop(player_x, player_y, player_z, speed, GROUND);
				this->follow.clean_up();
			}
		}
		Sleep(1);
	}

	this->follow.clean_up();
}

void Movement::move_vertial(float height) {
	float start_height = this->player_z;
	while (true) {
		check_and_fix_stuck();

		float distance = start_height + height - this->player_z;

		float speed = get_speed();

		if (abs(distance) > speed / 2) {
			if (distance < 0)
				player_z -= speed / 2;
			else
				player_z += speed / 2;
			move(player_x, player_y, player_z, speed, FLY);
		}
		else {
			if (distance < 0)
				player_z -= speed / 2;
			else
				player_z += speed / 2;
			move_stop(player_x, player_y, player_z, speed, FLY);
			return;
		}
	}
}

void Movement::get_fly() {
	if (!this->get_flying_flag()) {
		this->set_last_height(this->player_z);
		this->move_jump();
	}
	this->fly(this->fly_id);
}

void Movement::fly(unsigned int fly_id) {
	std::vector<unsigned char> arr = { 0x22, 0xB, 0xA, 0x28, 0, 1, 1, 0xC, 0 };
	arr.resize(13);
	memmove(arr.data() + 9, &fly_id, sizeof(fly_id));
	aux.crypt_send(arr);
}

void Movement::check_and_fix_stuck() {
	if (this->stuck) {
		this->set_coord(stuck.get_coord());
		move_counter             = stuck.coord.first;
		this->stuck.coord.first  = 0;
	}
}

void Movement::set_update_flag(bool f) {
	this->update_flag = f;
}

bool Movement::get_update_flag() const {
	return this->update_flag;
}

//void Movement::move_on_air() {
//	while (follow_record) {
//		if (!this->flying) {
//			this->follow_record = false;
//			while (!record_positions.empty()) record_positions.pop();
//			return;
//		}
//
//		if (!record_positions.empty()) {
//			if (stuck.first) {
//				move_counter   = stuck.first;
//				this->player_x = stuck.second.x;
//				this->player_y = stuck.second.y;
//				this->player_z = stuck.second.z;
//
//				stuck.first    = 0;
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
//				float c_hyp     = this->speed / (2 * distance);
//				float c_cateto1 = cateto_1 * c_hyp;
//				float c_cateto2 = cateto_2 * c_hyp;
//
//				this->player_x += c_cateto1;
//				this->player_y += c_cateto2;
//
//				move(player_x, player_y, this->record_positions.back().first.z, this->speed, FLY);
//			}
//			else if (record_positions.back().second == 0x23) {
//				this->player_x += cateto_1;
//				this->player_y += cateto_2;
//				this->last_player_height = record_positions.back().first.z;
//				move_stop(player_x, player_y, record_positions.back().first.z, this->speed, FLY);
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

void Movement::scan_packet(std::vector<unsigned char>& decode_data) {
	if (!update_flag)
		return;

	system("cls");

	int index;
	for (index = 0; index < (int)decode_data.size() - 2; index++) {
		if (decode_data[index] == 0x22 && decode_data[index + 1] == 0x18 && decode_data[index + 2] == 0x17 ||
			decode_data[index] == 0x22 && decode_data[index + 1] == 0x17 && decode_data[index + 2] == 0x16) {
			unsigned int object_id = *(unsigned int*)&decode_data[index + 5];
			std::cout << "id: " << object_id << std::endl;
			if (this->follow.get_target_id() == *(unsigned int*)&decode_data[index + 5]) {
				float x = *(float*)&decode_data[index + 9];
				float z = *(float*)&decode_data[index + 0xD];
				float y = *(float*)&decode_data[index + 0x11];

				if (this->follow)
					this->follow.set_target_coord(Coord{ x, y, z }, decode_data[index + 3]);
				break;
			}
		}
	}

	for (index = 0; index < (int)decode_data.size() - 3; index++) {
		if (decode_data[index] == 0x22 && decode_data[index + 1] == 0x7 && decode_data[index + 2] == 0x6) {
			if (decode_data[index + 3] == 0x60 && 
				*reinterpret_cast<unsigned int*>(&decode_data[index + 5]) == this->player_id) {
				//std::cout << "voo aberto" << std::endl;
				this->set_flying_flag(true);
				break;
			}
			else if (decode_data[index + 3] == 0x61 &&
				*reinterpret_cast<unsigned int*>(&decode_data[index + 5]) == this->player_id) {
				//std::cout << "voo fechado" << std::endl;
				this->set_flying_flag(false);
				std::thread t_thread(&Movement::move_fall, this);
				t_thread.detach();
				break;
			}
		}
	}

	for (index = 0; index < (int)decode_data.size() - 3; index++) {
		if (decode_data[index] == 0x22 && decode_data[index + 1] == 0x17 && decode_data[index + 2] == 0x16)
			if (decode_data[index + 3] == 0x36 && *reinterpret_cast<unsigned int*>(&decode_data[index + 5]) 
				== player_id && decode_data[index + 28] == 0xB1 ) {
				//std::cout << "PRESO" << std::endl;
 				float x = *(float*)&decode_data[index + 30];
				float z = *(float*)&decode_data[index + 34];
				float y = *(float*)&decode_data[index + 38];
				this->stuck.coord.second = Coord{ x, y, z };
				this->stuck.coord.first  = *(unsigned short*)&decode_data[index + 42];
				break;
			}
			else if (decode_data[index + 3] == 0xE) {
				this->player_x = *(float*)&decode_data[index + 5];
				this->player_z = *(float*)&decode_data[index + 9];
				this->player_y = *(float*)&decode_data[index + 13];		
				break;
			}
	}
}

void Movement::move_fall() {
	const float fall_speed = 15 / 2;

	while (true) {
		if (this->stuck)
			return;

		float distance = this->player_z - this->last_player_height;

		if (distance > fall_speed) {
			this->player_z -= fall_speed;
			move(this->player_x, this->player_y, this->player_z, 15, FALLING, 250);
		}
		else {
			this->player_z -= distance;
			move_stop(this->player_x, this->player_y, this->player_z, 7, FALLING);
			return;
		}
	}
}

// baixo tem funções de get e set ------------------------------------------------------------------------------

void Movement::set_coord(Coord coord) {
	this->player_x = coord.x;
	this->player_y = coord.y;
	this->player_z = coord.z;
}

void Movement::set_player_info(std::vector<unsigned char>& decode_data, unsigned int player_id) {
	unsigned int index;

	this->player_id = player_id;

	for (index = 0; index < decode_data.size() - 4; index++) {
		if (decode_data[index] == 0x22 && decode_data[index + 1] == 0x17 &&
			decode_data[index + 2] == 0x16 && decode_data[index + 3] == 0xE) {
			this->player_x = *(float*)&decode_data[index + 5];
			this->player_z = *(float*)&decode_data[index + 9];
			this->player_y = *(float*)&decode_data[index + 13];
			return;
		}
	}
}

//void Movement::set_enviroment(std::shared_ptr<Basic_actions> basic_actions_args) {
//	//this->basic_actions = basic_actions_args;
//}

unsigned int Movement::get_move_counter(unsigned int move_counter) const {
	return this->move_counter;
}

void Movement::set_flying_flag(bool flag) {
	if (flag) 
		this->speed = DEFAULT_FLY_SPEED;
	else 
		this->speed = DEFAULT_GROUND_SPEED;
	this->flying = flag;
}

bool Follow::have_coord() const {
	return this->target_coord.second;
}

unsigned int Follow::get_target_id() const {
	return this->target_id;
}

Coord Follow::get_target_coord() const {
	return this->target_coord.first;
}

void Follow::set_target_id(unsigned int id) {
	this->target_id = id;
}

unsigned char Follow::get_walk_type() const {
	return this->target_coord.second;
}

void Follow::clean_up() {
	std::lock_guard<std::mutex> lock(*mt);
	this->target_coord.first   = {};
	this->target_coord.second  = 0;
}

void Follow::set_target_coord(Coord coord, unsigned char type) {
	std::lock_guard<std::mutex> lock(*mt);
	this->target_coord.first   = coord;
	this->target_coord.second  = type;
}

void Movement::set_fly(unsigned int id) {
	this->fly_id = id;
}

void Movement::set_last_height(float z) {
	this->last_player_height = z;
}

void Movement::set_speed(float speed) {
	this->speed = speed;
}

bool Movement::get_flying_flag() const {
	return this->flying;
}

void Movement::set_move_counter(unsigned int move_counter) {
	this->move_counter = move_counter;
}

Coord Stuck::get_coord() const {
	return this->coord.second;
}
