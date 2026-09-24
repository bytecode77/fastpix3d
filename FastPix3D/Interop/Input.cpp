#include "Input.h"

bool Input::_HasExited;
vint3 Input::_MousePosition;
vint3 Input::_MouseSpeed;
int32 Input::MouseSpeedZRemainder;
bool Input::MouseDown[3];
bool Input::KeyDown[512];
bool Input::KeyPressed[512];

static const Scancode ScancodeLookup[] =
{
	/* 0x00 */ Scancode::Unknown,		Scancode::Escape,		Scancode::D1,				Scancode::D2,
	/* 0x04 */ Scancode::D3,			Scancode::D4,			Scancode::D5,				Scancode::D6,
	/* 0x08 */ Scancode::D7,			Scancode::D8,			Scancode::D9,				Scancode::D0,
	/* 0x0c */ Scancode::Minus,			Scancode::Equals,		Scancode::Back,				Scancode::Tab,
	/* 0x10 */ Scancode::Q,				Scancode::W,			Scancode::E,				Scancode::R,
	/* 0x14 */ Scancode::T,				Scancode::Y,			Scancode::U,				Scancode::I,
	/* 0x18 */ Scancode::O,				Scancode::P,			Scancode::LeftBracket,		Scancode::RightBracket,
	/* 0x1c */ Scancode::Return,		Scancode::LeftCtrl,		Scancode::A,				Scancode::S,
	/* 0x20 */ Scancode::D,				Scancode::F,			Scancode::G,				Scancode::H,
	/* 0x24 */ Scancode::J,				Scancode::K,			Scancode::L,				Scancode::Semicolon,
	/* 0x28 */ Scancode::Apostrophe,	Scancode::Grave,		Scancode::LeftShift,		Scancode::BackSlash,
	/* 0x2c */ Scancode::Z,				Scancode::X,			Scancode::C,				Scancode::V,
	/* 0x30 */ Scancode::B,				Scancode::N,			Scancode::M,				Scancode::Comma,
	/* 0x34 */ Scancode::Period,		Scancode::Slash,		Scancode::RightShift,		Scancode::KeyPadMultiply,
	/* 0x38 */ Scancode::LeftAlt,		Scancode::Space,		Scancode::Capslock,			Scancode::F1,
	/* 0x3c */ Scancode::F2,			Scancode::F3,			Scancode::F4,				Scancode::F5,
	/* 0x40 */ Scancode::F6,			Scancode::F7,			Scancode::F8,				Scancode::F9,
	/* 0x44 */ Scancode::F10,			Scancode::Numlock,		Scancode::ScrollLock,		Scancode::KeyPad7,
	/* 0x48 */ Scancode::KeyPad8,		Scancode::KeyPad9,		Scancode::KeyPadMinus,		Scancode::KeyPad4,
	/* 0x4c */ Scancode::KeyPad5,		Scancode::KeyPad6,		Scancode::KeyPadPlus,		Scancode::KeyPad1,
	/* 0x50 */ Scancode::KeyPad2,		Scancode::KeyPad3,		Scancode::KeyPad0,			Scancode::KeyPadPeriod,
	/* 0x54 */ Scancode::SysReq,		Scancode::Unknown,		Scancode::NonUSBackSlash,	Scancode::F11,
	/* 0x58 */ Scancode::F12,			Scancode::KeyPadEquals,	Scancode::Unknown,			Scancode::Unknown,
	/* 0x5c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x60 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x64 */ Scancode::F13,			Scancode::F14,			Scancode::F15,				Scancode::F16,
	/* 0x68 */ Scancode::F17,			Scancode::F18,			Scancode::F19,				Scancode::F20,
	/* 0x6c */ Scancode::F21,			Scancode::F22,			Scancode::F23,				Scancode::Unknown,
	/* 0x70 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::International1,
	/* 0x74 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::F24,				Scancode::Unknown,
	/* 0x78 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x7c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,

	/* 0x00 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x04 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x08 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x0c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x10 */ Scancode::MediaPrevious,	Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x14 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x18 */ Scancode::Unknown,		Scancode::MediaNext,	Scancode::Unknown,			Scancode::Unknown,
	/* 0x1c */ Scancode::KeyPadEnter,	Scancode::RightCtrl,	Scancode::Unknown,			Scancode::Unknown,
	/* 0x20 */ Scancode::Mute,			Scancode::Unknown,		Scancode::MediaPlayPause,	Scancode::Unknown,
	/* 0x24 */ Scancode::MediaStop,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x28 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x2c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::VolumeDown,		Scancode::Unknown,
	/* 0x30 */ Scancode::VolumeUp,		Scancode::Unknown,		Scancode::AcHome,			Scancode::Unknown,
	/* 0x34 */ Scancode::Unknown,		Scancode::KeyPadDivide,	Scancode::Unknown,			Scancode::PrintScreen,
	/* 0x38 */ Scancode::RightAlt,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x3c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x40 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x44 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Home,
	/* 0x48 */ Scancode::Up,			Scancode::PageUp,		Scancode::Unknown,			Scancode::Left,
	/* 0x4c */ Scancode::Unknown,		Scancode::Right,		Scancode::Unknown,			Scancode::End,
	/* 0x50 */ Scancode::Down,			Scancode::PageDown,		Scancode::Insert,			Scancode::Delete,
	/* 0x54 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x58 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::LeftGui,
	/* 0x5c */ Scancode::RightGui,		Scancode::Application,	Scancode::Power,			Scancode::Sleep,
	/* 0x60 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Wake,
	/* 0x64 */ Scancode::Unknown,		Scancode::AcSearch,		Scancode::AcBookmarks,		Scancode::AcRefresh,
	/* 0x68 */ Scancode::AcStop,		Scancode::AcForward,	Scancode::AcBack,			Scancode::Unknown,
	/* 0x6c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x70 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x74 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x78 */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown,
	/* 0x7c */ Scancode::Unknown,		Scancode::Unknown,		Scancode::Unknown,			Scancode::Unknown
};

void Input::Update()
{
	_MouseSpeed = vint3();

	MSG message;
	while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
		{
			_HasExited = true;
		}
		else
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}
}
void Input::SetMousePosition(const Window &window, int32 x, int32 y)
{
	_MousePosition.X = x;
	_MousePosition.Y = y;

	POINT point = { x, y };
	ClientToScreen(window.Handle, &point);
	SetCursorPos(point.x, point.y);
}
void Input::CenterMouse(const Window &window)
{
	SetMousePosition(window, window.Width / 2, window.Height / 2);
}

void Input::HandleMessage(const Window &window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			_HasExited = true;
			break;
		}
		case WM_KILLFOCUS:
		{
			Reset();
			break;
		}
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				Reset();
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			MouseDown[(int32)MouseButton::Left] = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			MouseDown[(int32)MouseButton::Left] = false;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			MouseDown[(int32)MouseButton::Right] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			MouseDown[(int32)MouseButton::Right] = false;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			MouseDown[(int32)MouseButton::Middle] = true;
			break;
		}
		case WM_MBUTTONUP:
		{
			MouseDown[(int32)MouseButton::Middle] = false;
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!window.RelativeMouseMode)
			{
				_MouseSpeed.X += (int16)LOWORD(lParam) - _MousePosition.X;
				_MouseSpeed.Y += (int16)HIWORD(lParam) - _MousePosition.Y;
				_MousePosition.X = (int16)LOWORD(lParam);
				_MousePosition.Y = (int16)HIWORD(lParam);
			}
			break;
		}
		case WM_INPUT:
		{
			if (window.RelativeMouseMode)
			{
				RAWINPUT input;
				UINT size = sizeof(RAWINPUT);
				if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER)) != (UINT)-1 && input.header.dwType == RIM_TYPEMOUSE)
				{
					RAWMOUSE &mouse = input.data.mouse;

					if (!(mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
					{
						_MouseSpeed.X += mouse.lLastX;
						_MouseSpeed.Y += mouse.lLastY;
						_MousePosition.X += mouse.lLastX;
						_MousePosition.Y += mouse.lLastY;
					}
					else
					{
						bool virtualDesktop = (mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) != 0;

						int32 screenLeft = virtualDesktop ? GetSystemMetrics(SM_XVIRTUALSCREEN) : 0;
						int32 screenTop = virtualDesktop ? GetSystemMetrics(SM_YVIRTUALSCREEN) : 0;
						int32 screenWidth = virtualDesktop ? GetSystemMetrics(SM_CXVIRTUALSCREEN) : GetSystemMetrics(SM_CXSCREEN);
						int32 screenHeight = virtualDesktop ? GetSystemMetrics(SM_CYVIRTUALSCREEN) : GetSystemMetrics(SM_CYSCREEN);

						POINT point =
						{
							screenLeft + MulDiv(mouse.lLastX, screenWidth - 1, 65535),
							screenTop + MulDiv(mouse.lLastY, screenHeight - 1, 65535)
						};

						ScreenToClient(window.Handle, &point);

						_MouseSpeed.X += point.x - _MousePosition.X;
						_MouseSpeed.Y += point.y - _MousePosition.Y;
						_MousePosition.X = point.x;
						_MousePosition.Y = point.y;
					}
				}
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			MouseSpeedZRemainder += GET_WHEEL_DELTA_WPARAM(wParam);
			int32 wheel = MouseSpeedZRemainder / WHEEL_DELTA;
			MouseSpeedZRemainder %= WHEEL_DELTA;

			_MouseSpeed.Z += wheel;
			_MousePosition.Z += wheel;
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			int32 scancode = (int32)ConvertToScancode(wParam, lParam);
			KeyDown[scancode] = true;
			KeyPressed[scancode] = true;
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			int32 scancode = (int32)ConvertToScancode(wParam, lParam);
			KeyDown[scancode] = false;
			KeyPressed[scancode] = false;
			break;
		}
	}
}
void Input::Reset()
{
	memset(MouseDown, 0, sizeof(MouseDown));
	memset(KeyDown, 0, sizeof(KeyDown));
	memset(KeyPressed, 0, sizeof(KeyPressed));
	MouseSpeedZRemainder = 0;
}

Scancode Input::ConvertToScancode(WPARAM virtualKey, LPARAM keyData)
{
	if (virtualKey == VK_PAUSE)
	{
		return Scancode::Pause;
	}

	Scancode scancode = ScancodeLookup[((uint32)keyData >> 16) & 0x7f | ((uint32)keyData >> 17) & 0x80];
	if (scancode != Scancode::Unknown)
	{
		return scancode;
	}

	switch (virtualKey)
	{
		case VK_NUMLOCK: return Scancode::Numlock;
		case VK_EXECUTE: return Scancode::Execute;
		case VK_HELP: return Scancode::Help;
		case VK_SELECT: return Scancode::Select;
		case VK_CANCEL: return Scancode::Cancel;
		case VK_SEPARATOR: return Scancode::Separator;
		default: return Scancode::Unknown;
	}
}