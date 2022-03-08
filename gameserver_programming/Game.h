#pragma once

class Game
{
	friend class Entity;

public:
	virtual bool Init() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;

	entt::registry& GetRegistry() { return mRegistry; }

private:
	entt::registry mRegistry;
};

