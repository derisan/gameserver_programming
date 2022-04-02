#include "ClientPCH.h"
#include "GameScene.h"

#include "gameserver_programming/PacketType.h"

#include "Client.h"

GameScene GameScene::sInstance;
extern string gServerAddr;

void GameScene::StaticCreate(Client* client)
{
	sInstance = GameScene(client);
}

GameScene* GameScene::Get()
{
	return &sInstance;
}

void GameScene::Enter()
{
	GS_LOG("GameScene::Enter");

	mClientSocket = SocketUtil::CreateTCPSocket();

	SocketAddress serveraddr(gServerAddr, SERVER_PORT);

	int retVal = mClientSocket->Connect(serveraddr);

	if (retVal == SOCKET_ERROR)
	{
		GS_ASSERT(false, "ASSERTION FAILED");
	}

	// 논블로킹 소켓으로 만듦
	mClientSocket->SetNonBlockingMode(1);

	// LoginRequest 패킷 송신
	MemoryStream packet;
	packet.WriteInt(static_cast<int32>(CtsPacket::eLoginRequest));
	mClientSocket->Send(&packet, sizeof(packet));

	// 체스보드판 생성
	{
		Entity board = mOwner->CreateEntityWithID(0);
		board.AddTag<ChessBoard>();
		board.AddComponent<TransformComponent>();
		auto& spriteRenderer = board.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/board.png"), 10);
		spriteRenderer.Width = SCREEN_WIDTH;
		spriteRenderer.Height = SCREEN_HEIGHT;
	}
}

void GameScene::Exit()
{
	GS_LOG("GameScene::Exit");

	mClientSocket = nullptr;

	mOwner->GetRegistry().clear();
}

void GameScene::ProcessInput()
{
	if (!mbLoginConfirmed)
	{
		return;
	}

	Vector2 direction = Vector2::Zero;
	bool isPressed = false;

	if (Input::IsButtonPressed(SDL_SCANCODE_LEFT))
	{
		isPressed = true;
		direction.x -= 1.0f;
	}

	if (Input::IsButtonPressed(SDL_SCANCODE_RIGHT))
	{
		isPressed = true;
		direction.x += 1.0f;
	}

	if (Input::IsButtonPressed(SDL_SCANCODE_UP))
	{
		isPressed = true;
		direction.y -= 1.0f;
	}

	if (Input::IsButtonPressed(SDL_SCANCODE_DOWN))
	{
		isPressed = true;
		direction.y += 1.0f;
	}

	if (isPressed && mMyPiece)
	{
		MemoryStream packet;

		auto& id = mMyPiece.GetComponent<IDComponent>();

		packet.WriteInt(static_cast<int32>(CtsPacket::eUserInput));
		packet.WriteUInt64(id.ID);
		packet.WriteVector2(direction);

		mClientSocket->Send(&packet, sizeof(packet));
	}
}

void GameScene::Update(float deltaTime)
{
	MemoryStream packet;
	int retVal = mClientSocket->Recv(&packet, sizeof(packet));

	if (retVal == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == WSAEWOULDBLOCK)
		{
			return;
		}
		else
		{
			SocketUtil::ReportError(L"GameScene::Update", error);
		}
	}
	else if (retVal > 0)
	{
		processPacket(&packet);
	}
	else
	{
		GS_LOG("Server down.");
		mOwner->SetRunning(false);
	}
}

void GameScene::Render(SDL_Renderer* renderer)
{
	(mOwner->GetRegistry()).sort<SpriteRendererComponent>([](const auto& lhs, const auto& rhs) {
		return lhs.DrawOrder < rhs.DrawOrder;
		});

	auto view = (mOwner->GetRegistry()).view<SpriteRendererComponent>();

	for (auto entity : view)
	{
		Entity e = Entity(entity, mOwner);

		auto& transform = e.GetComponent<TransformComponent>();
		auto& spriteRenderer = e.GetComponent<SpriteRendererComponent>();

		Systems::DrawSprite(transform.Position, transform.Rotation, transform.Scale,
			spriteRenderer.Texture, spriteRenderer.Width, spriteRenderer.Height, renderer);
	}
}

GameScene::GameScene(Client* client)
	: Scene(client)
{

}

void GameScene::processPacket(MemoryStream* outPacket)
{
	uint16 totalLen = outPacket->GetLength();
	outPacket->SetLength(0);

	while (outPacket->GetLength() < totalLen)
	{
		StCPacket pType;
		outPacket->ReadInt(reinterpret_cast<int32*>(&pType));

		switch (pType)
		{
		case StCPacket::eCreatePiece:
			processCreatePiece(outPacket);
			break;

		case StCPacket::eUpdatePosition:
			processUpdatePosition(outPacket);
			break;

		case StCPacket::eLoginConfirmed:
			processLoginConfirmed(outPacket);
			break;

		default:
			GS_LOG("Unknown packet type!");
			break;
		}
	}
}

void GameScene::processCreatePiece(MemoryStream* outPacket)
{
	int32 clientID = -1;
	outPacket->ReadInt(&clientID);

	uint64 id = 0;
	outPacket->ReadUInt64(&id);
	
	Entity e = mOwner->CreateEntityWithID(id);

	if (clientID == mOwner->GetClientID())
	{
		mMyPiece = e;
	}

	e.AddTag<ChessPiece>();

	Vector2 position = Vector2::Zero;
	outPacket->ReadVector2(&position);
	auto& transform = e.AddComponent<TransformComponent>();
	transform.Position = position;

	auto& spriteRenderer = e.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/knight.png"));
	spriteRenderer.Width = SCREEN_WIDTH / 8;
	spriteRenderer.Height = SCREEN_HEIGHT / 8;
}

void GameScene::processUpdatePosition(MemoryStream* outPacket)
{
	uint64 id = 0;
	outPacket->ReadUInt64(&id);

	Vector2 position = Vector2::Zero;
	outPacket->ReadVector2(&position);

	auto entity = mOwner->GetEntityByID(id);

	if (entity == entt::null)
	{
		GS_LOG("Entity does not exist!");
		mOwner->SetRunning(false);
		return;
	}

	Entity piece = Entity(entity, mOwner);
	auto& transform = piece.GetComponent<TransformComponent>();
	transform.Position = position;
}

void GameScene::processLoginConfirmed(MemoryStream* outPacket)
{
	int myClientID = -1;
	outPacket->ReadInt(&myClientID);
	mOwner->SetClientID(myClientID);

	GS_LOG("my client id: {0}", myClientID);

	mbLoginConfirmed = true;
}

