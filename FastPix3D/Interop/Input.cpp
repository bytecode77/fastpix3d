#include "Input.h"
#include <SDL3/SDL.h>

bool Input::IsInitialized = false;
bool Input::Exited = false;
vint3 Input::MousePosition;
vint3 Input::MouseSpeed;
bool Input::MouseDown[3];
bool Input::KeyDown[512];
bool Input::KeyPressed[512];

void Input::Update()
{
	MouseSpeed = vint3();

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_EVENT_QUIT:
			{
				Exited = true;
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[(int32)MouseButton::Left] = true; break;
					case SDL_BUTTON_RIGHT: MouseDown[(int32)MouseButton::Right] = true; break;
					case SDL_BUTTON_MIDDLE: MouseDown[(int32)MouseButton::Middle] = true; break;
				}
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_UP:
			{
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[(int32)MouseButton::Left] = false; break;
					case SDL_BUTTON_RIGHT: MouseDown[(int32)MouseButton::Right] = false; break;
					case SDL_BUTTON_MIDDLE: MouseDown[(int32)MouseButton::Middle] = false; break;
				}
				break;
			}
			case SDL_EVENT_MOUSE_MOTION:
			{
				MouseSpeed.X += (int32)e.motion.x - MousePosition.X;
				MouseSpeed.Y += (int32)e.motion.y - MousePosition.Y;
				MousePosition.X = (int32)e.motion.x;
				MousePosition.Y = (int32)e.motion.y;
				break;
			}
			case SDL_EVENT_MOUSE_WHEEL:
			{
				MouseSpeed.Z += (int32)e.wheel.y;
				MousePosition.Z += (int32)e.wheel.y;
				break;
			}
			case SDL_EVENT_KEY_DOWN:
			{
				if (e.key.scancode < 512)
				{
					KeyDown[(int32)e.key.scancode] = true;
					KeyPressed[(int32)e.key.scancode] = true;
				}
				break;
			}
			case SDL_EVENT_KEY_UP:
			{
				if (e.key.scancode < 512)
				{
					KeyDown[(int32)e.key.scancode] = false;
					KeyPressed[(int32)e.key.scancode] = false;
				}
				break;
			}
		}
	}
}
void Input::SetMousePosition(const Window &window, int32 x, int32 y)
{
	MousePosition.X = x;
	MousePosition.Y = y;

	SDL_WarpMouseInWindow(window.SDLWindow, (float)x, (float)y);
}
void Input::CenterMouse(const Window &window)
{
	SetMousePosition(window, window.Width / 2, window.Height / 2);
}