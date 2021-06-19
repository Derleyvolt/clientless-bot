#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <algorithm>
#include <string>
#include <map>
#include <mutex>
#include <numeric>
#include <queue>
#include <chrono>
#include "Socket.h"
#include "EncDec.h"
#include "Decomp.h"

void dec_arr(unsigned char arr[], int length);
void dec_vector(std::vector<unsigned char>& arr);

constexpr unsigned int basic_hash(const char* str, int h = 0) {
	return !str[h] ? 5381 : (basic_hash(str, h + 1) * 33) ^ str[h];
}

typedef std::vector<unsigned char> list;

class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> m_beg;

public:
	Timer() : m_beg(std::chrono::steady_clock::now()) {
	}

	void reset() {
		m_beg = std::chrono::steady_clock::now();
	}

	int elapsed() const {
		return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - m_beg).count();
	}
};

class Aux_function {
public:
	Aux_function();

	int   serialize(list & packet, std::string login, list hash);
	int   crypt_send(list& message);
	void  decrypt_unpack(list& decode_data, int recv_length, unsigned char recv_buf[]);
	list  reverse_bytes(int val);
	int   reverse_bytes_ex(int val);
	int   recv_complete(unsigned char recv_buf[], int& recv_length);
	void  get_player_basic_info(list decode_data, std::map<std::string, unsigned int>& player_info);
	void  set_descriptor(SOCKET arg_socket_descriptor);


	EncDec crypt;
private:
	MPPCUnpack dec;
	SOCKET socket_descriptor;
	std::shared_ptr<std::mutex> mt;
};
