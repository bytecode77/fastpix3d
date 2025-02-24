#pragma once
#include "../FastPix3D.h"
#include "vushort16.h"

__declspec(align(32)) struct FASTPIX3D_API vbyte32
{
private:
	union
	{
		byte M[32];
		_vbyte32 MM;
	};

public:
	readonly_property(vushort16, Low8)
	{
		return _mm256_unpacklo_epi8(MM, _mm256_setzero_si256());
	}
	readonly_property(vushort16, High8)
	{
		return _mm256_unpackhi_epi8(MM, _mm256_setzero_si256());
	}

	__forceinline vbyte32() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vbyte32(const vbyte32 &value) :
		MM(value.MM)
	{
	}
	__forceinline vbyte32(_vbyte32 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vbyte32(const byte *ptr) :
		MM(_mm256_loadu_si256((_vbyte32*)ptr))
	{
	}
	__forceinline explicit vbyte32(const vbyte32 *ptr) :
		MM(_mm256_load_si256((_vbyte32*)ptr))
	{
	}
	__forceinline explicit vbyte32(const _vbyte32 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vbyte32(byte uniform) :
		MM(_mm256_set1_epi8(uniform))
	{
	}
	__forceinline explicit vbyte32(byte m0, byte m1, byte m2 = 0, byte m3 = 0, byte m4 = 0, byte m5 = 0, byte m6 = 0, byte m7 = 0, byte m8 = 0, byte m9 = 0, byte m10 = 0, byte m11 = 0, byte m12 = 0, byte m13 = 0, byte m14 = 0, byte m15 = 0, byte m16 = 0, byte m17 = 0, byte m18 = 0, byte m19 = 0, byte m20 = 0, byte m21 = 0, byte m22 = 0, byte m23 = 0, byte m24 = 0, byte m25 = 0, byte m26 = 0, byte m27 = 0, byte m28 = 0, byte m29 = 0, byte m30 = 0, byte m31 = 0) :
		MM(_mm256_setr_epi8(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31))
	{
	}

	__forceinline static void Write(vbyte32 *dest, const vbyte32 &src)
	{
		_mm256_store_si256((_vbyte32*)dest, src.MM);
	}
	__forceinline static void Write(_vbyte32 *dest, const vbyte32 &src)
	{
		_mm256_store_si256((_vbyte32*)dest, src.MM);
	}

	__forceinline vbyte32& operator =(const vbyte32 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vbyte32 operator +(const vbyte32 &other) const
	{
		return _mm256_add_epi8(MM, other.MM);
	}
	__forceinline vbyte32 operator +(byte scalar) const
	{
		return _mm256_add_epi8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator -(const vbyte32 &other) const
	{
		return _mm256_sub_epi8(MM, other.MM);
	}
	__forceinline vbyte32 operator -(byte scalar) const
	{
		return _mm256_sub_epi8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator /(const vbyte32 &other) const
	{
		return _mm256_div_epu8(MM, other.MM);
	}
	__forceinline vbyte32 operator /(byte scalar) const
	{
		return _mm256_div_epu8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator %(const vbyte32 &other) const
	{
		return _mm256_rem_epu8(MM, other.MM);
	}
	__forceinline vbyte32 operator %(byte scalar) const
	{
		return _mm256_rem_epu8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator ^(const vbyte32 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vbyte32 operator ^(byte scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator |(const vbyte32 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vbyte32 operator |(byte scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator &(const vbyte32 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vbyte32 operator &(byte scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vbyte32 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi8(-1));
	}
	__forceinline vbyte32& operator +=(const vbyte32 &other)
	{
		MM = _mm256_add_epi8(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator +=(byte scalar)
	{
		MM = _mm256_add_epi8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator -=(const vbyte32 &other)
	{
		MM = _mm256_sub_epi8(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator -=(byte scalar)
	{
		MM = _mm256_sub_epi8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator /=(const vbyte32 &other)
	{
		MM = _mm256_div_epu8(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator /=(byte scalar)
	{
		MM = _mm256_div_epu8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator %=(const vbyte32 &other)
	{
		MM = _mm256_rem_epu8(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator %=(byte scalar)
	{
		MM = _mm256_rem_epu8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator ^=(const vbyte32 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator ^=(byte scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator |=(const vbyte32 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator |=(byte scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator &=(const vbyte32 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vbyte32& operator &=(byte scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vbyte32& operator ++()
	{
		MM = _mm256_add_epi8(MM, _mm256_set1_epi8(1));
		return *this;
	}
	__forceinline vbyte32 operator ++(int)
	{
		_vbyte32 value = MM;
		MM = _mm256_add_epi8(MM, _mm256_set1_epi8(1));
		return value;
	}
	__forceinline vbyte32& operator --()
	{
		MM = _mm256_sub_epi8(MM, _mm256_set1_epi8(1));
		return *this;
	}
	__forceinline vbyte32 operator --(int)
	{
		_vbyte32 value = MM;
		MM = _mm256_sub_epi8(MM, _mm256_set1_epi8(1));
		return value;
	}
	__forceinline bool operator ==(const vbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(byte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, _mm256_set1_epi8(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(byte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, _mm256_set1_epi8(scalar))) != 0xffffffff;
	}
	__forceinline byte& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline byte operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vbyte32() const
	{
		return MM;
	}
	void* operator new[](size_t size)
	{
		return _aligned_malloc(size, 32);
	}
	void operator delete[](void *ptr)
	{
		if (ptr)
		{
			_aligned_free(ptr);
		}
	}
};