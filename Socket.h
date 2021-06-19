#pragma once
#include <winsock.h>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

class Socket
{
public:
	Socket();

	void setPort(USHORT wPort);
	void setServerIP(LPCSTR lpszHostIP);
	void setSocketType(USHORT wFamilySocket);
	void setConnection(int nSocketType, int nType, int nProtocol);
	void delete_descriptor();

	int  Connect();
	int  Recv(char* buf, int length);
	int  Send(std::vector<unsigned char> packet);
private:
	SOCKADDR_IN  m_Addr;
	SOCKET       m_Connection;
};

