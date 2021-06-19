#include "stdafx.h"
#include "Socket.h"
#include <iostream>

Socket::Socket()
{
	memset(&m_Addr, 0, sizeof(m_Addr));
}

void Socket::setServerIP(LPCSTR lpszHostIP)
{
	m_Addr.sin_addr.s_addr = inet_addr(lpszHostIP);
}

void Socket::setPort(USHORT wPort)
{
	m_Addr.sin_port = htons(wPort);
}

void Socket::setSocketType(USHORT wFamilySocket)
{
	m_Addr.sin_family = wFamilySocket;
}

void Socket::setConnection(int nSocketType, int nType, int nProtocol)
{
	m_Connection = socket(nSocketType, nType, nProtocol);
}

void Socket::delete_descriptor() {
	closesocket(m_Connection);
	WSACleanup();
}

int Socket::Connect()
{
	return connect(m_Connection, (SOCKADDR*)&m_Addr, sizeof(m_Addr));
}

int Socket::Recv(char* buf, int length)
{
	return recv(m_Connection, buf, length, NULL);
}

int Socket::Send(std::vector<unsigned char> packet)
{
	return send(m_Connection, (char*)&packet[0], packet.size(), NULL);
} 