#include "PCH.h"
#include "Components.h"

TransformComponent::TransformComponent(float x /*= 0.0f*/, float y /*= 0.0f*/, float rotation /*= 0*/, float scale /*= 1.0f*/)
	: Position(x, y)
	, Rotation(rotation)
	, Scale(scale)
{

}

TransformComponent::TransformComponent(const Vector2& position, float rotation /*= 0*/, float scale /*= 1.0f*/)
	: Position(position)
	, Rotation(rotation)
	, Scale(scale)
{

}

SpriteRendererComponent::SpriteRendererComponent(SDL_Texture* texture /*= nullptr*/, int drawOrder /*= 100*/)
	: Texture(texture)
	, DrawOrder(drawOrder)
{
	SDL_QueryTexture(texture, nullptr, nullptr, &Width, &Height);
}
