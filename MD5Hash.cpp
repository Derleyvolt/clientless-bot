#include "stdafx.h"
#include "MD5Hash.h"

void MD5Hash::hash_logging(std::string login, std::string senha, unsigned char key[], int length, 
	std::vector<unsigned char>& output) {
	memset(m_btLogin, 0, sizeof(m_btLogin));
	m_md5.HMACMD5_(login, senha, key, length, m_btHash);
	memcpy(m_btLogin, login.c_str(), login.length());
	std::copy(m_btHash, m_btHash + 16, output.begin());
}

void MD5Hash::hash(unsigned char key[], int length, std::vector<unsigned char>& output) {
	unsigned char btKey[32];
	memcpy(btKey, m_btHash, sizeof(m_btHash));
	memcpy(btKey + 16, key , length);
	m_md5.HMACMD5((char*)m_btLogin, btKey, sizeof(btKey), output);
}
