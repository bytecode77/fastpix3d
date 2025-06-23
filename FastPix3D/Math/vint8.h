#pragma once
#include "../FastPix3D.h"
#include "vfloat8.h"
#include "vshort16.h"

__declspec(align(32)) struct FASTPIX3D_API vint8
{
private:
	union
	{
		int32 M[8];
		_vint8 MM;
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
	readonly_property(vint8, Low16)
	{
		return _mm256_unpacklo_epi16(MM, _mm256_setzero_si256());
	}
	readonly_property(vint8, High16)
	{
		return _mm256_unpackhi_epi16(MM, _mm256_setzero_si256());
	}

	__forceinline vint8() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vint8(const vint8 &value) :
		MM(value.MM)
	{
	}
	__forceinline vint8(_vint8 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vint8(const int32 *ptr) :
		MM(_mm256_loadu_si256((_vint8*)ptr))
	{
	}
	__forceinline explicit vint8(const vint8 *ptr) :
		MM(_mm256_load_si256((_vint8*)ptr))
	{
	}
	__forceinline explicit vint8(const _vint8 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vint8(const vfloat8 &value) :
		MM(_mm256_cvtps_epi32(value))
	{
	}
	__forceinline explicit vint8(int32 uniform) :
		MM(_mm256_set1_epi32(uniform))
	{
	}
	__forceinline explicit vint8(int32 m0, int32 m1, int32 m2 = 0, int32 m3 = 0, int32 m4 = 0, int32 m5 = 0, int32 m6 = 0, int32 m7 = 0) :
		MM(_mm256_setr_epi32(m0, m1, m2, m3, m4, m5, m6, m7))
	{
	}

	__forceinline static vint8 Read(const int32 *src, _vuint8 offsets)
	{
		return _mm256_i32gather_epi32(src, offsets, 4);
	}
	__forceinline static vint8 Read(const int32 *src, _vuint8 offsets, _vuint8 mask)
	{
		return _mm256_mask_i32gather_epi32(vint8(), src, offsets, mask, 4);
	}
	__forceinline static void Write(vint8 *dest, const vint8 &src)
	{
		_mm256_store_si256((_vint8*)dest, src.MM);
	}
	__forceinline static void Write(_vint8 *dest, const vint8 &src)
	{
		_mm256_store_si256((_vint8*)dest, src.MM);
	}
	__forceinline static void Write(vint8 *dest, const vint8 &src, _vuint8 mask)
	{
		_mm256_maskstore_epi32((int32*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vint8 *dest, const vint8 &src, _vuint8 mask)
	{
		_mm256_maskstore_epi32((int32*)dest, mask, src.MM);
	}

	__forceinline vint8& operator =(const vint8 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vint8 operator +(const vint8 &other) const
	{
		return _mm256_add_epi32(MM, other.MM);
	}
	__forceinline vint8 operator +(int32 scalar) const
	{
		return _mm256_add_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator -(const vint8 &other) const
	{
		return _mm256_sub_epi32(MM, other.MM);
	}
	__forceinline vint8 operator -(int32 scalar) const
	{
		return _mm256_sub_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator -() const
	{
		return _mm256_sub_epi32(_mm256_setzero_si256(), MM);
	}
	__forceinline vint8 operator *(const vint8 &other) const
	{
		return _mm256_mullo_epi32(MM, other.MM);
	}
	__forceinline vint8 operator *(int32 scalar) const
	{
		return _mm256_mullo_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator /(const vint8 &other) const
	{
		return _mm256_div_epi32(MM, other.MM);
	}
	__forceinline vint8 operator /(int32 scalar) const
	{
		return _mm256_div_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator %(const vint8 &other) const
	{
		return _mm256_rem_epi32(MM, other.MM);
	}
	__forceinline vint8 operator %(int32 scalar) const
	{
		return _mm256_rem_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator ^(const vint8 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vint8 operator ^(int32 scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator |(const vint8 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vint8 operator |(int32 scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator &(const vint8 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vint8 operator &(int32 scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vint8 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi32(-1));
	}
	__forceinline vint8 operator <<(const vint8 &other) const
	{
		return _mm256_sllv_epi32(MM, other.MM);
	}
	__forceinline vint8 operator <<(int32 count) const
	{
		return _mm256_slli_epi32(MM, count);
	}
	__forceinline vint8 operator >>(const vint8 &other) const
	{
		return _mm256_srav_epi32(MM, other.MM);
	}
	__forceinline vint8 operator >>(int32 count) const
	{
		return _mm256_srai_epi32(MM, count);
	}
	__forceinline vint8& operator +=(const vint8 &other)
	{
		MM = _mm256_add_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator +=(int32 scalar)
	{
		MM = _mm256_add_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator -=(const vint8 &other)
	{
		MM = _mm256_sub_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator -=(int32 scalar)
	{
		MM = _mm256_sub_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator *=(const vint8 &other)
	{
		MM = _mm256_mullo_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator *=(int32 scalar)
	{
		MM = _mm256_mullo_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator /=(const vint8 &other)
	{
		MM = _mm256_div_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator /=(int32 scalar)
	{
		MM = _mm256_div_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator %=(const vint8 &other)
	{
		MM = _mm256_rem_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator %=(int32 scalar)
	{
		MM = _mm256_rem_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator ^=(const vint8 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator ^=(int32 scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator |=(const vint8 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator |=(int32 scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator &=(const vint8 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator &=(int32 scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint8& operator <<=(const vint8 &other)
	{
		MM = _mm256_sllv_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator <<=(int32 count)
	{
		MM = _mm256_slli_epi32(MM, count);
		return *this;
	}
	__forceinline vint8& operator >>=(const vint8 &other)
	{
		MM = _mm256_srav_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint8& operator >>=(int32 count)
	{
		MM = _mm256_srai_epi32(MM, count);
		return *this;
	}
	__forceinline vint8& operator ++()
	{
		MM = _mm256_add_epi32(MM, _mm256_set1_epi32(1));
		return *this;
	}
	__forceinline vint8 operator ++(int)
	{
		_vint8 value = MM;
		MM = _mm256_add_epi32(MM, _mm256_set1_epi32(1));
		return value;
	}
	__forceinline vint8& operator --()
	{
		MM = _mm256_sub_epi32(MM, _mm256_set1_epi32(1));
		return *this;
	}
	__forceinline vint8 operator --(int)
	{
		_vint8 value = MM;
		MM = _mm256_sub_epi32(MM, _mm256_set1_epi32(1));
		return value;
	}
	__forceinline bool operator ==(const vint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(int32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, _mm256_set1_epi32(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(int32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, _mm256_set1_epi32(scalar))) != 0xffffffff;
	}
	__forceinline bool operator <(const vint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(other.MM, MM)) == 0xffffffff;
	}
	__forceinline bool operator <(int32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(_mm256_set1_epi32(scalar), MM)) == 0xffffffff;
	}
	__forceinline bool operator >(const vint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator >(int32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(MM, _mm256_set1_epi32(scalar))) == 0xffffffff;
	}
	__forceinline bool operator <=(const vint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(MM, other.MM)) == 0;
	}
	__forceinline bool operator <=(int32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(MM, _mm256_set1_epi32(scalar))) == 0;
	}
	__forceinline bool operator >=(const vint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(other.MM, MM)) == 0;
	}
	__forceinline bool operator >=(int32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(_mm256_set1_epi32(scalar), MM)) == 0;
	}
	__forceinline int32& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline int32 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vint8() const
	{
		return MM;
	}
	__forceinline operator vfloat8() const
	{
		return _mm256_cvtepi32_ps(MM);
	}
	__forceinline operator _vfloat8() const
	{
		return _mm256_cvtepi32_ps(MM);
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

using vfix8 = vint8;