#pragma once
#include "../FastPix3D.h"

__declspec(align(32)) struct FASTPIX3D_API vfloat8
{
private:
	union
	{
		float M[8];
		_vfloat8 MM;
	};

public:
	readonly_property(vfloat8, Reciprocal)
	{
		return _mm256_rcp_ps(MM);
	}

	__forceinline vfloat8() :
		MM(_mm256_setzero_ps())
	{
	}
	__forceinline vfloat8(const vfloat8 &value) :
		MM(value.MM)
	{
	}
	__forceinline vfloat8(_vfloat8 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vfloat8(const float *ptr) :
		MM(_mm256_loadu_ps(ptr))
	{
	}
	__forceinline explicit vfloat8(const vfloat8 *ptr) :
		MM(_mm256_load_ps((float*)ptr))
	{
	}
	__forceinline explicit vfloat8(const _vfloat8 *ptr) :
		MM(_mm256_load_ps((float*)ptr))
	{
	}
	__forceinline explicit vfloat8(float uniform) :
		MM(_mm256_set1_ps(uniform))
	{
	}
	__forceinline explicit vfloat8(float m0, float m1, float m2 = 0, float m3 = 0, float m4 = 0, float m5 = 0, float m6 = 0, float m7 = 0) :
		MM(_mm256_setr_ps(m0, m1, m2, m3, m4, m5, m6, m7))
	{
	}

	__forceinline static vfloat8 Read(const float *src, _vuint8 offsets)
	{
		return _mm256_i32gather_ps(src, offsets, 4);
	}
	__forceinline static vfloat8 Read(const float *src, _vuint8 offsets, _vuint8 mask)
	{
		return _mm256_mask_i32gather_ps(vfloat8(), src, offsets, _mm256_castsi256_ps(mask), 4);
	}
	__forceinline static void Write(vfloat8 *dest, const vfloat8 &src)
	{
		_mm256_store_ps((float*)dest, src.MM);
	}
	__forceinline static void Write(_vfloat8 *dest, const vfloat8 &src)
	{
		_mm256_store_ps((float*)dest, src.MM);
	}
	__forceinline static void Write(vfloat8 *dest, const vfloat8 &src, _vuint8 mask)
	{
		_mm256_maskstore_ps((float*)dest, mask, src.MM);
	}
	__forceinline static void Write(_vfloat8 *dest, const vfloat8 &src, _vuint8 mask)
	{
		_mm256_maskstore_ps((float*)dest, mask, src.MM);
	}

	__forceinline vfloat8& operator =(const vfloat8 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vfloat8 operator +(const vfloat8 &other) const
	{
		return _mm256_add_ps(MM, other.MM);
	}
	__forceinline vfloat8 operator +(float scalar) const
	{
		return _mm256_add_ps(MM, _mm256_set1_ps(scalar));
	}
	__forceinline vfloat8 operator -(const vfloat8 &other) const
	{
		return _mm256_sub_ps(MM, other.MM);
	}
	__forceinline vfloat8 operator -(float scalar) const
	{
		return _mm256_sub_ps(MM, _mm256_set1_ps(scalar));
	}
	__forceinline vfloat8 operator -() const
	{
		return _mm256_castsi256_ps(_mm256_xor_epi32(_mm256_castps_si256(MM), _mm256_set1_epi32(0x80000000)));
	}
	__forceinline vfloat8 operator *(const vfloat8 &other) const
	{
		return _mm256_mul_ps(MM, other.MM);
	}
	__forceinline vfloat8 operator *(float scalar) const
	{
		return _mm256_mul_ps(MM, _mm256_set1_ps(scalar));
	}
	__forceinline vfloat8 operator /(const vfloat8 &other) const
	{
		return _mm256_div_ps(MM, other.MM);
	}
	__forceinline vfloat8 operator /(float scalar) const
	{
		return _mm256_div_ps(MM, _mm256_set1_ps(scalar));
	}
	__forceinline vfloat8& operator +=(const vfloat8 &other)
	{
		MM = _mm256_add_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat8& operator +=(float scalar)
	{
		MM = _mm256_add_ps(MM, _mm256_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat8& operator -=(const vfloat8 &other)
	{
		MM = _mm256_sub_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat8& operator -=(float scalar)
	{
		MM = _mm256_sub_ps(MM, _mm256_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat8& operator *=(const vfloat8 &other)
	{
		MM = _mm256_mul_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat8& operator *=(float scalar)
	{
		MM = _mm256_mul_ps(MM, _mm256_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat8& operator /=(const vfloat8 &other)
	{
		MM = _mm256_div_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat8& operator /=(float scalar)
	{
		MM = _mm256_div_ps(MM, _mm256_set1_ps(scalar));
		return *this;
	}
	__forceinline bool operator ==(const vfloat8 &other) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, other.MM, _CMP_NEQ_OQ)) == 0;
	}
	__forceinline bool operator ==(float scalar) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, _mm256_set1_ps(scalar), _CMP_NEQ_OQ)) == 0;
	}
	__forceinline bool operator !=(const vfloat8 &other) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, other.MM, _CMP_NEQ_OQ)) != 0;
	}
	__forceinline bool operator !=(float scalar) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, _mm256_set1_ps(scalar), _CMP_NEQ_OQ)) != 0;
	}
	__forceinline bool operator <(const vfloat8 &other) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, other.MM, _CMP_GE_OQ)) == 0;
	}
	__forceinline bool operator <(float scalar) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, _mm256_set1_ps(scalar), _CMP_GE_OQ)) == 0;
	}
	__forceinline bool operator >(const vfloat8 &other) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, other.MM, _CMP_LE_OQ)) == 0;
	}
	__forceinline bool operator >(float scalar) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, _mm256_set1_ps(scalar), _CMP_LE_OQ)) == 0;
	}
	__forceinline bool operator <=(const vfloat8 &other) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, other.MM, _CMP_GT_OQ)) == 0;
	}
	__forceinline bool operator <=(float scalar) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, _mm256_set1_ps(scalar), _CMP_GT_OQ)) == 0;
	}
	__forceinline bool operator >=(const vfloat8 &other) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, other.MM, _CMP_LT_OQ)) == 0;
	}
	__forceinline bool operator >=(float scalar) const
	{
		return _mm256_movemask_ps(_mm256_cmp_ps(MM, _mm256_set1_ps(scalar), _CMP_LT_OQ)) == 0;
	}
	__forceinline float& operator [](int32 index)
	{
		return M[index];
	}
	__forceinline float operator [](int32 index) const
	{
		return M[index];
	}
	__forceinline operator _vfloat8() const
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