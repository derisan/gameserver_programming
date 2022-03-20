#pragma once

#include "Game.h"

class Entity
{
public:
	Entity(entt::entity handle = entt::null, Game* game = nullptr)
		: mEntityHandle(handle)
		, mGame(game) {}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		GS_ASSERT(!HasComponent<T>(), "Entity already has component.");
		T& component = mGame->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	void AddTag()
	{
		GS_ASSERT(!HasComponent<T>(), "Entity already has tag.");
		mGame->mRegistry.emplace<T>(mEntityHandle);
	}

	template<typename T>
	T& GetComponent()
	{
		GS_ASSERT(HasComponent<T>(), "Entity does not have component.");
		T& component = mGame->mRegistry.get<T>(mEntityHandle);
		return component;
	}

	template<typename T>
	bool HasComponent()
	{
		return mGame->mRegistry.any_of<T>(mEntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		GS_ASSERT(HasComponent<T>(), "Entity does not have component.");
		mGame->mRegistry.remove<T>(mEntityHandle);
	}

	operator entt::entity() const { return mEntityHandle; }
	operator entt::entity() { return mEntityHandle; }
	operator bool() const { return (mEntityHandle != entt::null) && mGame; }

	bool operator==(const Entity& other) const
	{
		return mEntityHandle == other.mEntityHandle;
	}

	bool operator!= (const Entity & other) const
	{
		return !(*this == other);
	}

private:
	entt::entity mEntityHandle;
	Game* mGame;
};

