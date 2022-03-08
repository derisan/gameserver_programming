#include "PCH.h"
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

}

GameScene::GameScene(Client* client)
	: Scene(client)
{

}


