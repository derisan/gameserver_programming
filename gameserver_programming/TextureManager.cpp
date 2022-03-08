#include "PCH.h"
#include "TextureManager.h"

unordered_map<string, SDL_Texture*> TextureManager::sTextures;
SDL_Renderer* TextureManager::sRenderer;

void TextureManager::Init(SDL_Renderer* renderer)
{
	sRenderer = renderer;
}

void TextureManager::Shutdown()
{
	for (auto& [_, texture] : sTextures)
	{
		SDL_DestroyTexture(texture);
	}

	sTextures.clear();
}

SDL_Texture* TextureManager::GetTexture(const string& filename)
{
	auto iter = sTextures.find(filename);

	if (iter != sTextures.end())
	{
		return iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(filename.c_str());

		if (surf == nullptr)
		{
			GS_LOG("Failed to load texture: {0}", filename);
			GS_ASSERT(false, "ASSERTION FAILED");
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(sRenderer, surf);
		SDL_FreeSurface(surf);

		if (tex == nullptr)
		{
			GS_LOG("Failed to convert surface to texture: {0}", filename);
			GS_ASSERT(false, "ASSERTION FAILED");
		}

		sTextures.emplace(filename, tex);

		return tex;
	}
}


