#include "ServerPCH.h"
#include "Server.h"

#include "gameserver_programming/PacketType.h"

bool Server::Init()
{
	SocketUtil::Init();

	waitPlayer();
	initGameWorld();

	return true;
}

void Server::Run()
{
	while (true)
	{
		MemoryStream packet;

		int retVal = mClientSocket->Recv(&packet, sizeof(packet));

		if (retVal == 0)
		{
			GS_LOG("Client disconnected!");
			break;
		}
		else if (retVal == SOCKET_ERROR)
		{
			GS_LOG("Recv error");
			break;
		}
		else
		{
			processPacket(&packet);
		}
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
		GS_ASSERT(false, "");
	}

	retVal = listenSocket->Listen();

	if (retVal == SOCKET_ERROR)
	{
		GS_ASSERT(false, "");
	}

	SocketAddress clientAddr;
	int clientNum = 0;

	mClientSocket = listenSocket->Accept(&clientAddr);

	GS_LOG("Client Connected: {0}", clientAddr.ToString());
}

void Server::initGameWorld()
{
	MemoryStream packet;

	{
		Entity piece = createEntity();
		auto& id = piece.GetComponent<IDComponent>();
		auto& transform = piece.AddComponent<TransformComponent>();

		packet.WriteInt(static_cast<int32>(StCPacket::eCreatePiece));
		packet.WriteUInt64(id.ID);
		packet.WriteVector2(transform.Position);
	}

	mClientSocket->Send(&packet, sizeof(MemoryStream));
}

void Server::processPacket(MemoryStream* packet)
{
	uint16 totalLen = packet->GetLength();
	packet->SetLength(0);

	while (packet->GetLength() < totalLen)
	{
		CtsPacket pType;
		packet->ReadInt(reinterpret_cast<int32*>(&pType));

		switch (pType)
		{
		case CtsPacket::eUserInput:
			processUserInput(packet);
			break;

		default:
			GS_LOG("Unknown packet type!");
			break;
		}
	}
}

void Server::processUserInput(MemoryStream* packet)
{
	int64 id = -1;
	packet->ReadInt64(&id);

	auto entity = GetEntityByID(id);

	if (entity == entt::null)
	{
		GS_ASSERT(false, "Entity does not exist!");
	}

	Vector2 direction = Vector2::Zero;
	packet->ReadVector2(&direction);

	Entity piece = Entity(entity, this);
	auto& transform = piece.GetComponent<TransformComponent>();

	Systems::Move(&transform.Position, (SCREEN_WIDTH / 8) * direction);
	Systems::ClampPosition(&transform.Position, SCREEN_WIDTH - PIECE_WIDTH, SCREEN_HEIGHT - PIECE_HEIGHT);

	GS_LOG("말의 위치: {0} {1}", GetChessBoardIndex(transform.Position.x), 
		GetChessBoardIndex(transform.Position.y));

	MemoryStream spacket;
	spacket.WriteInt(static_cast<int32>(StCPacket::eUpdatePosition));
	spacket.WriteInt64(id);
	spacket.WriteVector2(transform.Position);

	mClientSocket->Send(&spacket, sizeof(MemoryStream));
}

int GetChessBoardIndex(int position)
{
	int idx = (position / (SCREEN_WIDTH >> 3)) + 1;
	return idx;
}
