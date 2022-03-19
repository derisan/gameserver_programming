#include "ClientPCH.h"
#include "GameScene.h"

#include "Client.h"

GameScene GameScene::sInstance;

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

	SocketAddress serveraddr("127.0.0.1", SERVER_PORT);

	int retVal = mClientSocket->Connect(serveraddr);

	if (retVal == SOCKET_ERROR)
	{
		GS_ASSERT(false, "ASSERTION FAILED");
	}

	mClientSocket->SetNonBlockingMode(1);

	{
		Entity board = mOwner->CreateEntity();
		auto& id = board.AddComponent<IDComponent>();
		GS_LOG("BOARD ID: {0}", id.ID);
		auto& transform = board.AddComponent<TransformComponent>();
		auto& spriteRenderer = board.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/board.png"), 10);
		spriteRenderer.Width = mOwner->GetScreenWidth();
		spriteRenderer.Height = mOwner->GetScreenHeight();
		board.AddTag<ChessBoard>();
	}

	{
		mPiece = mOwner->CreateEntity();
		auto& id = mPiece.AddComponent<IDComponent>();
		GS_LOG("PIECE ID: {0}", id.ID);
		auto& spriteRenderer = mPiece.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/knight.png"));
		spriteRenderer.Width = mOwner->GetScreenWidth() / 8;
		spriteRenderer.Height = mOwner->GetScreenHeight() / 8;
		auto& transform = mPiece.AddComponent<TransformComponent>();
		mPiece.AddTag<ChessPiece>();
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
	if (Input::IsButtonPressed(SDL_SCANCODE_LEFT))
	{
		auto& transform = mPiece.GetComponent<TransformComponent>();

		Systems::Move(&transform.Position, Vector2(-mOwner->GetScreenWidth() / 8.0f, 0.0f));
	}

	if (Input::IsButtonPressed(SDL_SCANCODE_RIGHT))
	{
		auto& transform = mPiece.GetComponent<TransformComponent>();

		Systems::Move(&transform.Position, Vector2(mOwner->GetScreenWidth() / 8.0f, 0.0f));
	}

	if (Input::IsButtonPressed(SDL_SCANCODE_UP))
	{
		auto& transform = mPiece.GetComponent<TransformComponent>();

		Systems::Move(&transform.Position, Vector2(0.0f, -mOwner->GetScreenHeight() / 8.0f));
	}

	if (Input::IsButtonPressed(SDL_SCANCODE_DOWN))
	{
		auto& transform = mPiece.GetComponent<TransformComponent>();

		Systems::Move(&transform.Position, Vector2(0.0f, mOwner->GetScreenHeight() / 8.0f));
	}
}

void GameScene::Update(float deltaTime)
{
	auto view = (mOwner->GetRegistry()).view<ChessPiece>();

	const int screenWidth = mOwner->GetScreenWidth();
	const int screenHeight = mOwner->GetScreenHeight();

	for (auto entity : view)
	{
		Entity e = Entity(entity, mOwner);

		auto& transform = e.GetComponent<TransformComponent>();
		auto& spriteRenderer = e.GetComponent<SpriteRendererComponent>();
		
		Systems::ClampPosition(&transform.Position, screenWidth - spriteRenderer.Width, screenHeight - spriteRenderer.Height);
	}

	MemoryStream buffer;
	mClientSocket->Recv(&buffer, sizeof(buffer));

	if (buffer.GetLength() > 0)
	{
		buffer.SetLength(0);

		uint64 id = 0;
		buffer.ReadUInt64(&id);

		int cls = 0;
		buffer.ReadInt(&cls);

		Vector2 position;
		buffer.ReadVector2(&position);

		GS_LOG("ID: {0}", id);
		GS_LOG("CLASS: {0}", cls);
		GS_LOG("POSITION: {0} {1}", position.x, position.y);
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


