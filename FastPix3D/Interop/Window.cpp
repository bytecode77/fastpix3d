#include "Window.h"
#include "Input.h"

static const char WindowClassName[] = "FastPix3D_Window";

int32 Window::WindowCount;
Window *Window::RelativeMouseWindow;

void Window::set_Title(const char *value)
{
	if (!value)
	{
		value = "";
	}

	SetWindowTextA(Handle, value);

	int32 length = lstrlenA(value);
	char *newTitle = new char[length + 1];
	memcpy(newTitle, value, length + 1);

	delete[] _Title;
	_Title = newTitle;
}

Window::Window(int32 width, int32 height) : Window(width, height, nullptr, false)
{
}
Window::Window(int32 width, int32 height, const char *title) : Window(width, height, title, false)
{
}
Window::Window(int32 width, int32 height, const char *title, bool fullScreen)
{
	// Only support resolutions that are multiples of 8 to simplify the halfspace rasterization.
	if (width <= 0 || height <= 0 || (width & 7) || (height & 7)) throw std::runtime_error("Window width and height must be multiples of 8.");

	if (WindowCount == 0)
	{
		WNDCLASSEXA windowClass = {};
		windowClass.cbSize = sizeof(WNDCLASSEXA);
		windowClass.style = CS_OWNDC;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = GetModule();
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.lpszClassName = WindowClassName;

		if (!RegisterClassExA(&windowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
		{
			throw std::runtime_error("Failed to register Win32 window class.");
		}
	}

	_Width = width;
	_Height = height;
	_Pixels = (Color*)VirtualAlloc(nullptr, _Width * _Height * sizeof(Color), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth = _Width;
	BitmapInfo.bmiHeader.biHeight = -_Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	DWORD windowStyle;
	int32 windowX;
	int32 windowY;
	int32 windowWidth;
	int32 windowHeight;

	if (fullScreen)
	{
		windowStyle = WS_POPUP;
		windowX = 0;
		windowY = 0;
		windowWidth = _Width;
		windowHeight = _Height;
	}
	else
	{
		windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		RECT windowRect = { 0, 0, _Width, _Height };
		AdjustWindowRectEx(&windowRect, windowStyle, FALSE, 0);

		windowX = CW_USEDEFAULT;
		windowY = CW_USEDEFAULT;
		windowWidth = windowRect.right - windowRect.left;
		windowHeight = windowRect.bottom - windowRect.top;

		RECT workArea;
		if (SystemParametersInfoA(SPI_GETWORKAREA, 0, &workArea, 0))
		{
			windowX = workArea.left + ((workArea.right - workArea.left) - windowWidth) / 2;
			windowY = workArea.top + ((workArea.bottom - workArea.top) - windowHeight) / 2;
		}
	}

	Handle = CreateWindowExA(fullScreen ? WS_EX_TOPMOST : 0, WindowClassName, title ? title : "", windowStyle, windowX, windowY, windowWidth, windowHeight, nullptr, nullptr, GetModule(), this);
	if (!Handle) throw std::runtime_error("Failed to create Win32 window.");

	DeviceContext = GetDC(Handle);
	if (!DeviceContext) throw std::runtime_error("Failed to get Win32 window device context.");

	Title = title;
	WindowCount++;

	ShowWindow(Handle, SW_SHOW);
	UpdateWindow(Handle);

	_MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);
}
Window* Window::CreateFullScreen()
{
	return CreateFullScreen(nullptr);
}
Window* Window::CreateFullScreen(const char *title)
{
	int32 width = GetSystemMetrics(SM_CXSCREEN);
	int32 height = GetSystemMetrics(SM_CYSCREEN);

	width -= width & 7;
	height -= height & 7;

	return new Window(width, height, title, true);
}
Window::~Window()
{
	SetRelativeMouseMode(false);
	DestroyWindow(Handle);
	VirtualFree(_Pixels, 0, MEM_RELEASE);

	delete[] _Title;

	if (--WindowCount == 0)
	{
		UnregisterClassA(WindowClassName, GetModule());
	}
}

void Window::Flip() const
{
	SetDIBitsToDevice(DeviceContext, 0, 0, _Width, _Height, 0, 0, 0, _Height, _Pixels, &BitmapInfo, DIB_RGB_COLORS);
}
void Window::SetRelativeMouseMode(bool enabled)
{
	if (RelativeMouseMode == enabled)
	{
		return;
	}

	if (enabled)
	{
		if (RelativeMouseWindow && RelativeMouseWindow != this)
		{
			RelativeMouseWindow->SetRelativeMouseMode(false);
		}

		RAWINPUTDEVICE device = {};
		device.usUsagePage = 1;
		device.usUsage = 2;
		device.dwFlags = 0;
		device.hwndTarget = Handle;

		if (!RegisterRawInputDevices(&device, 1, sizeof(RAWINPUTDEVICE)))
		{
			throw std::runtime_error("Failed to register raw mouse input.");
		}

		RelativeMouseMode = true;
		RelativeMouseWindow = this;

		if (GetForegroundWindow() == Handle)
		{
			SetMouseClip(true);
		}
	}
	else
	{
		RAWINPUTDEVICE device = {};
		device.usUsagePage = 1;
		device.usUsage = 2;
		device.dwFlags = RIDEV_REMOVE;
		device.hwndTarget = nullptr;

		RegisterRawInputDevices(&device, 1, sizeof(RAWINPUTDEVICE));
		SetMouseClip(false);

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(Handle, &point);

		Input::_MousePosition.X = point.x;
		Input::_MousePosition.Y = point.y;

		RelativeMouseMode = false;

		if (RelativeMouseWindow == this)
		{
			RelativeMouseWindow = nullptr;
		}
	}
}

LRESULT CALLBACK Window::WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window *window = (Window*)GetWindowLongPtrA(windowHandle, GWLP_USERDATA);

	if (message == WM_NCCREATE)
	{
		CREATESTRUCTA *createStruct = (CREATESTRUCTA*)lParam;

		window = (Window*)createStruct->lpCreateParams;
		window->Handle = windowHandle;

		SetWindowLongPtrA(windowHandle, GWLP_USERDATA, (LONG_PTR)window);
	}

	if (window)
	{
		Input::HandleMessage(*window, message, wParam, lParam);
	}

	switch (message)
	{
		case WM_CLOSE:
		{
			// Input::HasExited goes to true, so do not close the Window here.
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			BeginPaint(windowHandle, &paint);
			EndPaint(windowHandle, &paint);
			return 0;
		}
		case WM_ERASEBKGND:
		{
			// The frame buffer covers the entire client area.
			return 1;
		}
		case WM_ACTIVATE:
		{
			if (window && window->RelativeMouseMode)
			{
				window->SetMouseClip(LOWORD(wParam) != WA_INACTIVE);
			}
			break;
		}
		case WM_MOVE:
		{
			if (window && window->RelativeMouseMode && GetForegroundWindow() == windowHandle)
			{
				window->SetMouseClip(true);
			}
			break;
		}
		case WM_SETCURSOR:
		{
			if (window && window->RelativeMouseMode && LOWORD(lParam) == HTCLIENT && GetForegroundWindow() == windowHandle)
			{
				SetCursor(nullptr);
				return TRUE;
			}
			break;
		}
	}

	return DefWindowProcA(windowHandle, message, wParam, lParam);
}
HMODULE Window::GetModule()
{
	HMODULE module;
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&WindowCount, &module);

	return module;
}
void Window::SetMouseClip(bool enabled) const
{
	if (enabled)
	{
		RECT rect;
		GetClientRect(Handle, &rect);

		POINT topLeft = { rect.left, rect.top };
		POINT bottomRight = { rect.right, rect.bottom };

		ClientToScreen(Handle, &topLeft);
		ClientToScreen(Handle, &bottomRight);

		rect.left = topLeft.x;
		rect.top = topLeft.y;
		rect.right = bottomRight.x;
		rect.bottom = bottomRight.y;

		ClipCursor(&rect);
		SetCursor(nullptr);
	}
	else
	{
		ClipCursor(nullptr);
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
	}
}