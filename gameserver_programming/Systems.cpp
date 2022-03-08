#include "PCH.h"
#include "Systems.h"

void Systems::DrawSprite(const Vector2& position, const float rotation, const float scale, SDL_Texture* texture, int texWidth, int texHeight, SDL_Renderer* renderer)
{
	SDL_Rect r;
	r.w = static_cast<int>(texWidth * scale);
	r.h = static_cast<int>(texHeight * scale);
	r.x = static_cast<int>(position.x - r.w / 2);
	r.y = static_cast<int>(position.y - r.h / 2);

	SDL_RenderCopyEx(renderer, texture, nullptr, &r, -rotation, nullptr, SDL_FLIP_NONE);
}
