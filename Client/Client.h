#pragma once

class Scene;

class Client : public Game
{
public:
	Client();

	virtual bool Init() override;
	virtual void Run() override;
	virtual void Shutdown() override;

	int GetScreenWidth() const { return mScreenWidth; }
	int GetScreenHeight() const { return mScreenHeight; }

	Entity CreateEntity();

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

	int mScreenWidth;
	int mScreenHeight;
};

