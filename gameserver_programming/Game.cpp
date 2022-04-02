#include "PCH.h"
#include "Game.h"

void Game::RegisterEntity(const GSID& id, const entt::entity e)
{
	auto iter = mEntities.find(id);

	if (iter != mEntities.end())
	{
		GS_LOG("Entity ID[{0}] already registered!", id);
	}
	else
	{
		mEntities.try_emplace(id, e);
	}
}

void Game::RemoveEntity(const GSID& id)
{
	auto iter = mEntities.find(id);

	if (iter != mEntities.end())
	{
		mRegistry.destroy(iter->second);
		mEntities.erase(iter);
	}
	else
	{
		GS_LOG("Entity ID[{0}] does not exist!", id);
	}
}

entt::entity Game::GetEntityByID(const GSID& id) const
{
	auto iter = mEntities.find(id);

	if (iter != mEntities.end())
	{
		return iter->second;
	}
	else
	{
		GS_LOG("Entity[{0}] does not exist.", id);
		return entt::null;
	}
}
