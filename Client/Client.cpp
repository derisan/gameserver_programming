#include "ClientPCH.h"
#include "Client.h"

#include "GameScene.h"

Client::Client()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mActiveScene(nullptr)
	, mScreenWidth(0)
	, mScreenHeight(0)
{

}

bool Client::Init()
{
	SocketUtil::Init();
	Input::Init();

	bool success = createWindow("GameServerProgramming", SCREEN_WIDTH, SCREEN_HEIGHT);

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

	TextureManager::Shutdown();

	SDL_Quit();

	SocketUtil::Shutdown();
}

Entity Client::CreateEntityWithID(uint64 id)
{
	Entity e = Entity(GetRegistry().create(), this);
	auto& idComp = e.AddComponent<IDComponent>(id);
	RegisterEntity(idComp.ID, e);

	return e;
}

bool Client::createWindow(const string& title, int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		GS_LOG("Unable to initialize SDL: {0}", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		GS_LOG("Unable to initialize SDL_image: {0}", IMG_GetError());
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

	Input::Update(keystate);

	if (keystate[SDL_SCANCODE_ESCAPE])
	{
		mbRunning = false;
	}

	if (mActiveScene)
	{
		mActiveScene->ProcessInput();
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
