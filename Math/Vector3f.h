#pragma once
#include "FastPix3D.h"

_MM_ALIGN16 struct Vector3f
{
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
		};
		__m128 MM;
	};

	property_get(float, Length)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(MM, MM, 0x71)));
	}
	property_get(float, InverseLength)
	{
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(MM, MM, 0x71)));
	}

	Vector3f() :
		MM(_mm_setzero_ps())
	{
	}
	Vector3f(__m128 mm) :
		MM(mm)
	{
	}
	explicit Vector3f(float uniform) :
		MM(_mm_set_ps(0, uniform, uniform, uniform))
	{
	}
	explicit Vector3f(float x, float y, float z) :
		MM(_mm_set_ps(0, z, y, x))
	{
	}

	Vector3f Normalize() const
	{
		return _mm_mul_ps(MM, _mm_rsqrt_ps(_mm_dp_ps(MM, MM, 0x7f)));
	}
	float DotProduct(const Vector3f &other) const
	{
		return _mm_cvtss_f32(_mm_dp_ps(MM, other.MM, 0x71));
	}
	Vector3f CrossProduct(const Vector3f &other) const
	{
		return _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(MM, MM, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(other.MM, other.MM, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(MM, MM, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(other.MM, other.MM, _MM_SHUFFLE(3, 0, 2, 1)))
		);
	}

	Vector3f operator +(const Vector3f &other) const
	{
		return _mm_add_ps(MM, other.MM);
	}
	Vector3f operator -(const Vector3f &other) const
	{
		return _mm_sub_ps(MM, other.MM);
	}
	Vector3f operator -() const
	{
		return _mm_sub_ps(_mm_setzero_ps(), MM);
	}
	Vector3f operator *(const Vector3f &other) const
	{
		return _mm_mul_ps(MM, other.MM);
	}
	Vector3f operator *(float scalar) const
	{
		return _mm_mul_ps(MM, _mm_set1_ps(scalar));
	}
	Vector3f operator /(const Vector3f &other) const
	{
		return _mm_div_ps(MM, other.MM);
	}
	Vector3f operator /(float scalar) const
	{
		return _mm_div_ps(MM, _mm_set1_ps(scalar));
	}
	Vector3f& operator +=(const Vector3f &other)
	{
		MM = _mm_add_ps(MM, other.MM);
		return *this;
	}
	Vector3f& operator -=(const Vector3f &other)
	{
		MM = _mm_sub_ps(MM, other.MM);
		return *this;
	}
	Vector3f& operator *=(const Vector3f &other)
	{
		MM = _mm_mul_ps(MM, other.MM);
		return *this;
	}
	Vector3f& operator *=(float scalar)
	{
		MM = _mm_mul_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	Vector3f& operator /=(const Vector3f &other)
	{
		MM = _mm_div_ps(MM, other.MM);
		return *this;
	}
	Vector3f& operator /=(float scalar)
	{
		MM = _mm_div_ps(MM, _mm_set1_ps(scalar));
		return *this;
	}
	bool operator ==(const Vector3f &other) const
	{
		return X == other.X && Y == other.Y && Z == other.Z;
	}
	bool operator !=(const Vector3f &other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z;
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