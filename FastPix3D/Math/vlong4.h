#pragma once
#include "../FastPix3D.h"

__declspec(align(32)) struct FASTPIX3D_API vlong4
{
private:
	union
	{
		int64 M[4];
		_vlong4 MM;
	};

public:
	__forceinline vlong4() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vlong4(const vlong4 &value) :
		MM(value.MM)
	{
	}
	__forceinline vlong4(_vlong4 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vlong4(const int64 *ptr) :
		MM(_mm256_loadu_si256((_vlong4*)ptr))
	{
	}
	__forceinline explicit vlong4(const vlong4 *ptr) :
		MM(_mm256_load_si256((_vlong4*)ptr))
	{
	}
	__forceinline explicit vlong4(const _vlong4 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vlong4(int64 uniform) :
		MM(_mm256_set1_epi64x(uniform))
	{
	}
	__forceinline explicit vlong4(int64 m0, int64 m1, int64 m2 = 0, int64 m3 = 0) :
		MM(_mm256_set_epi64x(m3, m2, m1, m0))
	{
	}

	__forceinline static vlong4 Read(const int64 *src, vlong4 offsets)
	{
		return _mm256_i64gather_epi64(src, offsets, 8);
	}
	__forceinline static vlong4 Read(const int64 *src, vlong4 offsets, vlong4 mask)
	{
		return _mm256_mask_i64gather_epi64(vlong4(), src, offsets, mask, 8);
	}
	__forceinline static void Write(vlong4 *dest, const vlong4 &src)
	{
		_mm256_store_si256((_vlong4*)dest, src.MM);
	}
	__forceinline static void Write(_vlong4 *dest, const vlong4 &src)
	{
		_mm256_store_si256((_vlong4*)dest, src.MM);
	}
	__forceinline static void Write(vlong4 *dest, const vlong4 &src, vlong4 mask)
	{
		_mm256_maskstore_epi64((int64*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vlong4 *dest, const vlong4 &src, vlong4 mask)
	{
		_mm256_maskstore_epi64((int64*)dest, mask, src.MM);
	}

	__forceinline vlong4& operator =(const vlong4 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vlong4 operator +(const vlong4 &other) const
	{
		return _mm256_add_epi64(MM, other.MM);
	}
	__forceinline vlong4 operator +(int64 scalar) const
	{
		return _mm256_add_epi64(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator -(const vlong4 &other) const
	{
		return _mm256_sub_epi64(MM, other.MM);
	}
	__forceinline vlong4 operator -(int64 scalar) const
	{
		return _mm256_sub_epi64(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator -() const
	{
		return _mm256_sub_epi64(_mm256_setzero_si256(), MM);
	}
	__forceinline vlong4 operator /(const vlong4 &other) const
	{
		return _mm256_div_epi64(MM, other.MM);
	}
	__forceinline vlong4 operator /(int64 scalar) const
	{
		return _mm256_div_epi64(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator %(const vlong4 &other) const
	{
		return _mm256_rem_epi64(MM, other.MM);
	}
	__forceinline vlong4 operator %(int64 scalar) const
	{
		return _mm256_rem_epi64(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator ^(const vlong4 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vlong4 operator ^(int64 scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator |(const vlong4 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vlong4 operator |(int64 scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator &(const vlong4 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vlong4 operator &(int64 scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi64x(scalar));
	}
	__forceinline vlong4 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi64x(-1));
	}
	__forceinline vlong4& operator +=(const vlong4 &other)
	{
		MM = _mm256_add_epi64(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator +=(int64 scalar)
	{
		MM = _mm256_add_epi64(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator -=(const vlong4 &other)
	{
		MM = _mm256_sub_epi64(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator -=(int64 scalar)
	{
		MM = _mm256_sub_epi64(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator /=(const vlong4 &other)
	{
		MM = _mm256_div_epi64(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator /=(int64 scalar)
	{
		MM = _mm256_div_epi64(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator %=(const vlong4 &other)
	{
		MM = _mm256_rem_epi64(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator %=(int64 scalar)
	{
		MM = _mm256_rem_epi64(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator ^=(const vlong4 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator ^=(int64 scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator |=(const vlong4 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator |=(int64 scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator &=(const vlong4 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vlong4& operator &=(int64 scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi64x(scalar));
		return *this;
	}
	__forceinline vlong4& operator ++()
	{
		MM = _mm256_add_epi64(MM, _mm256_set1_epi64x(1));
		return *this;
	}
	__forceinline vlong4 operator ++(int)
	{
		_vlong4 value = MM;
		MM = _mm256_add_epi64(MM, _mm256_set1_epi64x(1));
		return value;
	}
	__forceinline vlong4& operator --()
	{
		MM = _mm256_sub_epi64(MM, _mm256_set1_epi64x(1));
		return *this;
	}
	__forceinline vlong4 operator --(int)
	{
		_vlong4 value = MM;
		MM = _mm256_sub_epi64(MM, _mm256_set1_epi64x(1));
		return value;
	}
	__forceinline bool operator ==(const vlong4 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi64(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(int64 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi64(MM, _mm256_set1_epi64x(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vlong4 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi64(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(int64 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi64(MM, _mm256_set1_epi64x(scalar))) != 0xffffffff;
	}
	__forceinline bool operator <(const vlong4 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(other.MM, MM)) == 0xffffffff;
	}
	__forceinline bool operator <(int64 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(_mm256_set1_epi64x(scalar), MM)) == 0xffffffff;
	}
	__forceinline bool operator >(const vlong4 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator >(int64 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(MM, _mm256_set1_epi64x(scalar))) == 0xffffffff;
	}
	__forceinline bool operator <=(const vlong4 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(MM, other.MM)) == 0;
	}
	__forceinline bool operator <=(int64 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(MM, _mm256_set1_epi64x(scalar))) == 0;
	}
	__forceinline bool operator >=(const vlong4 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(other.MM, MM)) == 0;
	}
	__forceinline bool operator >=(int64 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(_mm256_set1_epi64x(scalar), MM)) == 0;
	}
	__forceinline int64& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline int64 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vlong4() const
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

using vfixlong4 = vlong4;