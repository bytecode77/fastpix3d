#pragma once
#include "../FastPix3D.h"
#include "vshort16.h"

__declspec(align(32)) struct FASTPIX3D_API vsbyte32
{
private:
	union
	{
		sbyte M[32];
		_vsbyte32 MM;
	};

public:
	readonly_property(vshort16, Low8)
	{
		return _mm256_unpacklo_epi8(MM, _mm256_setzero_si256());
	}
	readonly_property(vshort16, High8)
	{
		return _mm256_unpackhi_epi8(MM, _mm256_setzero_si256());
	}

	__forceinline vsbyte32() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vsbyte32(const vsbyte32 &value) :
		MM(value.MM)
	{
	}
	__forceinline vsbyte32(_vsbyte32 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vsbyte32(const sbyte *ptr) :
		MM(_mm256_loadu_si256((_vsbyte32*)ptr))
	{
	}
	__forceinline explicit vsbyte32(const vsbyte32 *ptr) :
		MM(_mm256_load_si256((_vsbyte32*)ptr))
	{
	}
	__forceinline explicit vsbyte32(const _vsbyte32 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vsbyte32(sbyte uniform) :
		MM(_mm256_set1_epi8(uniform))
	{
	}
	__forceinline explicit vsbyte32(sbyte m0, sbyte m1, sbyte m2 = 0, sbyte m3 = 0, sbyte m4 = 0, sbyte m5 = 0, sbyte m6 = 0, sbyte m7 = 0, sbyte m8 = 0, sbyte m9 = 0, sbyte m10 = 0, sbyte m11 = 0, sbyte m12 = 0, sbyte m13 = 0, sbyte m14 = 0, sbyte m15 = 0, sbyte m16 = 0, sbyte m17 = 0, sbyte m18 = 0, sbyte m19 = 0, sbyte m20 = 0, sbyte m21 = 0, sbyte m22 = 0, sbyte m23 = 0, sbyte m24 = 0, sbyte m25 = 0, sbyte m26 = 0, sbyte m27 = 0, sbyte m28 = 0, sbyte m29 = 0, sbyte m30 = 0, sbyte m31 = 0) :
		MM(_mm256_setr_epi8(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31))
	{
	}

	__forceinline static void Write(vsbyte32 *dest, const vsbyte32 &src)
	{
		_mm256_store_si256((_vsbyte32*)dest, src.MM);
	}
	__forceinline static void Write(_vsbyte32 *dest, const vsbyte32 &src)
	{
		_mm256_store_si256((_vsbyte32*)dest, src.MM);
	}

	__forceinline vsbyte32& operator =(const vsbyte32 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vsbyte32 operator +(const vsbyte32 &other) const
	{
		return _mm256_add_epi8(MM, other.MM);
	}
	__forceinline vsbyte32 operator +(sbyte scalar) const
	{
		return _mm256_add_epi8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator -(const vsbyte32 &other) const
	{
		return _mm256_sub_epi8(MM, other.MM);
	}
	__forceinline vsbyte32 operator -(sbyte scalar) const
	{
		return _mm256_sub_epi8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator -() const
	{
		return _mm256_sub_epi8(_mm256_setzero_si256(), MM);
	}
	__forceinline vsbyte32 operator /(const vsbyte32 &other) const
	{
		return _mm256_div_epi8(MM, other.MM);
	}
	__forceinline vsbyte32 operator /(sbyte scalar) const
	{
		return _mm256_div_epi8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator %(const vsbyte32 &other) const
	{
		return _mm256_rem_epi8(MM, other.MM);
	}
	__forceinline vsbyte32 operator %(sbyte scalar) const
	{
		return _mm256_rem_epi8(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator ^(const vsbyte32 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vsbyte32 operator ^(sbyte scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator |(const vsbyte32 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vsbyte32 operator |(sbyte scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator &(const vsbyte32 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vsbyte32 operator &(sbyte scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi8(scalar));
	}
	__forceinline vsbyte32 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi8(-1));
	}
	__forceinline vsbyte32& operator +=(const vsbyte32 &other)
	{
		MM = _mm256_add_epi8(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator +=(sbyte scalar)
	{
		MM = _mm256_add_epi8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator -=(const vsbyte32 &other)
	{
		MM = _mm256_sub_epi8(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator -=(sbyte scalar)
	{
		MM = _mm256_sub_epi8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator /=(const vsbyte32 &other)
	{
		MM = _mm256_div_epi8(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator /=(sbyte scalar)
	{
		MM = _mm256_div_epi8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator %=(const vsbyte32 &other)
	{
		MM = _mm256_rem_epi8(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator %=(sbyte scalar)
	{
		MM = _mm256_rem_epi8(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator ^=(const vsbyte32 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator ^=(sbyte scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator |=(const vsbyte32 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator |=(sbyte scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator &=(const vsbyte32 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vsbyte32& operator &=(sbyte scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi8(scalar));
		return *this;
	}
	__forceinline vsbyte32& operator ++()
	{
		MM = _mm256_add_epi8(MM, _mm256_set1_epi8(1));
		return *this;
	}
	__forceinline vsbyte32 operator ++(int)
	{
		_vsbyte32 value = MM;
		MM = _mm256_add_epi8(MM, _mm256_set1_epi8(1));
		return value;
	}
	__forceinline vsbyte32& operator --()
	{
		MM = _mm256_sub_epi8(MM, _mm256_set1_epi8(1));
		return *this;
	}
	__forceinline vsbyte32 operator --(int)
	{
		_vsbyte32 value = MM;
		MM = _mm256_sub_epi8(MM, _mm256_set1_epi8(1));
		return value;
	}
	__forceinline bool operator ==(const vsbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(sbyte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, _mm256_set1_epi8(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vsbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(sbyte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(MM, _mm256_set1_epi8(scalar))) != 0xffffffff;
	}
	__forceinline bool operator <(const vsbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(other.MM, MM)) == 0xffffffff;
	}
	__forceinline bool operator <(sbyte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_set1_epi8(scalar), MM)) == 0xffffffff;
	}
	__forceinline bool operator >(const vsbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator >(sbyte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(MM, _mm256_set1_epi8(scalar))) == 0xffffffff;
	}
	__forceinline bool operator <=(const vsbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(MM, other.MM)) == 0;
	}
	__forceinline bool operator <=(sbyte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(MM, _mm256_set1_epi8(scalar))) == 0;
	}
	__forceinline bool operator >=(const vsbyte32 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(other.MM, MM)) == 0;
	}
	__forceinline bool operator >=(sbyte scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_set1_epi8(scalar), MM)) == 0;
	}
	__forceinline sbyte& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline sbyte operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vsbyte32() const
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