#pragma once
#include "../FastPix3D.h"

__declspec(align(16)) struct FASTPIX3D_API vuint4
{
private:
	union
	{
		uint32 M[4];
		_vuint4 MM;
	};

public:
	__forceinline vuint4() :
		MM(_mm_setzero_si128())
	{
	}
	__forceinline vuint4(const vuint4 &other) :
		MM(other.MM)
	{
	}
	__forceinline vuint4(_vuint4 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vuint4(const uint32 *ptr) :
		MM(_mm_loadu_si128((const _vuint4*)ptr))
	{
	}
	__forceinline explicit vuint4(const vuint4 *ptr) :
		MM(_mm_load_si128((_vuint4*)ptr))
	{
	}
	__forceinline explicit vuint4(const _vuint4 *ptr) :
		MM(_mm_load_si128(ptr))
	{
	}
	__forceinline explicit vuint4(uint32 uniform) :
		MM(_mm_set1_epi32(uniform))
	{
	}
	__forceinline explicit vuint4(uint32 m0, uint32 m1, uint32 m2 = 0, uint32 m3 = 0) :
		MM(_mm_setr_epi32(m0, m1, m2, m3))
	{
	}

	__forceinline static vuint4 Read(const uint32 *src, _vuint4 offsets)
	{
		return _mm_i32gather_epi32((const int32*)src, offsets, 4);
	}
	__forceinline static vuint4 Read(const uint32 *src, _vuint4 offsets, _vuint4 mask)
	{
		return _mm_mask_i32gather_epi32(vuint4(), (const int32*)src, offsets, mask, 4);
	}
	__forceinline static void Write(uint32 *dest, const vuint4 &src)
	{
		_mm_storeu_si128((_vuint4*)dest, src.MM);
	}
	__forceinline static void Write(vuint4 *dest, const vuint4 &src)
	{
		_mm_store_si128((_vuint4*)dest, src.MM);
	}
	__forceinline static void Write(_vuint4 *dest, const vuint4 &src)
	{
		_mm_store_si128((_vuint4*)dest, src.MM);
	}
	__forceinline static void Write(uint32 *dest, const vuint4 &src, _vuint4 mask)
	{
		_mm_maskstore_epi32((int32*)dest, mask, src.MM);
	}
	__forceinline static void Write(vuint4 *dest, const vuint4 &src, _vuint4 mask)
	{
		_mm_maskstore_epi32((int32*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vuint4 *dest, const vuint4 &src, _vuint4 mask)
	{
		_mm_maskstore_epi32((int32*)dest, mask, src.MM);
	}

	__forceinline vuint4& operator =(const vuint4 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vuint4 operator +(const vuint4 &other) const
	{
		return _mm_add_epi32(MM, other.MM);
	}
	__forceinline vuint4 operator +(uint32 scalar) const
	{
		return _mm_add_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator -(const vuint4 &other) const
	{
		return _mm_sub_epi32(MM, other.MM);
	}
	__forceinline vuint4 operator -(uint32 scalar) const
	{
		return _mm_sub_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator *(const vuint4 &other) const
	{
		return _mm_mullo_epi32(MM, other.MM);
	}
	__forceinline vuint4 operator *(uint32 scalar) const
	{
		return _mm_mullo_epi32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator /(const vuint4 &other) const
	{
		return _mm_div_epu32(MM, other.MM);
	}
	__forceinline vuint4 operator /(uint32 scalar) const
	{
		return _mm_div_epu32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator %(const vuint4 &other) const
	{
		return _mm_rem_epu32(MM, other.MM);
	}
	__forceinline vuint4 operator %(uint32 scalar) const
	{
		return _mm_rem_epu32(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator ^(const vuint4 &other) const
	{
		return _mm_xor_si128(MM, other.MM);
	}
	__forceinline vuint4 operator ^(uint32 scalar) const
	{
		return _mm_xor_si128(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator |(const vuint4 &other) const
	{
		return _mm_or_si128(MM, other.MM);
	}
	__forceinline vuint4 operator |(uint32 scalar) const
	{
		return _mm_or_si128(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator &(const vuint4 &other) const
	{
		return _mm_and_si128(MM, other.MM);
	}
	__forceinline vuint4 operator &(uint32 scalar) const
	{
		return _mm_and_si128(MM, _mm_set1_epi32(scalar));
	}
	__forceinline vuint4 operator ~() const
	{
		return _mm_xor_si128(MM, _mm_set1_epi32(-1));
	}
	__forceinline vuint4 operator <<(const vuint4 &other) const
	{
		return _mm_sllv_epi32(MM, other.MM);
	}
	__forceinline vuint4 operator <<(int32 count) const
	{
		return _mm_slli_epi32(MM, count);
	}
	__forceinline vuint4 operator >>(const vuint4 &other) const
	{
		return _mm_srlv_epi32(MM, other.MM);
	}
	__forceinline vuint4 operator >>(int32 count) const
	{
		return _mm_srli_epi32(MM, count);
	}
	__forceinline vuint4& operator +=(const vuint4 &other)
	{
		MM = _mm_add_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator +=(uint32 scalar)
	{
		MM = _mm_add_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator -=(const vuint4 &other)
	{
		MM = _mm_sub_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator -=(uint32 scalar)
	{
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator *=(const vuint4 &other)
	{
		MM = _mm_mullo_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator *=(uint32 scalar)
	{
		MM = _mm_mullo_epi32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator /=(const vuint4 &other)
	{
		MM = _mm_div_epu32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator /=(uint32 scalar)
	{
		MM = _mm_div_epu32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator %=(const vuint4 &other)
	{
		MM = _mm_rem_epu32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator %=(uint32 scalar)
	{
		MM = _mm_rem_epu32(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator ^=(const vuint4 &other)
	{
		MM = _mm_xor_si128(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator ^=(uint32 scalar)
	{
		MM = _mm_xor_si128(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator |=(const vuint4 &other)
	{
		MM = _mm_or_si128(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator |=(uint32 scalar)
	{
		MM = _mm_or_si128(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator &=(const vuint4 &other)
	{
		MM = _mm_and_si128(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator &=(uint32 scalar)
	{
		MM = _mm_and_si128(MM, _mm_set1_epi32(scalar));
		return *this;
	}
	__forceinline vuint4& operator <<=(const vuint4 &other)
	{
		MM = _mm_sllv_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator <<=(int32 count)
	{
		MM = _mm_slli_epi32(MM, count);
		return *this;
	}
	__forceinline vuint4& operator >>=(const vuint4 &other)
	{
		MM = _mm_srlv_epi32(MM, other.MM);
		return *this;
	}
	__forceinline vuint4& operator >>=(int32 count)
	{
		MM = _mm_srli_epi32(MM, count);
		return *this;
	}
	__forceinline vuint4& operator ++()
	{
		MM = _mm_add_epi32(MM, _mm_set1_epi32(1));
		return *this;
	}
	__forceinline vuint4 operator ++(int)
	{
		_vuint4 value = MM;
		MM = _mm_add_epi32(MM, _mm_set1_epi32(1));
		return value;
	}
	__forceinline vuint4& operator --()
	{
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(1));
		return *this;
	}
	__forceinline vuint4 operator --(int)
	{
		_vuint4 value = MM;
		MM = _mm_sub_epi32(MM, _mm_set1_epi32(1));
		return value;
	}
	__forceinline bool operator ==(const vuint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, other.MM)) == 0xffff;
	}
	__forceinline bool operator ==(uint32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, _mm_set1_epi32(scalar))) == 0xffff;
	}
	__forceinline bool operator !=(const vuint4 &other) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, other.MM)) != 0xffff;
	}
	__forceinline bool operator !=(uint32 scalar) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(MM, _mm_set1_epi32(scalar))) != 0xffff;
	}
	__forceinline uint32& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline uint32 operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vuint4() const
	{
		return MM;
	}
	void* operator new[](size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete[](void *ptr)
	{
		_aligned_free(ptr);
	}
};