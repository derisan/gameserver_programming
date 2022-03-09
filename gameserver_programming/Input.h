#pragma once

enum class eKeyState
{
	None,
	Repeat,
	Press,
	Release
};

class Input
{
public:
	static void Init();
	static void Update(const uint8* keystate);

	static bool IsButtonRepeat(SDL_Scancode key);
	static bool IsButtonPressed(SDL_Scancode key);
	static bool IsButtonReleased(SDL_Scancode key);

private:
	static vector<eKeyState> sKeyStates;
};
