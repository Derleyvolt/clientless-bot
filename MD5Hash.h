#pragma once
#include "HMAC.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

class MD5Hash
{
public:
	void hash_logging(std::string login, std::string senha, unsigned char key[], int length, 
		std::vector<unsigned char>& output);
	void hash(unsigned char key[], int length, std::vector<unsigned char>& output);

private:
	BYTE  m_btLogin[16];
	BYTE  m_btHash[16];
	HMAC  m_md5;
};

