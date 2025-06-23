#pragma once
#include "../FastPix3D.h"

__declspec(align(32)) struct FASTPIX3D_API vshort16
{
private:
	union
	{
		int16 M[16];
		_vshort16 MM;
	};

public:
	__forceinline vshort16() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vshort16(const vshort16 &value) :
		MM(value.MM)
	{
	}
	__forceinline vshort16(_vshort16 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vshort16(const int16 *ptr) :
		MM(_mm256_loadu_si256((_vshort16*)ptr))
	{
	}
	__forceinline explicit vshort16(const vshort16 *ptr) :
		MM(_mm256_load_si256((_vshort16*)ptr))
	{
	}
	__forceinline explicit vshort16(const _vshort16 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vshort16(int16 uniform) :
		MM(_mm256_set1_epi16(uniform))
	{
	}
	__forceinline explicit vshort16(int16 m0, int16 m1, int16 m2 = 0, int16 m3 = 0, int16 m4 = 0, int16 m5 = 0, int16 m6 = 0, int16 m7 = 0, int16 m8 = 0, int16 m9 = 0, int16 m10 = 0, int16 m11 = 0, int16 m12 = 0, int16 m13 = 0, int16 m14 = 0, int16 m15 = 0) :
		MM(_mm256_setr_epi16(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15))
	{
	}

	__forceinline static void Write(vshort16 *dest, const vshort16 &src)
	{
		_mm256_store_si256((_vshort16*)dest, src.MM);
	}
	__forceinline static void Write(_vshort16 *dest, const vshort16 &src)
	{
		_mm256_store_si256((_vshort16*)dest, src.MM);
	}

	__forceinline vshort16& operator =(const vshort16 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vshort16 operator +(const vshort16 &other) const
	{
		return _mm256_add_epi16(MM, other.MM);
	}
	__forceinline vshort16 operator +(int16 scalar) const
	{
		return _mm256_add_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator -(const vshort16 &other) const
	{
		return _mm256_sub_epi16(MM, other.MM);
	}
	__forceinline vshort16 operator -(int16 scalar) const
	{
		return _mm256_sub_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator -() const
	{
		return _mm256_sub_epi16(_mm256_setzero_si256(), MM);
	}
	__forceinline vshort16 operator *(const vshort16 &other) const
	{
		return _mm256_mullo_epi16(MM, other.MM);
	}
	__forceinline vshort16 operator *(int16 scalar) const
	{
		return _mm256_mullo_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator /(const vshort16 &other) const
	{
		return _mm256_div_epi16(MM, other.MM);
	}
	__forceinline vshort16 operator /(int16 scalar) const
	{
		return _mm256_div_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator %(const vshort16 &other) const
	{
		return _mm256_rem_epi16(MM, other.MM);
	}
	__forceinline vshort16 operator %(int16 scalar) const
	{
		return _mm256_rem_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator ^(const vshort16 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vshort16 operator ^(int16 scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator |(const vshort16 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vshort16 operator |(int16 scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator &(const vshort16 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vshort16 operator &(int16 scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vshort16 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi16(-1));
	}
	__forceinline vshort16 operator <<(int32 count) const
	{
		return _mm256_slli_epi16(MM, count);
	}
	__forceinline vshort16 operator >>(int32 count) const
	{
		return _mm256_srai_epi16(MM, count);
	}
	__forceinline vshort16& operator +=(const vshort16 &other)
	{
		MM = _mm256_add_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator +=(int16 scalar)
	{
		MM = _mm256_add_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator -=(const vshort16 &other)
	{
		MM = _mm256_sub_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator -=(int16 scalar)
	{
		MM = _mm256_sub_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator *=(const vshort16 &other)
	{
		MM = _mm256_mullo_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator *=(int16 scalar)
	{
		MM = _mm256_mullo_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator /=(const vshort16 &other)
	{
		MM = _mm256_div_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator /=(int16 scalar)
	{
		MM = _mm256_div_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator %=(const vshort16 &other)
	{
		MM = _mm256_rem_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator %=(int16 scalar)
	{
		MM = _mm256_rem_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator ^=(const vshort16 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator ^=(int16 scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator |=(const vshort16 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator |=(int16 scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator &=(const vshort16 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vshort16& operator &=(int16 scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vshort16& operator <<=(int32 count)
	{
		MM = _mm256_slli_epi16(MM, count);
		return *this;
	}
	__forceinline vshort16& operator >>=(int32 count)
	{
		MM = _mm256_srai_epi16(MM, count);
		return *this;
	}
	__forceinline vshort16& operator ++()
	{
		MM = _mm256_add_epi16(MM, _mm256_set1_epi16(1));
		return *this;
	}
	__forceinline vshort16 operator ++(int)
	{
		_vshort16 value = MM;
		MM = _mm256_add_epi16(MM, _mm256_set1_epi16(1));
		return value;
	}
	__forceinline vshort16& operator --()
	{
		MM = _mm256_sub_epi16(MM, _mm256_set1_epi16(1));
		return *this;
	}
	__forceinline vshort16 operator --(int)
	{
		_vshort16 value = MM;
		MM = _mm256_sub_epi16(MM, _mm256_set1_epi16(1));
		return value;
	}
	__forceinline bool operator ==(const vshort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(int16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, _mm256_set1_epi16(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vshort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(int16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, _mm256_set1_epi16(scalar))) != 0xffffffff;
	}
	__forceinline bool operator <(const vshort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(other.MM, MM)) == 0xffffffff;
	}
	__forceinline bool operator <(int16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(_mm256_set1_epi16(scalar), MM)) == 0xffffffff;
	}
	__forceinline bool operator >(const vshort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator >(int16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(MM, _mm256_set1_epi16(scalar))) == 0xffffffff;
	}
	__forceinline bool operator <=(const vshort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(MM, other.MM)) == 0;
	}
	__forceinline bool operator <=(int16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(MM, _mm256_set1_epi16(scalar))) == 0;
	}
	__forceinline bool operator >=(const vshort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(other.MM, MM)) == 0;
	}
	__forceinline bool operator >=(int16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(_mm256_set1_epi16(scalar), MM)) == 0;
	}
	__forceinline int16& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline int16 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vshort16() const
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