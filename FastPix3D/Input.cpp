#include "FastPix3D.h"

bool Input::Quit;
int32 Input::MouseX;
int32 Input::MouseY;
int32 Input::MouseZ;
int32 Input::MouseXSpeed;
int32 Input::MouseYSpeed;
int32 Input::MouseZSpeed;
bool *Input::MouseDown;
bool *Input::KeyDown;

void Input::Initialize()
{
	MouseDown = new bool[3];
	KeyDown = new bool[2048];
	memset(MouseDown, 0, 3);
	memset(KeyDown, 0, 2048);
}
void Input::Destroy()
{
	delete[] MouseDown;
	delete[] KeyDown;
}

void Input::Clear()
{
	SDL_Event e;
	while (SDL_PollEvent(&e));
}
void Input::Update()
{
	MouseXSpeed = 0;
	MouseYSpeed = 0;
	MouseZSpeed = 0;

	SDL_Event e;
	int32 key;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				Quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[0] = true; break;
					case SDL_BUTTON_RIGHT: MouseDown[1] = true; break;
					case SDL_BUTTON_MIDDLE: MouseDown[2] = true; break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[0] = false; break;
					case SDL_BUTTON_RIGHT: MouseDown[1] = false; break;
					case SDL_BUTTON_MIDDLE: MouseDown[2] = false; break;
				}
				break;
			case SDL_MOUSEMOTION:
				MouseX = e.motion.x;
				MouseY = e.motion.y;
				MouseXSpeed += e.motion.xrel;
				MouseYSpeed += e.motion.yrel;
				break;
			case SDL_MOUSEWHEEL:
				MouseZ += e.wheel.y;
				MouseZSpeed = e.wheel.y;
				break;
			case SDL_KEYDOWN:
				key = e.key.keysym.sym;
				if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
				if (key < 2048) KeyDown[key] = true;
				break;
			case SDL_KEYUP:
				key = e.key.keysym.sym;
				if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
				if (key < 2048) KeyDown[key] = false;
				break;
		}
	}
}

bool Input::getQuit()
{
	return Quit;
}
int32 Input::getMouseX()
{
	return MouseX;
}
int32 Input::getMouseY()
{
	return MouseY;
}
int32 Input::getMouseZ()
{
	return MouseZ;
}
int32 Input::getMouseXSpeed()
{
	return MouseXSpeed;
}
int32 Input::getMouseYSpeed()
{
	return MouseYSpeed;
}
int32 Input::getMouseZSpeed()
{
	return MouseZSpeed;
}
bool Input::getMouseDown(MouseButton mouseButton)
{
	return MouseDown[int32(mouseButton)];
}
bool Input::getKeyDown(int32 key)
{
	if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
	return KeyDown[key];
}

void Input::setMousePosition(int32 x, int32 y, bool updateMouseSpeed)
{
	if (updateMouseSpeed)
	{
		MouseXSpeed += MouseX - x;
		MouseYSpeed += MouseY - y;
	}
	MouseX = x;
	MouseY = y;
	SDL_WarpMouseInWindow(Device::Window, x, y);
}