#include "ServerPCH.h"
#include "Server.h"


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
		MemoryStream buffer;

		int retVal = mClientSocket->Recv(&buffer, sizeof(buffer));

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

		uint16 totalLen = buffer.GetLength();
		buffer.SetLength(0);

		uint64 id = 0;
		buffer.ReadUInt64(&id);

		int cls = 0;
		buffer.ReadInt(&cls);

		Vector2 direction = Vector2::Zero;
		buffer.ReadVector2(&direction);

		auto entity = GetEntityByID(id);

		if (entity == entt::null)
		{
			GS_ASSERT(false, "Entity does not exist!");
			break;
		}

		Entity e = Entity(entity, this);
		auto& transform = e.GetComponent<TransformComponent>();

		Systems::Move(&transform.Position, (SCREEN_WIDTH / 8) * direction);
		Systems::ClampPosition(&transform.Position, SCREEN_WIDTH - kPieceWidth, SCREEN_HEIGHT - kPieceHeight);

		buffer.Reset();
		buffer.WriteUInt64(id);
		buffer.WriteInt(cls);
		buffer.WriteVector2(transform.Position);

		GS_LOG("말의 위치: {0} {1}", GetChessBoardIndex(static_cast<int>(transform.Position.x)), 
			GetChessBoardIndex(static_cast<int>(transform.Position.y)));

		mClientSocket->Send(&buffer, sizeof(buffer));
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
	MemoryStream buffer;
	{
		Entity board = createEntity();
		auto& transform = board.AddComponent<TransformComponent>();
		auto& id = board.GetComponent<IDComponent>();

		buffer.WriteUInt64(id.ID);
		buffer.WriteInt('BORD');
		buffer.WriteVector2(transform.Position);
	}

	{
		Entity piece = createEntity();
		auto& transform = piece.AddComponent<TransformComponent>();
		auto& id = piece.GetComponent<IDComponent>();

		buffer.WriteUInt64(id.ID);
		buffer.WriteInt('PIEC');
		buffer.WriteVector2(transform.Position);
	}

	mClientSocket->Send(&buffer, sizeof(MemoryStream));
}

int GetChessBoardIndex(int position)
{
	int idx = (position / (SCREEN_WIDTH >> 3)) + 1;
	return idx;
}
