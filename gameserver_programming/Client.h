#pragma once

class Client
{
public:
	Client();

	bool Init();
	void Run();
	void Shutdown();

private:
	bool createWindow(const string& title, int width, int height);

	void processInput();
	void update();
	void render();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	uint32 mTicksCount;

	bool mbRunning;
};

