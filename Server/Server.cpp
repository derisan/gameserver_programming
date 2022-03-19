#include "ServerPCH.h"
#include "Server.h"

bool Server::Init()
{
	SocketUtil::Init();

	waitPlayer();

	return true;
}

void Server::Run()
{
	while (true)
	{
		
	}
}

void Server::Shutdown()
{
	mClientSocket = nullptr;

	SocketUtil::Shutdown();
}

void Server::waitPlayer()
{
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket();
	SocketAddress serveraddr(SERVER_PORT);

	int retVal = listenSocket->Bind(serveraddr);

	if (retVal == SOCKET_ERROR)
	{
		GS_ASSERT(false, "ASSERTION FAILED");
	}

	retVal = listenSocket->Listen();

	if (retVal == SOCKET_ERROR)
	{
		GS_ASSERT(false, "ASSERTION FAILED");
	}

	SocketAddress clientAddr;
	int clientNum = 0;

	mClientSocket = listenSocket->Accept(&clientAddr);

	GS_LOG("Client Connected: {0}", clientAddr.ToString());
}
