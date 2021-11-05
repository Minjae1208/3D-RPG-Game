#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WInSock2.h>
#include <map>

#define MAX_BUFFER 4096
#define SERVER_PORT 4000
#define MAX_CLIENTS 20

struct stOver
{
	WSAOVERLAPPED overlapped;
	WSABUF WSAbuf;
	SOCKET socket;
	char buf[MAX_BUFFER];
};

struct stProcess
{
	void(*Process)(std::stringstream &RecvStream, stOver* pSocket);

	stProcess()
	{
		Process = nullptr;
	}
};