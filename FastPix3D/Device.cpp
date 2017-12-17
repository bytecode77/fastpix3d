#include "FastPix3D.h"

int32 Device::Width;
int32 Device::Height;
int32 *Device::BackBuffer;
int16 *Device::DepthBuffer;
sbyte *Device::StencilBuffer;

SDL_Window *Device::Window;
SDL_Renderer *Device::Renderer;
SDL_Texture *Device::ScreenTexture;

void Device::Initialize(int32 width, int32 height)
{
	Width = width;
	Height = height;
	SDL_Init(SDL_INIT_VIDEO);
	Window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	ScreenTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, Width, Height);

	BackBuffer = new int32[Width * Height];
	DepthBuffer = new int16[Width * Height];
	StencilBuffer = new sbyte[Width * Height];

	setTitle("FastPix3D");
	Input::Initialize();
	RenderStates::Initialize();
	Light::Initialize();
	Drawer::Initialize();
}
void Device::Destroy()
{
	delete[] DepthBuffer;
	delete[] StencilBuffer;

	RenderStates::Destroy();
	Input::Destroy();

	SDL_DestroyTexture(ScreenTexture);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Device::ClearBackBuffer()
{
	memset(BackBuffer, 0, Width * Height * 4);
}
void Device::ClearBackBuffer(Color color)
{
	fill_n(BackBuffer, Width * Height, color.getRGB());
}
void Device::ClearDepthBuffer()
{
	memset(DepthBuffer, 1, Width * Height << 1);
}
void Device::ClearStencilBuffer()
{
	memset(StencilBuffer, 0, Width * Height);
}
void Device::Present()
{
	SDL_UpdateTexture(ScreenTexture, NULL, BackBuffer, Width << 2);
	SDL_RenderCopy(Renderer, ScreenTexture, NULL, NULL);
	SDL_RenderPresent(Renderer);
}

string Device::getTitle()
{
	return string(SDL_GetWindowTitle(Window));
}
int32 Device::getWidth()
{
	return Width;
}
int32 Device::getHeight()
{
	return Height;
}

void Device::setTitle(string title)
{
	SDL_SetWindowTitle(Window, title.c_str());
}