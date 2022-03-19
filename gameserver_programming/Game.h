#pragma once

#include "GSID.h"

class Game
{
	friend class Entity;

public:
	virtual bool Init() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;

	entt::registry& GetRegistry() { return mRegistry; }

	void RegisterEntity(const GSID& id, const entt::entity e);

	void RemoveEntity(const GSID& id);

	uint32 GetNumEntites() const { return mEntities.size(); }

private:
	entt::registry mRegistry;

	unordered_map<GSID, entt::entity> mEntities;
};

