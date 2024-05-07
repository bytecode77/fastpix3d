#pragma once
#include "FastPix3D.h"
#include "Window.h"
#include "Vector3i.h"

enum class MouseButton
{
	Left = 0,
	Right = 1,
	Middle = 2
};

class Input
{
private:
	static bool IsInitialized;
	static bool Exited;
	static Vector3i MousePosition;
	static Vector3i MouseSpeed;
	static bool *MouseDown;
	static bool *KeyDown;

public:
	Input() = delete;

	static bool HasExited()
	{
		EnsureInitialized();

		return Exited;
	}
	static Vector3i GetMousePosition()
	{
		EnsureInitialized();

		return MousePosition;
	}
	static Vector3i GetMouseSpeed()
	{
		EnsureInitialized();

		return MouseSpeed;
	}
	static bool GetMouseDown(MouseButton mouseButton)
	{
		EnsureInitialized();

		return MouseDown[(int32)mouseButton];
	}
	static bool GetKeyDown(int32 key)
	{
		EnsureInitialized();

		if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK)
		{
			key += 1024 - SDLK_SCANCODE_MASK;
		}

		return KeyDown[key];
	}

	static void Update();
	static void SetRelativeMouseMode(bool enabled);
	static void SetMousePosition(const Window &window, int32 x, int32 y);
	static void CenterMouse(const Window &window);

private:
	static void EnsureInitialized();
};