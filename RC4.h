#pragma once
#include <Windows.h>
#include <vector>

class RC4
{
public:
	RC4();
	void KSA(std::vector<unsigned char>& key);
	void PRGA(std::vector<unsigned char>& message);

private:
	BYTE m_table[256];
	BYTE btShift_1;
	BYTE btShift_2;
};

