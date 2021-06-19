#include "stdafx.h"
#include "EncDec.h"


void EncDec::CreateCrypt(std::vector<unsigned char>& key)
{
	m_enc.KSA(key);
}

void EncDec::CreateDecrypt(std::vector<unsigned char>& key)
{
	m_dec.KSA(key);
}

void EncDec::Encode(std::vector<unsigned char>& message)
{
	m_enc.PRGA(message);
}

void EncDec::Decode(std::vector<unsigned char>& message)
{
	m_dec.PRGA(message);
}
