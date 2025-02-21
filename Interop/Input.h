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

enum class Scancode
{
	A = 4,
	B = 5,
	C = 6,
	D = 7,
	E = 8,
	F = 9,
	G = 10,
	H = 11,
	I = 12,
	J = 13,
	K = 14,
	L = 15,
	M = 16,
	N = 17,
	O = 18,
	P = 19,
	Q = 20,
	R = 21,
	S = 22,
	T = 23,
	U = 24,
	V = 25,
	W = 26,
	X = 27,
	Y = 28,
	Z = 29,
	D1 = 30,
	D2 = 31,
	D3 = 32,
	D4 = 33,
	D5 = 34,
	D6 = 35,
	D7 = 36,
	D8 = 37,
	D9 = 38,
	D0 = 39,
	Return = 40,
	Escape = 41,
	Back = 42,
	Tab = 43,
	Space = 44,
	Minus = 45,
	Equals = 46,
	LeftBracket = 47,
	RightBracket = 48,
	BackSlash = 49,
	NonUShash = 50,
	Semicolon = 51,
	Apostrophe = 52,
	Grave = 53,
	Comma = 54,
	Period = 55,
	Slash = 56,
	Capslock = 57,
	F1 = 58,
	F2 = 59,
	F3 = 60,
	F4 = 61,
	F5 = 62,
	F6 = 63,
	F7 = 64,
	F8 = 65,
	F9 = 66,
	F10 = 67,
	F11 = 68,
	F12 = 69,
	PrintScreen = 70,
	ScrollLock = 71,
	Pause = 72,
	Insert = 73,
	Home = 74,
	PageUp = 75,
	Delete = 76,
	End = 77,
	PageDown = 78,
	Right = 79,
	Left = 80,
	Down = 81,
	Up = 82,
	Numlock = 83,
	KeyPadDivide = 84,
	KeyPadMultiply = 85,
	KeyPadMinus = 86,
	KeyPadPlus = 87,
	KeyPadEnter = 88,
	KeyPad1 = 89,
	KeyPad2 = 90,
	KeyPad3 = 91,
	KeyPad4 = 92,
	KeyPad5 = 93,
	KeyPad6 = 94,
	KeyPad7 = 95,
	KeyPad8 = 96,
	KeyPad9 = 97,
	KeyPad0 = 98,
	KeyPadPeriod = 99,
	NonUSBackSlash = 100,
	Application = 101,
	Power = 102,
	KeyPadEquals = 103,
	F13 = 104,
	F14 = 105,
	F15 = 106,
	F16 = 107,
	F17 = 108,
	F18 = 109,
	F19 = 110,
	F20 = 111,
	F21 = 112,
	F22 = 113,
	F23 = 114,
	F24 = 115,
	Execute = 116,
	Help = 117,
	Menu = 118,
	Select = 119,
	Stop = 120,
	Again = 121,
	Undo = 122,
	Cut = 123,
	Copy = 124,
	Paste = 125,
	Find = 126,
	Mute = 127,
	VolumeUp = 128,
	VolumeDown = 129,
	KeyPadComma = 133,
	KeyPadEqualsAs400 = 134,
	International1 = 135,
	International2 = 136,
	International3 = 137,
	International4 = 138,
	International5 = 139,
	International6 = 140,
	International7 = 141,
	International8 = 142,
	International9 = 143,
	Lang1 = 144,
	Lang2 = 145,
	Lang3 = 146,
	Lang4 = 147,
	Lang5 = 148,
	Lang6 = 149,
	Lang7 = 150,
	Lang8 = 151,
	Lang9 = 152,
	AltErase = 153,
	SysReq = 154,
	Cancel = 155,
	Clear = 156,
	Prior = 157,
	Return2 = 158,
	Separator = 159,
	Out = 160,
	Oper = 161,
	ClearAgain = 162,
	Crsel = 163,
	Exsel = 164,
	KeyPad00 = 176,
	KeyPad000 = 177,
	ThousandsSeparator = 178,
	DecimalSeparator = 179,
	CurrencyUnit = 180,
	CurrencySubUnit = 181,
	KeyPadLeftParen = 182,
	KeyPadRightParen = 183,
	KeyPadLeftBrace = 184,
	KeyPadRightBrace = 185,
	KeyPadTab = 186,
	KeyPadBackSpace = 187,
	KeyPadA = 188,
	KeyPadB = 189,
	KeyPadC = 190,
	KeyPadD = 191,
	KeyPadE = 192,
	KeyPadF = 193,
	KeyPadXor = 194,
	KeyPadPower = 195,
	KeyPadPercent = 196,
	KeyPadLess = 197,
	KeyPadGreater = 198,
	KeyPadAmpersand = 199,
	KeyPadDblAmpersand = 200,
	KeyPadVerticalBar = 201,
	KeyPadDblVerticalBar = 202,
	KeyPadColon = 203,
	KeyPadHash = 204,
	KeyPadSpace = 205,
	KeyPadAt = 206,
	KeyPadExclamation = 207,
	KeyPadMemStore = 208,
	KeyPadMemRecall = 209,
	KeyPadMemClear = 210,
	KeyPadMemAdd = 211,
	KeyPadMemSubtract = 212,
	KeyPadMemMultiply = 213,
	KeyPadMemDivide = 214,
	KeyPadPlusMinus = 215,
	KeyPadClear = 216,
	KeyPadClearEntry = 217,
	KeyPadBinary = 218,
	KeyPadOctal = 219,
	KeyPadDecimal = 220,
	KeyPadHexadecimal = 221,
	LeftCtrl = 224,
	LeftShift = 225,
	LeftAlt = 226,
	LeftGui = 227,
	RightCtrl = 228,
	RightShift = 229,
	RightAlt = 230,
	RightGui = 231,
	Mode = 257,
	Sleep = 258,
	Wake = 259,
	ChannelIncrement = 260,
	ChannelDecrement = 261,
	MediaPlay = 262,
	MediaPause = 263,
	MediaRecord = 264,
	MediaFastForward = 265,
	MediaRewind = 266,
	MediaNextTrack = 267,
	MediaPreviousTrack = 268,
	MediaStop = 269,
	MediaEject = 270,
	MediaPlayPause = 271,
	MediaSelect = 272,
	AcNew = 273,
	AcOpen = 274,
	AcClose = 275,
	AcExit = 276,
	AcSave = 277,
	AcPrint = 278,
	AcProperties = 279,
	AcSearch = 280,
	AcHome = 281,
	AcBack = 282,
	AcForward = 283,
	AcStop = 284,
	AcRefresh = 285,
	AcBookmarks = 286,
	SoftLeft = 287,
	SoftRight = 288,
	Call = 289,
	EndCall = 290
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
	static bool *KeyPressed;

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
	static bool GetKeyDown(Scancode scancode)
	{
		EnsureInitialized();

		return KeyDown[(int32)scancode];
	}
	static bool GetKeyPressed(Scancode scancode)
	{
		EnsureInitialized();

		bool pressed = KeyPressed[(int32)scancode];
		KeyPressed[(int32)scancode] = false;
		return pressed;
	}

	static void Update();
	static void SetMousePosition(const Window &window, int32 x, int32 y);
	static void CenterMouse(const Window &window);

private:
	static void EnsureInitialized();
};