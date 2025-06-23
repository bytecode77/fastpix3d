#pragma once
#include "../FastPix3D.h"

__declspec(align(16)) struct FASTPIX3D_API vfloat4
{
private:
	union
	{
		float M[4];
		_vfloat4 MM;
	};

public:
	readonly_property(vfloat4, Reciprocal)
	{
		return _mm_rcp_ps(MM);
	}

	__forceinline vfloat4() :
		MM(_mm_setzero_ps())
	{
	}
	__forceinline vfloat4(const vfloat4 &value) :
		MM(value.MM)
	{
	}
	__forceinline vfloat4(_vfloat4 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vfloat4(const float *ptr) :
		MM(_mm_loadu_ps(ptr))
	{
	}
	__forceinline explicit vfloat4(const vfloat4 *ptr) :
		MM(_mm_load_ps((float*)ptr))
	{
	}
	__forceinline explicit vfloat4(const _vfloat4 *ptr) :
		MM(_mm_load_ps((float*)ptr))
	{
	}
	__forceinline explicit vfloat4(float uniform) :
		MM(_mm_set1_ps(uniform))
	{
	}
	__forceinline explicit vfloat4(float m0, float m1, float m2 = 0, float m3 = 0) :
		MM(_mm_setr_ps(m0, m1, m2, m3))
	{
	}

	__forceinline static vfloat4 Read(const float *src, _vuint4 offsets)
	{
		return _mm_i32gather_ps(src, offsets, 4);
	}
	__forceinline static vfloat4 Read(const float *src, _vuint4 offsets, _vuint4 mask)
	{
		return _mm_mask_i32gather_ps(vfloat4(), src, offsets, _mm_castsi128_ps(mask), 4);
	}
	__forceinline static void Write(vfloat4 *dest, const vfloat4 &src, _vuint4 mask)
	{
		_mm_maskstore_ps((float*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vfloat4 *dest, const vfloat4 &src)
	{
		_mm_store_ps((float*)dest, src.MM);
	}
	__forceinline static void Write(vfloat4 *dest, const vfloat4 &src)
	{
		_mm_store_ps((float*)dest, src.MM);
	}
	__forceinline static void Write(_vfloat4 *dest, const vfloat4 &src, _vuint4 mask)
	{
		_mm_maskstore_ps((float*)dest, mask, src.MM);
	}

	__forceinline vfloat4& operator =(const vfloat4 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vfloat4 operator +(const vfloat4 &other) const
	{
		return _mm_add_ps(MM, other.MM);
	}
	__forceinline vfloat4 operator +(float scalar) const
	{
		return _mm_add_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat4 operator -(const vfloat4 &other) const
	{
		return _mm_sub_ps(MM, other.MM);
	}
	__forceinline vfloat4 operator -(float scalar) const
	{
		return _mm_sub_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat4 operator -() const
	{
		return _mm_castsi128_ps(_mm_xor_epi32(_mm_castps_si128(MM), _mm_set1_epi32(0x80000000)));
	}
	__forceinline vfloat4 operator *(const vfloat4 &other) const
	{
		return _mm_mul_ps(MM, other.MM);
	}
	__forceinline vfloat4 operator *(float scalar) const
	{
		return _mm_mul_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat4 operator /(const vfloat4 &other) const
	{
		return _mm_div_ps(MM, other.MM);
	}
	__forceinline vfloat4 operator /(float scalar) const
	{
		return _mm_div_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat4& operator +=(const vfloat4 &other)
	{
		MM = _mm_add_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat4& operator +=(float scalar)
	{
		MM = _mm_add_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat4& operator -=(const vfloat4 &other)
	{
		MM = _mm_sub_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat4& operator -=(float scalar)
	{
		MM = _mm_sub_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat4& operator *=(const vfloat4 &other)
	{
		MM = _mm_mul_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat4& operator *=(float scalar)
	{
		MM = _mm_mul_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat4& operator /=(const vfloat4 &other)
	{
		MM = _mm_div_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat4& operator /=(float scalar)
	{
		MM = _mm_div_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline bool operator ==(const vfloat4 &other) const
	{
		return _mm_movemask_ps(_mm_cmpneq_ps(MM, other.MM)) == 0;
	}
	__forceinline bool operator ==(float scalar) const
	{
		return _mm_movemask_ps(_mm_cmpneq_ps(MM, _mm_set1_ps(scalar))) == 0;
	}
	__forceinline bool operator !=(const vfloat4 &other) const
	{
		return _mm_movemask_ps(_mm_cmpneq_ps(MM, other.MM)) != 0;
	}
	__forceinline bool operator !=(float scalar) const
	{
		return _mm_movemask_ps(_mm_cmpneq_ps(MM, _mm_set1_ps(scalar))) != 0;
	}
	__forceinline bool operator <(const vfloat4 &other) const
	{
		return _mm_movemask_ps(_mm_cmpge_ps(MM, other.MM)) == 0;
	}
	__forceinline bool operator <(float scalar) const
	{
		return _mm_movemask_ps(_mm_cmpge_ps(MM, _mm_set1_ps(scalar))) == 0;
	}
	__forceinline bool operator >(const vfloat4 &other) const
	{
		return _mm_movemask_ps(_mm_cmple_ps(MM, other.MM)) == 0;
	}
	__forceinline bool operator >(float scalar) const
	{
		return _mm_movemask_ps(_mm_cmple_ps(MM, _mm_set1_ps(scalar))) == 0;
	}
	__forceinline bool operator <=(const vfloat4 &other) const
	{
		return _mm_movemask_ps(_mm_cmpgt_ps(MM, other.MM)) == 0;
	}
	__forceinline bool operator <=(float scalar) const
	{
		return _mm_movemask_ps(_mm_cmpgt_ps(MM, _mm_set1_ps(scalar))) == 0;
	}
	__forceinline bool operator >=(const vfloat4 &other) const
	{
		return _mm_movemask_ps(_mm_cmplt_ps(MM, other.MM)) == 0;
	}
	__forceinline bool operator >=(float scalar) const
	{
		return _mm_movemask_ps(_mm_cmplt_ps(MM, _mm_set1_ps(scalar))) == 0;
	}
	__forceinline float& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline float operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vfloat4() const
	{
		return MM;
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