#pragma once
#include "../FastPix3D.h"
#include "Math_.h"
#include "VectorMath.h"

__declspec(align(32)) class FASTPIX3D_API Matrix4
{
private:
	// Each row is a 256-bit vector containing the complete transformation matrix in the low 128 bits and the rotation-only part in the high 128 bits.
	vfloat8 M[4];

	explicit Matrix4(const vfloat8 &m1, const vfloat8 &m2, const vfloat8 &m3, const vfloat8 &m4) :
		M { m1, m2, m3, m4 }
	{
	}
	explicit Matrix4(
		const vfloat4 &m1, const vfloat4 &m2, const vfloat4 &m3, const vfloat4 &m4,
		const vfloat4 &r1, const vfloat4 &r2, const vfloat4 &r3, const vfloat4 &r4) :
		M { Pack(m1, r1), Pack(m2, r2), Pack(m3, r3), Pack(m4, r4) }
	{
	}
	explicit Matrix4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44,
		float r11, float r12, float r13, float r14,
		float r21, float r22, float r23, float r24,
		float r31, float r32, float r33, float r34,
		float r41, float r42, float r43, float r44) :
		Matrix4(
			vfloat4(m11, m12, m13, m14),
			vfloat4(m21, m22, m23, m24),
			vfloat4(m31, m32, m33, m34),
			vfloat4(m41, m42, m43, m44),

			vfloat4(r11, r12, r13, r14),
			vfloat4(r21, r22, r23, r24),
			vfloat4(r31, r32, r33, r34),
			vfloat4(r41, r42, r43, r44)
		)
	{
	}

public:
	readonly_property(Matrix4, RotationPart)
	{
		vfloat4 r0 = GetRotation(M[0]);
		vfloat4 r1 = GetRotation(M[1]);
		vfloat4 r2 = GetRotation(M[2]);
		vfloat4 r3 = GetRotation(M[3]);

		return Matrix4(
			r0, r1, r2, r3,
			r0, r1, r2, r3
		);
	}

	Matrix4()
	{
	}
	Matrix4(const Matrix4 &other) :
		M { other.M[0], other.M[1], other.M[2], other.M[3] }
	{
	}
	explicit Matrix4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44) :
		Matrix4(
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44,

			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		)
	{
	}
	explicit Matrix4(const float *m) :
		Matrix4(
			vfloat4(m),
			vfloat4(&m[4]),
			vfloat4(&m[8]),
			vfloat4(&m[12]),

			vfloat4(1, 0, 0, 0),
			vfloat4(0, 1, 0, 0),
			vfloat4(0, 0, 1, 0),
			vfloat4(0, 0, 0, 1)
		)
	{
	}

	static Matrix4 Identity()
	{
		return Matrix4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4 Scale(float x, float y, float z)
	{
		return Matrix4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4 Scale(const vfloat3 &vector)
	{
		return Scale(vector.X, vector.Y, vector.Z);
	}
	static Matrix4 Scale(float uniform)
	{
		return Scale(uniform, uniform, uniform);
	}
	static Matrix4 RotateX(float angle)
	{
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		return Matrix4(
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
	static Matrix4 RotateY(float angle)
	{
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		return Matrix4(
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
	static Matrix4 RotateZ(float angle)
	{
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		return Matrix4(
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
	static Matrix4 Translate(float x, float y, float z)
	{
		return Matrix4(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		);
	}
	static Matrix4 Translate(const vfloat3 &vector)
	{
		return Translate(vector.X, vector.Y, vector.Z);
	}

	Matrix4 Transpose() const
	{
		vfloat4 m0 = GetMatrix(M[0]);
		vfloat4 m1 = GetMatrix(M[1]);
		vfloat4 m2 = GetMatrix(M[2]);
		vfloat4 m3 = GetMatrix(M[3]);

		vfloat4 r0 = GetRotation(M[0]);
		vfloat4 r1 = GetRotation(M[1]);
		vfloat4 r2 = GetRotation(M[2]);
		vfloat4 r3 = GetRotation(M[3]);

		_vfloat4 tm0 = _mm_unpacklo_ps(m0, m1);
		_vfloat4 tm1 = _mm_unpackhi_ps(m0, m1);
		_vfloat4 tm2 = _mm_unpacklo_ps(m2, m3);
		_vfloat4 tm3 = _mm_unpackhi_ps(m2, m3);

		_vfloat4 tr0 = _mm_unpacklo_ps(r0, r1);
		_vfloat4 tr1 = _mm_unpackhi_ps(r0, r1);
		_vfloat4 tr2 = _mm_unpacklo_ps(r2, r3);
		_vfloat4 tr3 = _mm_unpackhi_ps(r2, r3);

		return Matrix4(
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
	Matrix4 ToNormalMatrix() const
	{
		vfloat4 m0 = GetMatrix(M[0]);
		vfloat4 m1 = GetMatrix(M[1]);
		vfloat4 m2 = GetMatrix(M[2]);

		float det =
			m0[0] * (m1[1] * m2[2] - m1[2] * m2[1]) -
			m0[1] * (m1[0] * m2[2] - m1[2] * m2[0]) +
			m0[2] * (m1[0] * m2[1] - m1[1] * m2[0]);

		if (Math::Abs(det) < 1e-6f)
		{
			return Matrix4::Identity();
		}
		else
		{
			vfloat4 inverseDet = vfloat4(1 / det);

			return Matrix4(
				vfloat4(
					m1[1] * m2[2] - m1[2] * m2[1],
					m1[2] * m2[0] - m1[0] * m2[2],
					m1[0] * m2[1] - m1[1] * m2[0],
					0
				) * inverseDet,
				vfloat4(
					m0[2] * m2[1] - m0[1] * m2[2],
					m0[0] * m2[2] - m0[2] * m2[0],
					m0[1] * m2[0] - m0[0] * m2[1],
					0
				) * inverseDet,
				vfloat4(
					m0[1] * m1[2] - m0[2] * m1[1],
					m0[2] * m1[0] - m0[0] * m1[2],
					m0[0] * m1[1] - m0[1] * m1[0],
					0
				) * inverseDet,
				vfloat4(),
				vfloat4(1, 0, 0, 0),
				vfloat4(0, 1, 0, 0),
				vfloat4(0, 0, 1, 0),
				vfloat4(0, 0, 0, 1)
			);
		}
	}

	Matrix4& operator =(const Matrix4 &other)
	{
		M[0] = other.M[0];
		M[1] = other.M[1];
		M[2] = other.M[2];
		M[3] = other.M[3];

		return *this;
	}
	Matrix4 operator *(const Matrix4 &other) const
	{
		Matrix4 result;

		for (int32 i = 0; i < 4; i++)
		{
			vfloat8 value = M[0] * Broadcast<0>(other.M[i]);
			value = VectorMath::MulAdd(M[1], Broadcast<1>(other.M[i]), value);
			value = VectorMath::MulAdd(M[2], Broadcast<2>(other.M[i]), value);
			value = VectorMath::MulAdd(M[3], Broadcast<3>(other.M[i]), value);

			result.M[i] = value;
		}

		return result;
	}
	vfloat3 operator *(const vfloat3 &other) const
	{
		vfloat4 vector = _mm_insert_ps(other, _mm_set_ss(1), 0x30);

		return vfloat3(
			_mm_cvtss_f32(_mm_dp_ps(GetMatrix(M[0]), vector, 0xf1)),
			_mm_cvtss_f32(_mm_dp_ps(GetMatrix(M[1]), vector, 0xf1)),
			_mm_cvtss_f32(_mm_dp_ps(GetMatrix(M[2]), vector, 0xf1))
		);
	}
	Matrix4& operator *=(const Matrix4 &other)
	{
		*this = *this * other;
		return *this;
	}
	bool operator ==(const Matrix4 &other) const
	{
		return M[0] == other.M[0] && M[1] == other.M[1] && M[2] == other.M[2] && M[3] == other.M[3];
	}
	bool operator !=(const Matrix4 &other) const
	{
		return M[0] != other.M[0] || M[1] != other.M[1] || M[2] != other.M[2] || M[3] != other.M[3];
	}
	void* operator new[](size_t size)
	{
		return _aligned_malloc(size, 32);
	}
	void operator delete[](void *ptr)
	{
		_aligned_free(ptr);
	}

	__forceinline static vfloat8 Pack(const vfloat4 &matrix, const vfloat4 &rotation)
	{
		// Store the complete transformation matrix in lanes 0-3 and the rotation-only part in lanes 4-7.
		return _mm256_insertf128_ps(_mm256_castps128_ps256(matrix), rotation, 1);
	}
	__forceinline static vfloat4 GetMatrix(const vfloat8 &value)
	{
		// Lanes 0-3 contain the complete transformation matrix.
		return _mm256_castps256_ps128(value);
	}
	__forceinline static vfloat4 GetRotation(const vfloat8 &value)
	{
		// Lanes 4-7 contain the rotation-only part of the matrix.
		return _mm256_extractf128_ps(value, 1);
	}
	template<int32 index>
	__forceinline static vfloat8 Broadcast(const vfloat8 &value)
	{
		static_assert(index >= 0 && index < 4);

		return _mm256_permute_ps(value, _MM_SHUFFLE(index, index, index, index));
	}
};