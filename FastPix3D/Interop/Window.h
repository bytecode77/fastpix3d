#pragma once
#include "../FastPix3D.h"
#include "../Math/Color.h"

struct SDL_Window;
struct SDL_Surface;

class FASTPIX3D_API Window
{
private:
	static int32 WindowCount;
	SDL_Window *SDLWindow;
	SDL_Surface *SDLSurface;

public:
	readonly_property(int32, Width);
	readonly_property(int32, Height);
	readonly_property(Color*, Pixels);
	property_get(const char*, Title);
	property_set(const char*, Title);

	explicit Window(int32 width, int32 height, const char *title);
	~Window();

	void SetRelativeMouseMode(bool enabled);
	void Lock();
	void Unlock();
	void Flip();

	friend class Input;
};