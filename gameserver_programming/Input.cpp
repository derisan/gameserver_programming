#include "PCH.h"
#include "Input.h"

vector<eKeyState> Input::sKeyStates;

void Input::Init()
{
	sKeyStates.resize(SDL_Scancode::SDL_NUM_SCANCODES, eKeyState::None);
}

void Input::Update(const uint8* keystate)
{
	for (int key = 0; key < SDL_Scancode::SDL_NUM_SCANCODES; ++key)
	{
		if (keystate[key])
		{
			eKeyState& state = sKeyStates[key];

			if (state == eKeyState::Repeat || state == eKeyState::Press)
			{
				state = eKeyState::Repeat;
			}
			else
			{
				state = eKeyState::Press;
			}
		}
		else
		{
			eKeyState& state = sKeyStates[key];

			if (state == eKeyState::Repeat || state == eKeyState::Press)
			{
				state = eKeyState::Release;
			}
			else
			{
				state = eKeyState::None;
			}
		}
	}
}

bool Input::IsButtonRepeat(SDL_Scancode key)
{
	return sKeyStates[key] == eKeyState::Repeat;
}

bool Input::IsButtonPressed(SDL_Scancode key)
{
	return sKeyStates[key] == eKeyState::Press;
}

bool Input::IsButtonReleased(SDL_Scancode key)
{
	return sKeyStates[key] == eKeyState::Release;
}
