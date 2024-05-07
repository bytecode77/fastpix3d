#include "Input.h"

bool Input::IsInitialized = false;
bool Input::Exited = false;
Vector3i Input::MousePosition;
Vector3i Input::MouseSpeed;
bool *Input::MouseDown;
bool *Input::KeyDown;

void Input::Update()
{
	EnsureInitialized();

	MouseSpeed = Vector3i();

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
			{
				Exited = true;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[(int32)MouseButton::Left] = true; break;
					case SDL_BUTTON_RIGHT: MouseDown[(int32)MouseButton::Right] = true; break;
					case SDL_BUTTON_MIDDLE: MouseDown[(int32)MouseButton::Middle] = true; break;
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[(int32)MouseButton::Left] = false; break;
					case SDL_BUTTON_RIGHT: MouseDown[(int32)MouseButton::Right] = false; break;
					case SDL_BUTTON_MIDDLE: MouseDown[(int32)MouseButton::Middle] = false; break;
				}
				break;
			}
			case SDL_MOUSEMOTION:
			{
				MouseSpeed.X += e.motion.x - MousePosition.X;
				MouseSpeed.Y += e.motion.y - MousePosition.Y;
				MousePosition.X = e.motion.x;
				MousePosition.Y = e.motion.y;
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				MouseSpeed.Z += e.wheel.y;
				MousePosition.Z += e.wheel.y;
				break;
			}
			case SDL_KEYDOWN:
			{
				int32 key = e.key.keysym.sym;
				if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
				if (key < 2048) KeyDown[key] = true;
				break;
			}
			case SDL_KEYUP:
			{
				int32 key = e.key.keysym.sym;
				if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
				if (key < 2048) KeyDown[key] = false;
				break;
			}
		}
	}
}
void Input::SetRelativeMouseMode(bool enabled)
{
	SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
}
void Input::SetMousePosition(const Window &window, int32 x, int32 y)
{
	EnsureInitialized();

	MousePosition.X = x;
	MousePosition.Y = y;

	SDL_WarpMouseInWindow(window.SDLWindow, x, y);
}
void Input::CenterMouse(const Window &window)
{
	SetMousePosition(window, window.Width / 2, window.Height / 2);
}

void Input::EnsureInitialized()
{
	if (!IsInitialized)
	{
		MouseDown = new bool[3]();
		KeyDown = new bool[2048]();
		IsInitialized = true;
	}
}