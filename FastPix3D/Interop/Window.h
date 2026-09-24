#pragma once
#include "../FastPix3D.h"
#include "../Math/Color.h"

class FASTPIX3D_API Window
{
private:
	static int32 WindowCount;
	static Window *RelativeMouseWindow;

	HWND Handle = nullptr;
	HDC DeviceContext = nullptr;
	BITMAPINFO BitmapInfo = {};

	int32 _Width;
	int32 _Height;
	Color *_Pixels;
	char *_Title = nullptr;
	bool RelativeMouseMode = false;

public:
	readonly_property(int32, Width)
	{
		return _Width;
	}
	readonly_property(int32, Height)
	{
		return _Height;
	}
	readonly_property(Color*, Pixels)
	{
		return _Pixels;
	}
	property_get(const char*, Title)
	{
		return _Title ? _Title : "";
	}
	property_set(const char*, Title);

	explicit Window(int32 width, int32 height);
	explicit Window(int32 width, int32 height, const char *title);
	static Window* CreateFullScreen();
	static Window* CreateFullScreen(const char *title);
	Window(const Window&) = delete;
	~Window();

	void Flip() const;
	void SetRelativeMouseMode(bool enabled);

private:
	explicit Window(int32 width, int32 height, const char *title, bool fullScreen);

	static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	static HMODULE GetModule();
	void SetMouseClip(bool enabled) const;

public:
	Window& operator =(const Window&) = delete;

	friend class Input;
};