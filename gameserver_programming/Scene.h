#pragma once

class Client;

class Scene
{
public:
	Scene(Client* owner)
		: mOwner(owner) {}
	virtual ~Scene() = default;

	virtual void Enter() {}
	virtual void Exit() {}
	virtual void ProcessInput() {}
	virtual void Update(float deltaTime) {}
	virtual void Render(SDL_Renderer* renderer) {}

protected:
	Client* mOwner;
};

