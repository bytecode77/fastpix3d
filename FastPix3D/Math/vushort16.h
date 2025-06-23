#pragma once
#include "../FastPix3D.h"

__declspec(align(32)) struct FASTPIX3D_API vushort16
{
private:
	union
	{
		uint16 M[16];
		_vushort16 MM;
	};

public:
	__forceinline vushort16() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vushort16(const vushort16 &value) :
		MM(value.MM)
	{
	}
	__forceinline vushort16(_vushort16 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vushort16(const uint16 *ptr) :
		MM(_mm256_loadu_si256((_vushort16*)ptr))
	{
	}
	__forceinline explicit vushort16(const vushort16 *ptr) :
		MM(_mm256_load_si256((_vushort16*)ptr))
	{
	}
	__forceinline explicit vushort16(const _vushort16 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vushort16(uint16 uniform) :
		MM(_mm256_set1_epi16(uniform))
	{
	}
	__forceinline explicit vushort16(uint16 m0, uint16 m1, uint16 m2 = 0, uint16 m3 = 0, uint16 m4 = 0, uint16 m5 = 0, uint16 m6 = 0, uint16 m7 = 0, uint16 m8 = 0, uint16 m9 = 0, uint16 m10 = 0, uint16 m11 = 0, uint16 m12 = 0, uint16 m13 = 0, uint16 m14 = 0, uint16 m15 = 0) :
		MM(_mm256_setr_epi16(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15))
	{
	}

	__forceinline static void Write(vushort16 *dest, const vushort16 &src)
	{
		_mm256_store_si256((_vushort16*)dest, src.MM);
	}
	__forceinline static void Write(_vushort16 *dest, const vushort16 &src)
	{
		_mm256_store_si256((_vushort16*)dest, src.MM);
	}

	__forceinline vushort16& operator =(const vushort16 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vushort16 operator +(const vushort16 &other) const
	{
		return _mm256_add_epi16(MM, other.MM);
	}
	__forceinline vushort16 operator +(uint16 scalar) const
	{
		return _mm256_add_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator -(const vushort16 &other) const
	{
		return _mm256_sub_epi16(MM, other.MM);
	}
	__forceinline vushort16 operator -(uint16 scalar) const
	{
		return _mm256_sub_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator *(const vushort16 &other) const
	{
		return _mm256_mullo_epi16(MM, other.MM);
	}
	__forceinline vushort16 operator *(uint16 scalar) const
	{
		return _mm256_mullo_epi16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator /(const vushort16 &other) const
	{
		return _mm256_div_epu16(MM, other.MM);
	}
	__forceinline vushort16 operator /(uint16 scalar) const
	{
		return _mm256_div_epu16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator %(const vushort16 &other) const
	{
		return _mm256_rem_epu16(MM, other.MM);
	}
	__forceinline vushort16 operator %(uint16 scalar) const
	{
		return _mm256_rem_epu16(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator ^(const vushort16 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vushort16 operator ^(uint16 scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator |(const vushort16 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vushort16 operator |(uint16 scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator &(const vushort16 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vushort16 operator &(uint16 scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi16(scalar));
	}
	__forceinline vushort16 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi16(-1));
	}
	__forceinline vushort16 operator <<(int32 count) const
	{
		return _mm256_slli_epi16(MM, count);
	}
	__forceinline vushort16 operator >>(int32 count) const
	{
		return _mm256_srli_epi16(MM, count);
	}
	__forceinline vushort16& operator +=(const vushort16 &other)
	{
		MM = _mm256_add_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator +=(uint16 scalar)
	{
		MM = _mm256_add_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator -=(const vushort16 &other)
	{
		MM = _mm256_sub_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator -=(uint16 scalar)
	{
		MM = _mm256_sub_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator *=(const vushort16 &other)
	{
		MM = _mm256_mullo_epi16(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator *=(uint16 scalar)
	{
		MM = _mm256_mullo_epi16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator /=(const vushort16 &other)
	{
		MM = _mm256_div_epu16(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator /=(uint16 scalar)
	{
		MM = _mm256_div_epu16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator %=(const vushort16 &other)
	{
		MM = _mm256_rem_epu16(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator %=(uint16 scalar)
	{
		MM = _mm256_rem_epu16(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator ^=(const vushort16 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator ^=(uint16 scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator |=(const vushort16 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator |=(uint16 scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator &=(const vushort16 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vushort16& operator &=(uint16 scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi16(scalar));
		return *this;
	}
	__forceinline vushort16& operator <<=(int32 count)
	{
		MM = _mm256_slli_epi16(MM, count);
		return *this;
	}
	__forceinline vushort16& operator >>=(int32 count)
	{
		MM = _mm256_srli_epi16(MM, count);
		return *this;
	}
	__forceinline vushort16& operator ++()
	{
		MM = _mm256_add_epi16(MM, _mm256_set1_epi16(1));
		return *this;
	}
	__forceinline vushort16 operator ++(int)
	{
		_vushort16 value = MM;
		MM = _mm256_add_epi16(MM, _mm256_set1_epi16(1));
		return value;
	}
	__forceinline vushort16& operator --()
	{
		MM = _mm256_sub_epi16(MM, _mm256_set1_epi16(1));
		return *this;
	}
	__forceinline vushort16 operator --(int)
	{
		_vushort16 value = MM;
		MM = _mm256_sub_epi16(MM, _mm256_set1_epi16(1));
		return value;
	}
	__forceinline bool operator ==(const vushort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(uint16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, _mm256_set1_epi16(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vushort16 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(uint16 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(MM, _mm256_set1_epi16(scalar))) != 0xffffffff;
	}
	__forceinline uint16& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline uint16 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vushort16() const
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