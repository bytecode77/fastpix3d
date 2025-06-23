#pragma once
#include "../FastPix3D.h"
#include "vushort16.h"

__declspec(align(32)) struct FASTPIX3D_API vuint8
{
private:
	union
	{
		uint32 M[8];
		_vuint8 MM;
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
	readonly_property(vuint8, Low16)
	{
		return _mm256_unpacklo_epi16(MM, _mm256_setzero_si256());
	}
	readonly_property(vuint8, High16)
	{
		return _mm256_unpackhi_epi16(MM, _mm256_setzero_si256());
	}

	__forceinline vuint8() :
		MM(_mm256_setzero_si256())
	{
	}
	__forceinline vuint8(const vuint8 &value) :
		MM(value.MM)
	{
	}
	__forceinline vuint8(_vuint8 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vuint8(const uint32 *ptr) :
		MM(_mm256_loadu_si256((_vuint8*)ptr))
	{
	}
	__forceinline explicit vuint8(const vuint8 *ptr) :
		MM(_mm256_load_si256((_vuint8*)ptr))
	{
	}
	__forceinline explicit vuint8(const _vuint8 *ptr) :
		MM(_mm256_load_si256(ptr))
	{
	}
	__forceinline explicit vuint8(uint32 uniform) :
		MM(_mm256_set1_epi32(uniform))
	{
	}
	__forceinline explicit vuint8(uint32 m0, uint32 m1, uint32 m2 = 0, uint32 m3 = 0, uint32 m4 = 0, uint32 m5 = 0, uint32 m6 = 0, uint32 m7 = 0) :
		MM(_mm256_setr_epi32(m0, m1, m2, m3, m4, m5, m6, m7))
	{
	}

	__forceinline static vuint8 Read(const uint32 *src, _vuint8 offsets)
	{
		return _mm256_i32gather_epi32((int32*)src, offsets, 4);
	}
	__forceinline static vuint8 Read(const uint32 *src, _vuint8 offsets, _vuint8 mask)
	{
		return _mm256_mask_i32gather_epi32(vuint8(), (int32*)src, offsets, mask, 4);
	}
	__forceinline static void Write(vuint8 *dest, const vuint8 &src)
	{
		_mm256_store_si256((_vuint8*)dest, src.MM);
	}
	__forceinline static void Write(_vuint8 *dest, const vuint8 &src)
	{
		_mm256_store_si256((_vuint8*)dest, src.MM);
	}
	__forceinline static void Write(vuint8 *dest, const vuint8 &src, _vuint8 mask)
	{
		_mm256_maskstore_epi32((int32*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vuint8 *dest, const vuint8 &src, _vuint8 mask)
	{
		_mm256_maskstore_epi32((int32*)dest, mask, src.MM);
	}

	__forceinline vuint8& operator =(const vuint8 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vuint8 operator +(const vuint8 &other) const
	{
		return _mm256_add_epi32(MM, other.MM);
	}
	__forceinline vuint8 operator +(uint32 scalar) const
	{
		return _mm256_add_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator -(const vuint8 &other) const
	{
		return _mm256_sub_epi32(MM, other.MM);
	}
	__forceinline vuint8 operator -(uint32 scalar) const
	{
		return _mm256_sub_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator *(const vuint8 &other) const
	{
		return _mm256_mullo_epi32(MM, other.MM);
	}
	__forceinline vuint8 operator *(uint32 scalar) const
	{
		return _mm256_mullo_epi32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator /(const vuint8 &other) const
	{
		return _mm256_div_epu32(MM, other.MM);
	}
	__forceinline vuint8 operator /(uint32 scalar) const
	{
		return _mm256_div_epu32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator %(const vuint8 &other) const
	{
		return _mm256_rem_epu32(MM, other.MM);
	}
	__forceinline vuint8 operator %(uint32 scalar) const
	{
		return _mm256_rem_epu32(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator ^(const vuint8 &other) const
	{
		return _mm256_xor_si256(MM, other.MM);
	}
	__forceinline vuint8 operator ^(uint32 scalar) const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator |(const vuint8 &other) const
	{
		return _mm256_or_si256(MM, other.MM);
	}
	__forceinline vuint8 operator |(uint32 scalar) const
	{
		return _mm256_or_si256(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator &(const vuint8 &other) const
	{
		return _mm256_and_si256(MM, other.MM);
	}
	__forceinline vuint8 operator &(uint32 scalar) const
	{
		return _mm256_and_si256(MM, _mm256_set1_epi32(scalar));
	}
	__forceinline vuint8 operator ~() const
	{
		return _mm256_xor_si256(MM, _mm256_set1_epi32(-1));
	}
	__forceinline vuint8 operator <<(const vuint8 &other) const
	{
		return _mm256_sllv_epi32(MM, other.MM);
	}
	__forceinline vuint8 operator <<(uint32 count) const
	{
		return _mm256_slli_epi32(MM, count);
	}
	__forceinline vuint8 operator >>(const vuint8 &other) const
	{
		return _mm256_srlv_epi32(MM, other.MM);
	}
	__forceinline vuint8 operator >>(uint32 count) const
	{
		return _mm256_srli_epi32(MM, count);
	}
	__forceinline vuint8& operator +=(const vuint8 &other)
	{
		MM = _mm256_add_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator +=(uint32 scalar)
	{
		MM = _mm256_add_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator -=(const vuint8 &other)
	{
		MM = _mm256_sub_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator -=(uint32 scalar)
	{
		MM = _mm256_sub_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator *=(const vuint8 &other)
	{
		MM = _mm256_mullo_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator *=(uint32 scalar)
	{
		MM = _mm256_mullo_epi32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator /=(const vuint8 &other)
	{
		MM = _mm256_div_epu32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator /=(uint32 scalar)
	{
		MM = _mm256_div_epu32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator %=(const vuint8 &other)
	{
		MM = _mm256_rem_epu32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator %=(uint32 scalar)
	{
		MM = _mm256_rem_epu32(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator ^=(const vuint8 &other)
	{
		MM = _mm256_xor_si256(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator ^=(uint32 scalar)
	{
		MM = _mm256_xor_si256(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator |=(const vuint8 &other)
	{
		MM = _mm256_or_si256(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator |=(uint32 scalar)
	{
		MM = _mm256_or_si256(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator &=(const vuint8 &other)
	{
		MM = _mm256_and_si256(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator &=(uint32 scalar)
	{
		MM = _mm256_and_si256(MM, _mm256_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint8& operator <<=(const vuint8 &other)
	{
		MM = _mm256_sllv_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator <<=(uint32 count)
	{
		MM = _mm256_slli_epi32(MM, count);
		return *this;
	}
	__forceinline vuint8& operator >>=(const vuint8 &other)
	{
		MM = _mm256_srlv_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint8& operator >>=(uint32 count)
	{
		MM = _mm256_srli_epi32(MM, count);
		return *this;
	}
	__forceinline vuint8& operator ++()
	{
		MM = _mm256_add_epi32(MM, _mm256_set1_epi32(1));
		return *this;
	}
	__forceinline vuint8 operator ++(int)
	{
		_vuint8 value = MM;
		MM = _mm256_add_epi32(MM, _mm256_set1_epi32(1));
		return value;
	}
	__forceinline vuint8& operator --()
	{
		MM = _mm256_sub_epi32(MM, _mm256_set1_epi32(1));
		return *this;
	}
	__forceinline vuint8 operator --(int)
	{
		_vuint8 value = MM;
		MM = _mm256_sub_epi32(MM, _mm256_set1_epi32(1));
		return value;
	}
	__forceinline bool operator ==(const vuint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, other.MM)) == 0xffffffff;
	}
	__forceinline bool operator ==(uint32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, _mm256_set1_epi32(scalar))) == 0xffffffff;
	}
	__forceinline bool operator !=(const vuint8 &other) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, other.MM)) != 0xffffffff;
	}
	__forceinline bool operator !=(uint32 scalar) const
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(MM, _mm256_set1_epi32(scalar))) != 0xffffffff;
	}
	__forceinline uint32& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline uint32 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vuint8() const
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