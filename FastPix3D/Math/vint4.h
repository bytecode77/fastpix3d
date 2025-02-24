#pragma once
#include "../FastPix3D.h"
#include "vfloat4.h"

__declspec(align(16)) struct FASTPIX3D_API vint4
{
private:
	union
	{
		int32 M[4];
		_vint4 MM;
	};

public:
	__forceinline vint4() :
		MM(_mm_setzero_si128())
	{
	}
	__forceinline vint4(const vint4 &value) :
		MM(value.MM)
	{
	}
	__forceinline vint4(_vint4 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vint4(const int32 *ptr) :
		MM(_mm_loadu_si128((_vint4*)ptr))
	{
	}
	__forceinline explicit vint4(const vint4 *ptr) :
		MM(_mm_load_si128((_vint4*)ptr))
	{
	}
	__forceinline explicit vint4(const _vint4 *ptr) :
		MM(_mm_load_si128(ptr))
	{
	}
	__forceinline explicit vint4(const vfloat4 &value) :
		MM(_mm_cvtps_epi32(value))
	{
	}
	__forceinline explicit vint4(int32 uniform) :
		MM(_mm_set1_epi32(uniform))
	{
	}
	__forceinline explicit vint4(int32 m0, int32 m1, int32 m2 = 0, int32 m3 = 0) :
		MM(_mm_setr_epi32(m0, m1, m2, m3))
	{
	}

	__forceinline static vint4 Read(const int32 *src, _vuint4 offsets)
	{
		return _mm_i32gather_epi32(src, offsets, 4);
	}
	__forceinline static vint4 Read(const int32 *src, _vuint4 offsets, _vuint4 mask)
	{
		return _mm_mask_i32gather_epi32(vint4(), src, offsets, mask, 4);
	}
	__forceinline static void Write(vint4 *dest, const vint4 &src)
	{
		_mm_store_si128((_vint4*)dest, src.MM);
	}
	__forceinline static void Write(_vint4 *dest, const vint4 &src)
	{
		_mm_store_si128((_vint4*)dest, src.MM);
	}
	__forceinline static void Write(vint4 *dest, const vint4 &src, _vuint4 mask)
	{
		_mm_maskstore_epi32((int32*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vint4 *dest, const vint4 &src, _vuint4 mask)
	{
		_mm_maskstore_epi32((int32*)dest, mask, src.MM);
	}

	__forceinline vint4& operator =(const vint4 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vint4 operator +(const vint4 &other) const
	{
		return _mm_add_epi32(MM, other.MM);
	}
	__forceinline vint4 operator +(int32 scalar) const
	{
		return _mm_add_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator -(const vint4 &other) const
	{
		return _mm_sub_epi32(MM, other.MM);
	}
	__forceinline vint4 operator -(int32 scalar) const
	{
		return _mm_sub_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator -() const
	{
		return _mm_sub_epi32(_mm_setzero_si128(), MM);
	}
	__forceinline vint4 operator *(const vint4 &other) const
	{
		return _mm_mullo_epi32(MM, other.MM);
	}
	__forceinline vint4 operator *(int32 scalar) const
	{
		return _mm_mullo_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator /(const vint4 &other) const
	{
		return _mm_div_epi32(MM, other.MM);
	}
	__forceinline vint4 operator /(int32 scalar) const
	{
		return _mm_div_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator %(const vint4 &other) const
	{
		return _mm_rem_epi32(MM, other.MM);
	}
	__forceinline vint4 operator %(int32 scalar) const
	{
		return _mm_rem_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator ^(const vint4 &other) const
	{
		return _mm_xor_epi32(MM, other.MM);
	}
	__forceinline vint4 operator ^(int32 scalar) const
	{
		return _mm_xor_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator |(const vint4 &other) const
	{
		return _mm_or_epi32(MM, other.MM);
	}
	__forceinline vint4 operator |(int32 scalar) const
	{
		return _mm_or_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator &(const vint4 &other) const
	{
		return _mm_and_epi32(MM, other.MM);
	}
	__forceinline vint4 operator &(int32 scalar) const
	{
		return _mm_and_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vint4 operator ~() const
	{
		return _mm_xor_si128(MM, _mm_set1_epi32(-1));
	}
	__forceinline vint4 operator <<(const vint4 &other) const
	{
		return _mm_sllv_epi32(MM, other.MM);
	}
	__forceinline vint4 operator <<(int32 count) const
	{
		return _mm_slli_epi32(MM, count);
	}
	__forceinline vint4 operator >>(const vint4 &other) const
	{
		return _mm_srav_epi32(MM, other.MM);
	}
	__forceinline vint4 operator >>(int32 count) const
	{
		return _mm_srai_epi32(MM, count);
	}
	__forceinline vint4& operator +=(const vint4 &other)
	{
		MM = _mm_add_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator +=(int32 scalar)
	{
		MM = _mm_add_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator -=(const vint4 &other)
	{
		MM = _mm_sub_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator -=(int32 scalar)
	{
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator *=(const vint4 &other)
	{
		MM = _mm_mullo_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator *=(int32 scalar)
	{
		MM = _mm_mullo_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator /=(const vint4 &other)
	{
		MM = _mm_div_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator /=(int32 scalar)
	{
		MM = _mm_div_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator %=(const vint4 &other)
	{
		MM = _mm_rem_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator %=(int32 scalar)
	{
		MM = _mm_rem_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator ^=(const vint4 &other)
	{
		MM = _mm_xor_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator ^=(int32 scalar)
	{
		MM = _mm_xor_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator |=(const vint4 &other)
	{
		MM = _mm_or_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator |=(int32 scalar)
	{
		MM = _mm_or_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator &=(const vint4 &other)
	{
		MM = _mm_and_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator &=(int32 scalar)
	{
		MM = _mm_and_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vint4& operator <<=(const vint4 &other)
	{
		MM = _mm_sllv_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator <<=(int32 count)
	{
		MM = _mm_slli_epi32(MM, count);
		return *this;
	}
	__forceinline vint4& operator >>=(const vint4 &other)
	{
		MM = _mm_srav_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vint4& operator >>=(int32 count)
	{
		MM = _mm_srai_epi32(MM, count);
		return *this;
	}
	__forceinline vint4& operator ++()
	{
		MM = _mm_add_epi32(MM, _mm_set1_epi32(1));
		return *this;
	}
	__forceinline vint4 operator ++(int)
	{
		_vint4 value = MM;
		MM = _mm_add_epi32(MM, _mm_set1_epi32(1));
		return value;
	}
	__forceinline vint4& operator --()
	{
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(1));
		return *this;
	}
	__forceinline vint4 operator --(int)
	{
		_vint4 value = MM;
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(1));
		return value;
	}
	__forceinline bool operator ==(const vint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, other.MM)) == 0xffff;
	}
	__forceinline bool operator ==(int32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, _mm_set1_epi32(scalar))) == 0xffff;
	}
	__forceinline bool operator !=(const vint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, other.MM)) != 0xffff;
	}
	__forceinline bool operator !=(int32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, _mm_set1_epi32(scalar))) != 0xffff;
	}
	__forceinline bool operator <(const vint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmplt_epi32(MM, other.MM)) == 0xffff;
	}
	__forceinline bool operator <(int32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmplt_epi32(MM, _mm_set1_epi32(scalar))) == 0xffff;
	}
	__forceinline bool operator >(const vint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi32(MM, other.MM)) == 0xffff;
	}
	__forceinline bool operator >(int32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi32(MM, _mm_set1_epi32(scalar))) == 0xffff;
	}
	__forceinline bool operator <=(const vint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi32(MM, other.MM)) == 0;
	}
	__forceinline bool operator <=(int32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi32(MM, _mm_set1_epi32(scalar))) == 0;
	}
	__forceinline bool operator >=(const vint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmplt_epi32(MM, other.MM)) == 0;
	}
	__forceinline bool operator >=(int32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmplt_epi32(MM, _mm_set1_epi32(scalar))) == 0;
	}
	__forceinline int32& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline int32 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vint4() const
	{
		return MM;
	}
	__forceinline operator vfloat4() const
	{
		return _mm_cvtepi32_ps(MM);
	}
	__forceinline operator _vfloat4() const
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

using vfix4 = vint4;