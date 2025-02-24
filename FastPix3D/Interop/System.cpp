#include "System.h"
#include <SDL3/SDL.h>

int64 System::Milliseconds()
{
	return SDL_GetTicks();
}
int64 System::Microseconds()
{
	return SDL_GetTicksNS() / 1000;
}