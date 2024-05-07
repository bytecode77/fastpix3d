#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <Windows.h>
#include <Shlwapi.h>
#include <math.h>
#include <smmintrin.h>
#include <algorithm>
#include <vector>
#include "SDL/SDL.h"

#ifndef _WIN64
#error "FastPix3D is only supported in x64 build"
#endif

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")

using sbyte = signed __int8;
using int16 = signed __int16;
using int32 = signed __int32;
using int64 = signed __int64;
using byte = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#define property_get(type, name)												\
	__declspec(property(get = get_##name))										\
	type name;																	\
	type get_##name() const

#define property_getset(type, name)												\
	__declspec(property(get = get_##name, put = set_##name))					\
	type name;																	\
	type get_##name() const

#define property_set(type, name)												\
	void set_##name(type value)