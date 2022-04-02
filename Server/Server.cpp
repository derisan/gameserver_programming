#include "ServerPCH.h"
#include "Server.h"

#include "gameserver_programming/PacketType.h"

#include "ServerGlobal.h"

void CALLBACK RecvCallback(DWORD err, DWORD numBytes, LPWSAOVERLAPPED over, DWORD flags)
{
	int clientID = gOverToID[over];

	if (numBytes == 0)
	{
		GS_LOG("Client {0} disconnected!", clientID);
		gOverToID.erase(over);
		gIdToSession.erase(clientID);
		return;
	}

	auto& session = gIdToSession[clientID];
	auto& packet = session.GetMess();
	gServer->ProcessPacket(&packet, session);

	session.DoRecv();
}

void CALLBACK SendCallback(DWORD err, DWORD numBytes, LPWSAOVERLAPPED over, DWORD flags)
{
	SendData* data = reinterpret_cast<SendData*>(over);
	delete data;
}

bool Server::Init()
{
	SocketUtil::Init();

	mListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	bind(mListenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr));
	listen(mListenSocket, SOMAXCONN);

	return true;
}

void Server::Run()
{
	SOCKADDR_IN clientAddr;
	int addrsize = sizeof(clientAddr);
	int clientID = 0;

	while (true)
	{
		SOCKET client = WSAAccept(mListenSocket, reinterpret_cast<sockaddr*>(&clientAddr),
			&addrsize, 0, 0);
		gIdToSession.try_emplace(clientID, clientID, client);
		gIdToSession[clientID].DoRecv();

		GS_LOG("Client {0} connected.", clientID);
		clientID++;
	}
}

void Server::Shutdown()
{
	closesocket(mListenSocket);

	SocketUtil::Shutdown();
}

Entity Server::createEntity()
{
	Entity e(GetRegistry().create(), this);
	auto& id = e.AddComponent<IDComponent>();
	RegisterEntity(id.ID, e);

	return e;
}

void Server::ProcessPacket(MemoryStream* packet, Session& session)
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
			processUserInput(packet, session);
			break;

		case CtsPacket::eLoginRequest:
			processLoginRequest(packet, session);
			break;

		default:
			GS_LOG("Unknown packet type!");
			break;
		}
	}
}

void Server::processUserInput(MemoryStream* packet, Session& session)
{
	uint64 id = -1;
	packet->ReadUInt64(&id);

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

	GS_LOG("클라이언트[{0}] 말의 위치: {1} {2}", session.GetClientID(), GetChessBoardIndex(transform.Position.x), 
		GetChessBoardIndex(transform.Position.y));

	MemoryStream spacket;
	spacket.WriteInt(static_cast<int32>(StCPacket::eUpdatePosition));
	spacket.WriteUInt64(id);
	spacket.WriteVector2(transform.Position);

	for (auto& [_, session] : gIdToSession)
	{
		session.DoSend(spacket);
	}
}

void Server::processLoginRequest(MemoryStream* packet, Session& session)
{
	MemoryStream spacket;

	spacket.WriteInt(static_cast<int32>(StCPacket::eLoginConfirmed));
	spacket.WriteInt(session.GetClientID());

	Entity piece = createEntity();
	auto& id = piece.GetComponent<IDComponent>();
	auto& transform = piece.AddComponent<TransformComponent>();

	spacket.WriteInt(static_cast<int32>(StCPacket::eCreatePiece));
	spacket.WriteInt(session.GetClientID());
	spacket.WriteUInt64(id.ID);
	spacket.WriteVector2(transform.Position);

	session.DoSend(spacket);
}

int GetChessBoardIndex(int position)
{
	int idx = (position / (SCREEN_WIDTH >> 3)) + 1;
	return idx;
}
