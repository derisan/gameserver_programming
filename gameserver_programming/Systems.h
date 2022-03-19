#pragma once

class Systems
{
public:
	static void DrawSprite(const Vector2& position, const float rotation, const float scale,
		SDL_Texture* texture, int texWidth, int texHeight, SDL_Renderer* renderer);

	static void Move(Vector2* from, const Vector2& dist);

	static void ClampPosition(Vector2* outPosition, int borderX, int borderY);
};


