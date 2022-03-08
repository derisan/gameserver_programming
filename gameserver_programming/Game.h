#pragma once

class Game
{
	friend class Entity;

public:
	virtual bool Init() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;

private:
	entt::registry mRegistry;
};

