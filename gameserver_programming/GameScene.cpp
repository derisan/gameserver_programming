#include "PCH.h"
#include "GameScene.h"

#include "Client.h"
#include "Components.h"
#include "Systems.h"
#include "TextureManager.h"

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

	mAsteroid = Entity(mOwner->GetRegistry().create(), mOwner);
	mAsteroid.AddComponent<TransformComponent>();
	mAsteroid.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/Asteroid.png"));
}

void GameScene::Exit()
{
	GS_LOG("GameScene::Exit");
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

		Systems::DrawSprite(transform.Position, transform.Rotation, transform.Scale,
			spriteRenderer.Texture, spriteRenderer.Width, spriteRenderer.Height, renderer);
	}
}

GameScene::GameScene(Client* client)
	: Scene(client)
{

}


