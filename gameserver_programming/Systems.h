#pragma once

class Systems
{
public:
	static void DrawSprite(const Vector2& position, const float rotation, const float scale,
		SDL_Texture* texture, int texWidth, int texHeight, SDL_Renderer* renderer);
};


