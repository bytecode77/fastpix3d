#pragma once
#include "../FastPix3D.h"
#include "Math_.h"
#include "VectorMath.h"

__declspec(align(16)) class FASTPIX3D_API Matrix4f
{
private:
	vfloat4 M[4];
	vfloat4 R[4]; // Keep track of rotation-only part.

	explicit Matrix4f(
		const vfloat4 &m1, const vfloat4 &m2, const vfloat4 &m3, const vfloat4 &m4,
		const vfloat4 &r1, const vfloat4 &r2, const vfloat4 &r3, const vfloat4 &r4) :
		M { m1, m2, m3, m4 },
		R { r1, r2, r3, r4 }
	{
	}
	explicit Matrix4f(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44,
		float r11, float r12, float r13, float r14,
		float r21, float r22, float r23, float r24,
		float r31, float r32, float r33, float r34,
		float r41, float r42, float r43, float r44) :
		M { vfloat4(m11, m12, m13, m14), vfloat4(m21, m22, m23, m24), vfloat4(m31, m32, m33, m34), vfloat4(m41, m42, m43, m44) },
		R { vfloat4(r11, r12, r13, r14), vfloat4(r21, r22, r23, r24), vfloat4(r31, r32, r33, r34), vfloat4(r41, r42, r43, r44) }
	{
	}

public:
	readonly_property(Matrix4f, RotationPart)
	{
		return Matrix4f(
			R[0], R[1], R[2], R[3],
			R[0], R[1], R[2], R[3]
		);
	}

	Matrix4f()
	{
	}
	Matrix4f(const Matrix4f &value) :
		M { value.M[0], value.M[1], value.M[2], value.M[3] },
		R { value.R[0], value.R[1], value.R[2], value.R[3] }
	{
	}
	explicit Matrix4f(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44) :
		M { vfloat4(m11, m12, m13, m14), vfloat4(m21, m22, m23, m24), vfloat4(m31, m32, m33, m34), vfloat4(m41, m42, m43, m44) },
		R { vfloat4(1, 0, 0, 0), vfloat4(0, 1, 0, 0), vfloat4(0, 0, 1, 0), vfloat4(0, 0, 0, 1) }
	{
	}
	explicit Matrix4f(const float *m) :
		M { vfloat4(m), vfloat4(&m[4]), vfloat4(&m[8]), vfloat4(&m[12]) },
		R { vfloat4(1, 0, 0, 0), vfloat4(0, 1, 0, 0), vfloat4(0, 0, 1, 0), vfloat4(0, 0, 0, 1) }
	{
	}

	static Matrix4f Identity()
	{
		return Matrix4f(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f Scale(float x, float y, float z)
	{
		return Matrix4f(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f Scale(const vfloat3 &vector)
	{
		return Scale(vector.X, vector.Y, vector.Z);
	}
	static Matrix4f Scale(float uniform)
	{
		return Scale(uniform, uniform, uniform);
	}
	static Matrix4f RotateX(float angle)
	{
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		return Matrix4f(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1,

			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f RotateY(float angle)
	{
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		return Matrix4f(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1,

			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f RotateZ(float angle)
	{
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		return Matrix4f(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,

			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f Translate(float x, float y, float z)
	{
		return Matrix4f(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		);
	}
	static Matrix4f Translate(const vfloat3 &vector)
	{
		return Translate(vector.X, vector.Y, vector.Z);
	}

	Matrix4f Transpose() const
	{
		_vfloat4 tm0 = _mm_unpacklo_ps(M[0], M[1]);
		_vfloat4 tm1 = _mm_unpackhi_ps(M[0], M[1]);
		_vfloat4 tm2 = _mm_unpacklo_ps(M[2], M[3]);
		_vfloat4 tm3 = _mm_unpackhi_ps(M[2], M[3]);

		_vfloat4 tr0 = _mm_unpacklo_ps(R[0], R[1]);
		_vfloat4 tr1 = _mm_unpackhi_ps(R[0], R[1]);
		_vfloat4 tr2 = _mm_unpacklo_ps(R[2], R[3]);
		_vfloat4 tr3 = _mm_unpackhi_ps(R[2], R[3]);

		return Matrix4f(
			_mm_movelh_ps(tm0, tm2),
			_mm_movehl_ps(tm2, tm0),
			_mm_movelh_ps(tm1, tm3),
			_mm_movehl_ps(tm3, tm1),

			_mm_movelh_ps(tr0, tr2),
			_mm_movehl_ps(tr2, tr0),
			_mm_movelh_ps(tr1, tr3),
			_mm_movehl_ps(tr3, tr1)
		);
	}
	Matrix4f ToNormalMatrix() const
	{
		float det =
			M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
			M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) +
			M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]);

		if (Math::Abs(det) < 1e-6f) return Matrix4f::Identity();

		return Matrix4f(
			M[1][1] * M[2][2] - M[1][2] * M[2][1], M[1][2] * M[2][0] - M[1][0] * M[2][2], M[1][0] * M[2][1] - M[1][1] * M[2][0], 0,
			M[0][2] * M[2][1] - M[0][1] * M[2][2], M[0][0] * M[2][2] - M[0][2] * M[2][0], M[0][1] * M[2][0] - M[0][0] * M[2][1], 0,
			M[0][1] * M[1][2] - M[0][2] * M[1][1], M[0][2] * M[1][0] - M[0][0] * M[1][2], M[0][0] * M[1][1] - M[0][1] * M[1][0], 0,
			0, 0, 0, 0
		) * (1 / det);
	}

	Matrix4f& operator =(const Matrix4f &other)
	{
		M[0] = other.M[0];
		M[1] = other.M[1];
		M[2] = other.M[2];
		M[3] = other.M[3];

		R[0] = other.R[0];
		R[1] = other.R[1];
		R[2] = other.R[2];
		R[3] = other.R[3];
		return *this;
	}
	Matrix4f operator +(const Matrix4f &other) const
	{
		return Matrix4f(
			M[0] + other.M[0],
			M[1] + other.M[1],
			M[2] + other.M[2],
			M[3] + other.M[3],

			R[0] + other.R[0],
			R[1] + other.R[1],
			R[2] + other.R[2],
			R[3] + other.R[3]
		);
	}
	Matrix4f operator -(const Matrix4f &other) const
	{
		return Matrix4f(
			M[0] - other.M[0],
			M[1] - other.M[1],
			M[2] - other.M[2],
			M[3] - other.M[3],

			R[0] - other.R[0],
			R[1] - other.R[1],
			R[2] - other.R[2],
			R[3] - other.R[3]
		);
	}
	Matrix4f operator *(const Matrix4f &other) const
	{
		Matrix4f result;

		for (int32 i = 0; i < 4; i++)
		{
			result.M[i] =
				M[0] * VectorMath::Shuffle<0, 0, 0, 0>(other.M[i]) +
				M[1] * VectorMath::Shuffle<1, 1, 1, 1>(other.M[i]) +
				M[2] * VectorMath::Shuffle<2, 2, 2, 2>(other.M[i]) +
				M[3] * VectorMath::Shuffle<3, 3, 3, 3>(other.M[i]);
		}

		for (int32 i = 0; i < 4; i++)
		{
			result.R[i] =
				R[0] * VectorMath::Shuffle<0, 0, 0, 0>(other.R[i]) +
				R[1] * VectorMath::Shuffle<1, 1, 1, 1>(other.R[i]) +
				R[2] * VectorMath::Shuffle<2, 2, 2, 2>(other.R[i]) +
				R[3] * VectorMath::Shuffle<3, 3, 3, 3>(other.R[i]);
		}

		return result;
	}
	Matrix4f operator *(float scalar) const
	{
		vfloat4 vector = vfloat4(scalar);

		return Matrix4f(
			M[0] * vector,
			M[1] * vector,
			M[2] * vector,
			M[3] * vector,

			R[0] * vector,
			R[1] * vector,
			R[2] * vector,
			R[3] * vector
		);
	}
	vfloat3 operator *(const vfloat3 &other) const
	{
		vfloat4 vector = vfloat4(other.X, other.Y, other.Z, 1.0f);

		return vfloat3(
			_mm_cvtss_f32(_mm_dp_ps(M[0], vector, 0xf1)),
			_mm_cvtss_f32(_mm_dp_ps(M[1], vector, 0xf1)),
			_mm_cvtss_f32(_mm_dp_ps(M[2], vector, 0xf1))
		);
	}
	Matrix4f& operator +=(const Matrix4f &other)
	{
		*this = *this + other;
		return *this;
	}
	Matrix4f& operator -=(const Matrix4f &other)
	{
		*this = *this - other;
		return *this;
	}
	Matrix4f& operator *=(const Matrix4f &other)
	{
		*this = *this * other;
		return *this;
	}
	Matrix4f& operator *=(float scalar)
	{
		*this = *this * scalar;
		return *this;
	}
	bool operator ==(const Matrix4f &other) const
	{
		return M[0] == other.M[0] && M[1] == other.M[1] && M[2] == other.M[2] && M[3] == other.M[3];
	}
	bool operator !=(const Matrix4f &other) const
	{
		return M[0] != other.M[0] || M[1] != other.M[1] || M[2] != other.M[2] || M[3] != other.M[3];
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