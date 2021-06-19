#include "stdafx.h"
#include "RC4.h"

RC4::RC4() {
	for ( int i = 0; i < 256; i++ )
		m_table[i] = i;

	btShift_1 = 0;
	btShift_2 = 0;
}

// RC4 COM PEQUENAS MODIFCAÇÕES DO RC4 USUAL..

void RC4::KSA(std::vector<unsigned char>& key) {
	byte Shift = 0;
	for (int i = 0; i < 256; i++) {
		byte A = key[i % 16];
		Shift += (byte)(A + m_table[i]);
		byte B = m_table[i];
		m_table[i] = m_table[Shift];
		m_table[Shift] = B;
	}
}

void RC4::PRGA(std::vector<unsigned char>& message) {
	for (size_t i = 0; i < message.size(); i++) {
		btShift_1++;
		byte A = m_table[btShift_1];
		btShift_2 += A;
		byte B = m_table[btShift_2];
		m_table[btShift_2] = A;
		m_table[btShift_1] = B;
		byte C = (byte)(A + B);
		byte D = m_table[C];
		message[i] = (byte)(message[i] ^ D);
	}
}
