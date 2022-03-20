#pragma once

#include "GSID.h"

struct TransformComponent
{
	TransformComponent(float x = 0.0f, float y = 0.0f, float rotation = 0.0f, float scale = 1.0f);
	TransformComponent(const Vector2& position, float rotation = 0.0f, float scale = 1.0f);

	Vector2 Position;
	float Rotation;
	float Scale;
};

struct SpriteRendererComponent
{
	SpriteRendererComponent(SDL_Texture* texture = nullptr, int drawOrder = 100);

	SDL_Texture* Texture;
	int Width;
	int Height;
	int DrawOrder;
};

struct IDComponent
{
	IDComponent();
	IDComponent(uint64 id);

	GSID ID;
};