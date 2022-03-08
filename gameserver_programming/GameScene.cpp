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

	//Entity board = Entity(mOwner->GetRegistry().create(), mOwner);
	Entity board = mOwner->CreateEntity();
	auto& transform = board.AddComponent<TransformComponent>();
	transform.Position = Vector2(mOwner->GetScreenWidth() / 2.0f, mOwner->GetScreenHeight() / 2.0f);
	board.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/board.png"));
	board.AddTag<Board>();
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
	auto view = (mOwner->GetRegistry()).view<SpriteRendererComponent>();

	for (auto entity : view)
	{
		Entity e = Entity(entity, mOwner);

		auto& transform = e.GetComponent<TransformComponent>();
		auto& spriteRenderer = e.GetComponent<SpriteRendererComponent>();

		int texWidth = spriteRenderer.Width;
		int texHeight = spriteRenderer.Height;

		if (e.HasComponent<Board>())
		{
			texWidth = mOwner->GetScreenWidth();
			texHeight = mOwner->GetScreenHeight();
		}

		Systems::DrawSprite(transform.Position, transform.Rotation, transform.Scale,
			spriteRenderer.Texture, texWidth, texHeight, renderer);
	}
}

GameScene::GameScene(Client* client)
	: Scene(client)
{

}


