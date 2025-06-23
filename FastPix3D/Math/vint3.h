#pragma once
#include "../FastPix3D.h"
#include "vfloat3.h"

__declspec(align(16)) struct FASTPIX3D_API vint3
{
	union
	{
		struct
		{
			int32 X;
			int32 Y;
			int32 Z;
		};
		_vint3 MM;
	};

	readonly_property(float, Length)
	{
		return sqrtf((float)(X * X + Y * Y + Z * Z));
	}
	readonly_property(int32, SquaredLength)
	{
		return X * X + Y * Y + Z * Z;
	}
	readonly_property(float, InverseLength)
	{
		return __rsqrt_ss((float)(X * X + Y * Y + Z * Z));
	}
	readonly_property(float, InverseSquaredLength)
	{
		float inverseLength = __rsqrt_ss((float)(X * X + Y * Y + Z * Z));
		return inverseLength * inverseLength;
	}

	__forceinline vint3() :
		MM(_mm_setzero_si128())
	{
	}
	__forceinline vint3(const vint3 &value) :
		MM(value.MM)
	{
	}
	__forceinline vint3(_vint3 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vint3(const int32 *ptr) :
		MM(_mm_loadu_si128((_vint3*)ptr))
	{
	}
	__forceinline explicit vint3(const vint3 *ptr) :
		MM(_mm_load_si128((_vint3*)ptr))
	{
	}
	__forceinline explicit vint3(const _vint3 *ptr) :
		MM(_mm_load_si128(ptr))
	{
	}
	__forceinline explicit vint3(const vfloat3 &value) :
		MM(_mm_cvtps_epi32(value))
	{
	}
	__forceinline explicit vint3(int32 uniform) :
		MM(_mm_setr_epi32(uniform, uniform, uniform, 0))
	{
	}
	__forceinline explicit vint3(int32 x, int32 y, int32 z) :
		MM(_mm_setr_epi32(x, y, z, 0))
	{
	}

	__forceinline int32 DotProduct(const vint3 &other) const
	{
		return X * other.X + Y * other.Y + Z * other.Z;
	}
	__forceinline vint3 CrossProduct(const vint3 &other) const
	{
		return _mm_sub_epi32(
			_mm_mullo_epi32(_mm_shuffle_epi32(MM, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_epi32(other.MM, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mullo_epi32(_mm_shuffle_epi32(MM, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_epi32(other.MM, _MM_SHUFFLE(3, 0, 2, 1)))
		);
	}

	__forceinline vint3& operator =(const vint3 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vint3 operator +(const vint3 &other) const
	{
		return _mm_add_epi32(MM, other.MM);
	}
	__forceinline vint3 operator +(int32 scalar) const
	{
		return _mm_add_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint3 operator -(const vint3 &other) const
	{
		return _mm_sub_epi32(MM, other.MM);
	}
	__forceinline vint3 operator -(int32 scalar) const
	{
		return _mm_sub_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint3 operator -() const
	{
		return _mm_sub_epi32(_mm_setzero_si128(), MM);
	}
	__forceinline vint3 operator *(const vint3 &other) const
	{
		return _mm_mullo_epi32(MM, other.MM);
	}
	__forceinline vint3 operator *(int32 scalar) const
	{
		return _mm_mullo_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint3 operator /(const vint3 &other) const
	{
		return _mm_div_epi32(MM, _mm_or_si128(other.MM, _mm_set_epi32(0x7fffffff, 0, 0, 0)));
	}
	__forceinline vint3 operator /(int32 scalar) const
	{
		return _mm_div_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint3& operator +=(const vint3 &other)
	{
		MM = _mm_add_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint3& operator +=(int32 scalar)
	{
		MM = _mm_add_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint3& operator -=(const vint3 &other)
	{
		MM = _mm_sub_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint3& operator -=(int32 scalar)
	{
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint3& operator *=(const vint3 &other)
	{
		MM = _mm_mullo_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint3& operator *=(int32 scalar)
	{
		MM = _mm_mullo_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint3& operator /=(const vint3 &other)
	{
		MM = _mm_div_epi32(MM, _mm_or_si128(other.MM, _mm_set_epi32(0x7fffffff, 0, 0, 0)));
		return *this;
	}
	__forceinline vint3& operator /=(int32 scalar)
	{
		MM = _mm_div_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline bool operator ==(const vint3 &other) const
	{
		return (_mm_movemask_epi8(_mm_cmpeq_epi32(MM, other.MM)) & 0xfff) == 0xfff;
	}
	__forceinline bool operator ==(int32 scalar) const
	{
		return (_mm_movemask_epi8(_mm_cmpeq_epi32(MM, _mm_set1_epi32(scalar))) & 0xfff) == 0xfff;
	}
	__forceinline bool operator !=(const vint3 &other) const
	{
		return (_mm_movemask_epi8(_mm_cmpeq_epi32(MM, other.MM)) & 0xfff) != 0xfff;
	}
	__forceinline bool operator !=(int32 scalar) const
	{
		return (_mm_movemask_epi8(_mm_cmpeq_epi32(MM, _mm_set1_epi32(scalar))) & 0xfff) != 0xfff;
	}
	__forceinline bool operator <(const vint3 &other) const
	{
		return (_mm_movemask_epi8(_mm_cmplt_epi32(MM, other.MM)) & 0xfff) == 0xfff;
	}
	__forceinline bool operator <(int32 scalar) const
	{
		return (_mm_movemask_epi8(_mm_cmplt_epi32(MM, _mm_set1_epi32(scalar))) & 0xfff) == 0xfff;
	}
	__forceinline bool operator >(const vint3 &other) const
	{
		return (_mm_movemask_epi8(_mm_cmpgt_epi32(MM, other.MM)) & 0xfff) == 0xfff;
	}
	__forceinline bool operator >(int32 scalar) const
	{
		return (_mm_movemask_epi8(_mm_cmpgt_epi32(MM, _mm_set1_epi32(scalar))) & 0xfff) == 0xfff;
	}
	__forceinline bool operator <=(const vint3 &other) const
	{
		return (_mm_movemask_epi8(_mm_cmpgt_epi32(MM, other.MM)) & 0xfff) == 0;
	}
	__forceinline bool operator <=(int32 scalar) const
	{
		return (_mm_movemask_epi8(_mm_cmpgt_epi32(MM, _mm_set1_epi32(scalar))) & 0xfff) == 0;
	}
	__forceinline bool operator >=(const vint3 &other) const
	{
		return (_mm_movemask_epi8(_mm_cmplt_epi32(MM, other.MM)) & 0xfff) == 0;
	}
	__forceinline bool operator >=(int32 scalar) const
	{
		return (_mm_movemask_epi8(_mm_cmplt_epi32(MM, _mm_set1_epi32(scalar))) & 0xfff) == 0;
	}
	__forceinline operator _vint3() const
	{
		return MM;
	}
	__forceinline operator vfloat3() const
	{
		return _mm_cvtepi32_ps(MM);
	}
	__forceinline operator _vfloat3() const
	{
		return _mm_cvtepi32_ps(MM);
	}
	void* operator new[](size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete[](void *ptr)
	{
		if (ptr)
		{
			_aligned_free(ptr);
		}
	}
};

using vfix3 = vint3;