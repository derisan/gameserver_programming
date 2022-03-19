#include "ServerPCH.h"
#include "Server.h"

bool Server::Init()
{
	SocketUtil::Init();

	initGameWorld();

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

Entity Server::createEntity()
{
	Entity e(GetRegistry().create(), this);
	auto& id = e.AddComponent<IDComponent>();

	RegisterEntity(id.ID, e);

	return e;
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

void Server::initGameWorld()
{
	Entity board = createEntity();
	board.AddComponent<TransformComponent>();

	Entity piece = createEntity();
	piece.AddComponent<TransformComponent>();
}
