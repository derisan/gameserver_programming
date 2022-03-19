#include "PCH.h"
#include "Systems.h"

void Systems::DrawSprite(const Vector2& position, const float rotation, const float scale, SDL_Texture* texture, int texWidth, int texHeight, SDL_Renderer* renderer)
{
	SDL_Rect r;
	r.w = static_cast<int>(texWidth * scale);
	r.h = static_cast<int>(texHeight * scale);
	r.x = static_cast<int>(position.x);
	r.y = static_cast<int>(position.y);

	SDL_RenderCopyEx(renderer, texture, nullptr, &r, -rotation, nullptr, SDL_FLIP_NONE);
}

void Systems::Move(Vector2* from, const Vector2& dist)
{
	from->x += dist.x;
	from->y += dist.y;
}

void Systems::ClampPosition(Vector2* outPosition, int borderX, int borderY)
{
	outPosition->x = Math::Clamp(outPosition->x, 0.0f, static_cast<float>(borderX));
	outPosition->y = Math::Clamp(outPosition->y, 0.0f, static_cast<float>(borderY));

	GS_LOG("{0} {1}", outPosition->x, outPosition->y);
}
