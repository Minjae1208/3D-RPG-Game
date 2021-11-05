#include "Server/Server.h"

int main()
{
	Server IOCP;
	if (IOCP.Init())
	{
		IOCP.Start();
	}

	return 0;
}