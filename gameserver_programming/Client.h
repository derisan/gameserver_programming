#pragma once

#include "Game.h"

class Scene;

class Client : public Game
{
public:
	Client();

	virtual bool Init() override;
	virtual void Run() override;
	virtual void Shutdown() override;

private:
	bool createWindow(const string& title, int width, int height);
	bool createScene();

	void processInput();
	void update();
	void render();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	uint32 mTicksCount;

	bool mbRunning;

	Scene* mActiveScene;
};

