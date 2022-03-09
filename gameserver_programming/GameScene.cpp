#include "PCH.h"
#include "GameScene.h"

#include "Client.h"
#include "Components.h"
#include "Systems.h"
#include "TextureManager.h"
#include "Tags.h"
#include "Input.h"

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
		mPiece = mOwner->CreateEntity();
		auto& spriteRenderer = mPiece.AddComponent<SpriteRendererComponent>(TextureManager::GetTexture("../Assets/knight.png"));
		spriteRenderer.Width = mOwner->GetScreenWidth() / 8;
		spriteRenderer.Height = mOwner->GetScreenHeight() / 8;
		auto& transform = mPiece.AddComponent<TransformComponent>();
		transform.Position = Vector2(spriteRenderer.Width / 2.0f, spriteRenderer.Height / 2.0f);
		mChessPieceOffset = spriteRenderer.Width / 2;
		mPiece.AddTag<ChessPiece>();
	}
}

void GameScene::Exit()
{
	GS_LOG("GameScene::Exit");

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
	auto view = (mOwner->GetRegistry()).view<TransformComponent>();

	const int screenWidth = mOwner->GetScreenWidth();
	const int screenHeight = mOwner->GetScreenHeight();

	for (auto entity : view)
	{
		auto& transform = view.get<TransformComponent>(entity);

		Systems::Reposition(&transform.Position, screenWidth, screenHeight, mChessPieceOffset);
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
	, mChessPieceOffset(0)
{

}


