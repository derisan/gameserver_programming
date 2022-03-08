#include "PCH.h"
#include "Client.h"

#include "GameScene.h"
#include "TextureManager.h"

Client::Client()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mActiveScene(nullptr)
{

}

bool Client::Init()
{
	bool success = createWindow("GameServerProgramming", 640, 480);

	if (!success)
	{
		GS_ASSERT(false, "Failed to create SDL window.");
		return false;
	}

	success = createScene();

	if (!success)
	{
		GS_ASSERT(false, "Failed to create intial scene.");
		return false;
	}

	TextureManager::Init(mRenderer);

	mTicksCount = SDL_GetTicks();

	mActiveScene->Enter();

	return true;
}

void Client::Run()
{
	while (mbRunning)
	{
		processInput();
		update();
		render();
	}
}

void Client::Shutdown()
{
	if (mActiveScene)
	{
		mActiveScene->Exit();
	}

	SDL_Quit();
}

bool Client::createWindow(const string& title, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		GS_LOG("Unable to initialize SDL: {0}", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		GS_LOG("Unable to initialize SDL_image: {0}", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(title.c_str(), 100, 100, width, height, 0);
	if (!mWindow)
	{
		GS_LOG("Failed to create window: {0}", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		GS_LOG("Failed to create renderer: {0}", SDL_GetError());
		return false;
	}

	return true;
}

bool Client::createScene()
{
	GameScene::StaticCreate(this);
	mActiveScene = GameScene::Get();

	return true;
}

void Client::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mbRunning = false;
			break;
		}
	}

	const uint8_t* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_ESCAPE])
	{
		mbRunning = false;
	}

	if (mActiveScene)
	{
		mActiveScene->ProcessInput(keystate);
	}
}

void Client::update()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();
	
	if (mActiveScene)
	{
		mActiveScene->Update(deltaTime);
	}
}

void Client::render()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	if (mActiveScene)
	{
		mActiveScene->Render(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}
