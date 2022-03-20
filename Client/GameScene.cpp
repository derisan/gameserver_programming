#include "ClientPCH.h"
#include "GameScene.h"

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

	mClientSocket->SetNonBlockingMode(1);
}

void GameScene::Exit()
{
	GS_LOG("GameScene::Exit");

	mClientSocket = nullptr;

	mOwner->GetRegistry().clear();
}

void GameScene::ProcessInput()
{
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

	if (isPressed)
	{
		MemoryStream packet;

		auto& id = mMyPiece.GetComponent<IDComponent>();

		packet.WriteUInt64(id.ID);
		packet.WriteInt('PIEC');
		packet.WriteVector2(direction);

		mClientSocket->Send(&packet, sizeof(packet));
	}
}

void GameScene::Update(float deltaTime)
{
	MemoryStream packet;
	mClientSocket->Recv(&packet, sizeof(packet));

	if (packet.GetLength() > 0)
	{
		processPacket(&packet);
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
		uint64 id = 0;
		outPacket->ReadUInt64(&id);

		int cls = 0;
		outPacket->ReadInt(&cls);

		Vector2 position = Vector2::Zero;
		outPacket->ReadVector2(&position);

		auto entity = mOwner->GetEntityByID(id);

		Entity e;
		if (entity == entt::null)
		{
			e = Entity(mOwner->GetRegistry().create(), mOwner);
			auto& idComp = e.AddComponent<IDComponent>(id);
			mOwner->RegisterEntity(idComp.ID, e);

			switch (cls)
			{
			case 'BORD':
			{
				e.AddTag<ChessBoard>();
				e.AddComponent<TransformComponent>();
				auto& spriteRenderer = e.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/board.png"), 10);
				spriteRenderer.Width = SCREEN_WIDTH;
				spriteRenderer.Height = SCREEN_HEIGHT;
			}
				break;

			case 'PIEC':
			{
				e.AddTag<ChessPiece>();
				e.AddComponent<TransformComponent>();
				auto& spriteRenderer = e.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/knight.png"));
				spriteRenderer.Width = SCREEN_WIDTH / 8;
				spriteRenderer.Height = SCREEN_HEIGHT / 8;
				mMyPiece = e;
			}
				break;

			default:
				GS_LOG("Unknown class type!");
				break;
			}
		}
		else
		{
			e = Entity(entity, mOwner);
		}

		auto& transform = e.GetComponent<TransformComponent>();
		Systems::MoveTo(&transform.Position, position);
	}
}

