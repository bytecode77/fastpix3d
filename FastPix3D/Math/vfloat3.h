#pragma once
#include "../FastPix3D.h"

__declspec(align(16)) struct FASTPIX3D_API vfloat3
{
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
		};
		_vfloat3 MM;
	};

	readonly_property(vfloat3, Reciprocal)
	{
		return _mm_rcp_ps(MM);
	}
	readonly_property(float, Length)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(MM, MM, 0x71)));
	}
	readonly_property(float, SquaredLength)
	{
		return _mm_cvtss_f32(_mm_dp_ps(MM, MM, 0x71));
	}
	readonly_property(float, InverseLength)
	{
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(MM, MM, 0x71)));
	}
	readonly_property(float, InverseSquaredLength)
	{
		float inverseLength = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(MM, MM, 0x71)));
		return inverseLength * inverseLength;
	}

	__forceinline vfloat3() :
		MM(_mm_setzero_ps())
	{
	}
	__forceinline vfloat3(const vfloat3 &value) :
		MM(value.MM)
	{
	}
	__forceinline vfloat3(_vfloat3 mm) :
		MM(mm)
	{
	}
	__forceinline explicit vfloat3(const float *ptr) :
		MM(_mm_loadu_ps(ptr))
	{
	}
	__forceinline explicit vfloat3(const vfloat3 *ptr) :
		MM(_mm_load_ps((float*)ptr))
	{
	}
	__forceinline explicit vfloat3(const _vfloat3 *ptr) :
		MM(_mm_load_ps((float*)ptr))
	{
	}
	__forceinline explicit vfloat3(float uniform) :
		MM(_mm_setr_ps(uniform, uniform, uniform, 0))
	{
	}
	__forceinline explicit vfloat3(float x, float y, float z) :
		MM(_mm_setr_ps(x, y, z, 0))
	{
	}

	__forceinline vfloat3 Normalize() const
	{
		return _mm_mul_ps(MM, _mm_rsqrt_ps(_mm_dp_ps(MM, MM, 0x77)));
	}
	__forceinline float DotProduct(const vfloat3 &other) const
	{
		return _mm_cvtss_f32(_mm_dp_ps(MM, other.MM, 0x71));
	}
	__forceinline vfloat3 CrossProduct(const vfloat3 &other) const
	{
		return _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(MM, MM, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(other.MM, other.MM, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(MM, MM, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(other.MM, other.MM, _MM_SHUFFLE(3, 0, 2, 1)))
		);
	}

	__forceinline vfloat3& operator =(const vfloat3 &other)
	{
		MM = other.MM;
		return *this;
	}
	__forceinline vfloat3 operator +(const vfloat3 &other) const
	{
		return _mm_add_ps(MM, other.MM);
	}
	__forceinline vfloat3 operator +(float scalar) const
	{
		return _mm_add_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat3 operator -(const vfloat3 &other) const
	{
		return _mm_sub_ps(MM, other.MM);
	}
	__forceinline vfloat3 operator -(float scalar) const
	{
		return _mm_sub_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat3 operator -() const
	{
		return _mm_castsi128_ps(_mm_xor_epi32(_mm_castps_si128(MM), _mm_set1_epi32(0x80000000)));
	}
	__forceinline vfloat3 operator *(const vfloat3 &other) const
	{
		return _mm_mul_ps(MM, other.MM);
	}
	__forceinline vfloat3 operator *(float scalar) const
	{
		return _mm_mul_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat3 operator /(const vfloat3 &other) const
	{
		return _mm_div_ps(MM, other.MM);
	}
	__forceinline vfloat3 operator /(float scalar) const
	{
		return _mm_div_ps(MM, _mm_set1_ps(scalar));
	}
	__forceinline vfloat3& operator +=(const vfloat3 &other)
	{
		MM = _mm_add_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat3& operator +=(float scalar)
	{
		MM = _mm_add_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat3& operator -=(const vfloat3 &other)
	{
		MM = _mm_sub_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat3& operator -=(float scalar)
	{
		MM = _mm_sub_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat3& operator *=(const vfloat3 &other)
	{
		MM = _mm_mul_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat3& operator *=(float scalar)
	{
		MM = _mm_mul_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline vfloat3& operator /=(const vfloat3 &other)
	{
		MM = _mm_div_ps(MM, other.MM);
		return *this;
	}
	__forceinline vfloat3& operator /=(float scalar)
	{
		MM = _mm_div_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	__forceinline bool operator ==(const vfloat3 &other) const
	{
		return (_mm_movemask_ps(_mm_cmpeq_ps(MM, other.MM)) & 7) == 7;
	}
	__forceinline bool operator ==(float scalar) const
	{
		return (_mm_movemask_ps(_mm_cmpeq_ps(MM, _mm_set1_ps(scalar))) & 7) == 7;
	}
	__forceinline bool operator !=(const vfloat3 &other) const
	{
		return (_mm_movemask_ps(_mm_cmpeq_ps(MM, other.MM)) & 7) != 7;
	}
	__forceinline bool operator !=(float scalar) const
	{
		return (_mm_movemask_ps(_mm_cmpeq_ps(MM, _mm_set1_ps(scalar))) & 7) != 7;
	}
	__forceinline bool operator <(const vfloat3 &other) const
	{
		return (_mm_movemask_ps(_mm_cmplt_ps(MM, other.MM)) & 7) == 7;
	}
	__forceinline bool operator <(float scalar) const
	{
		return (_mm_movemask_ps(_mm_cmplt_ps(MM, _mm_set1_ps(scalar))) & 7) == 7;
	}
	__forceinline bool operator >(const vfloat3 &other) const
	{
		return (_mm_movemask_ps(_mm_cmpgt_ps(MM, other.MM)) & 7) == 7;
	}
	__forceinline bool operator >(float scalar) const
	{
		return (_mm_movemask_ps(_mm_cmpgt_ps(MM, _mm_set1_ps(scalar))) & 7) == 7;
	}
	__forceinline bool operator <=(const vfloat3 &other) const
	{
		return (_mm_movemask_ps(_mm_cmple_ps(MM, other.MM)) & 7) == 7;
	}
	__forceinline bool operator <=(float scalar) const
	{
		return (_mm_movemask_ps(_mm_cmple_ps(MM, _mm_set1_ps(scalar))) & 7) == 7;
	}
	__forceinline bool operator >=(const vfloat3 &other) const
	{
		return (_mm_movemask_ps(_mm_cmpge_ps(MM, other.MM)) & 7) == 7;
	}
	__forceinline bool operator >=(float scalar) const
	{
		return (_mm_movemask_ps(_mm_cmpge_ps(MM, _mm_set1_ps(scalar))) & 7) == 7;
	}
	__forceinline operator _vfloat3() const
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