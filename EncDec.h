#pragma once
#include "RC4.h"

class EncDec
{
public:
	void CreateCrypt(std::vector<unsigned char>& key);
	void CreateDecrypt(std::vector<unsigned char>& key);
	void Encode(std::vector<unsigned char>& message);
	void Decode(std::vector<unsigned char>& message);

private:
	RC4 m_enc;
	RC4 m_dec;
};