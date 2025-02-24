#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#pragma warning(disable: 4251) // 'std::[...]' needs to have dll - interface to be used by clients of '[...]'

#ifndef _WIN64
#error "FastPix3D is only supported in x64 build"
#endif

#ifndef __AVX2__
#error "FastPix3D requires AVX2 support"
#endif

#ifdef FASTPIX3D_EXPORTS
#define FASTPIX3D_API __declspec(dllexport)
#else
#define FASTPIX3D_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <Shlwapi.h>
#include <math.h>
#include <intrin.h>
#include <algorithm>
#include <vector>
#include <atomic>
#include <functional>

using sbyte = signed __int8;
using int16 = signed __int16;
using int32 = signed __int32;
using int64 = signed __int64;
using byte = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using fix32 = int32;
using fix64 = int64;

using _vint3 = __m128i;
using _vfloat3 = __m128;
using _vint4 = __m128i;
using _vuint4 = __m128i;
using _vlong4 = __m256i;
using _vfloat4 = __m128;
using _vint8 = __m256i;
using _vuint8 = __m256i;
using _vfloat8 = __m256;
using _vshort16 = __m256i;
using _vushort16 = __m256i;
using _vsbyte32 = __m256i;
using _vbyte32 = __m256i;

#define readonly_property(type, name)											\
	__declspec(property(get = get_##name))										\
	type name;																	\
	__forceinline type get_##name() const

#define property_get(type, name)												\
	__declspec(property(get = get_##name, put = set_##name))					\
	type name;																	\
	__forceinline type get_##name() const

#define property_set(type, name)												\
	__forceinline void set_##name(type value)

#define fix(value, exponent)					((value) << (exponent))
#define unfix(value, exponent)					((value) >> (exponent))
#define fixf(value, exponent)					((value) * (float)(1 << (exponent)))
#define unfixf(value, exponent)					((value) / (float)(1 << (exponent)))

#define halfspace_mul(value)					((value) << 3)
#define halfspace_div(value)					((value) >> 3)
#define halfspace_mulf(value)					((value) * 8)

#define halfspace_lower(value)					((value) & ~7)
#define halfspace_upper(value)					((value) | 7)
#define halfspace_lower_fix(value, exponent)	((value) & ~((1 << (3 + (exponent))) - 1))
#define halfspace_upper_fix(value, exponent)	((value) | ((1 << (3 + (exponent))) - 1))

#define ATTRIBUTE_Z					0
#define ATTRIBUTE_U					1
#define ATTRIBUTE_V					2
#define ATTRIBUTE_R					3
#define ATTRIBUTE_G					4
#define ATTRIBUTE_B					5
#define ATTRIBUTE_SPECULAR_R		0
#define ATTRIBUTE_SPECULAR_G		1
#define ATTRIBUTE_SPECULAR_B		2
#define ATTRIBUTE_SHADOW_X			0
#define ATTRIBUTE_SHADOW_Y			1
#define ATTRIBUTE_SHADOW_Z			2
#define ATTRIBUTE_SHADOW_R			3
#define ATTRIBUTE_SHADOW_G			4
#define ATTRIBUTE_SHADOW_B			5

alignas(32) static const _vbyte32 BroadcastByteToInt32Mask = _mm256_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12, 16, 16, 16, 16, 20, 20, 20, 20, 24, 24, 24, 24, 28, 28, 28, 28);
alignas(32) static const _vint8 Delta1To8Multiplier = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
alignas(32) static const _vfloat8 Delta1To8MultiplierF = _mm256_setr_ps(1, 2, 3, 4, 5, 6, 7, 8);

static const float MipLevels[] = {
	1.0f / (1 << 0),
	1.0f / (1 << 1),
	1.0f / (1 << 2),
	1.0f / (1 << 3),
	1.0f / (1 << 4),
	1.0f / (1 << 5),
	1.0f / (1 << 6),
	1.0f / (1 << 7)
};