#include "PCH.h"
#include "GameScene.h"

#include "Client.h"
#include "Components.h"
#include "Systems.h"
#include "TextureManager.h"
#include "Tags.h"

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

	{
		Entity board = mOwner->CreateEntity();
		auto& transform = board.AddComponent<TransformComponent>();
		transform.Position = Vector2(mOwner->GetScreenWidth() / 2.0f, mOwner->GetScreenHeight() / 2.0f);
		auto& spriteRenderer = board.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/board.png"), 10);
		spriteRenderer.Width = mOwner->GetScreenWidth();
		spriteRenderer.Height = mOwner->GetScreenHeight();
		board.AddTag<ChessBoard>();
	}

	{
		Entity piece = mOwner->CreateEntity();
		auto& transform = piece.AddComponent<TransformComponent>();
		transform.Position = Vector2(40.0f, 40.0f);
		auto& spriteRenderer = piece.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/knight.png"));
		spriteRenderer.Width = mOwner->GetScreenWidth() / 8;
		spriteRenderer.Height = mOwner->GetScreenHeight() / 8;
		piece.AddTag<ChessPiece>();
	}
}

void GameScene::Exit()
{
	GS_LOG("GameScene::Exit");

	mOwner->GetRegistry().clear();
}

void GameScene::ProcessInput(const uint8* keystate)
{
	
}

void GameScene::Update(float deltaTime)
{

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


