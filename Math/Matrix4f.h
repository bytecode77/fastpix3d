#pragma once
#include "FastPix3D.h"
#include "Vector3f.h"
#include "Math_.h"

class Matrix4f
{
private:
	float M11, M12, M13, M14;
	float M21, M22, M23, M24;
	float M31, M32, M33, M34;
	float M41, M42, M43, M44;

	// Track rotation-only part.
	float R11, R12, R13, R14;
	float R21, R22, R23, R24;
	float R31, R32, R33, R34;
	float R41, R42, R43, R44;

	explicit Matrix4f(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44,
		float r11, float r12, float r13, float r14,
		float r21, float r22, float r23, float r24,
		float r31, float r32, float r33, float r34,
		float r41, float r42, float r43, float r44) :
		M11(m11), M12(m12), M13(m13), M14(m14),
		M21(m21), M22(m22), M23(m23), M24(m24),
		M31(m31), M32(m32), M33(m33), M34(m34),
		M41(m41), M42(m42), M43(m43), M44(m44),
		R11(r11), R12(r12), R13(r13), R14(r14),
		R21(r21), R22(r22), R23(r23), R24(r24),
		R31(r31), R32(r32), R33(r33), R34(r34),
		R41(r41), R42(r42), R43(r43), R44(r44)
	{
	}
public:
	property_get(Matrix4f, RotationPart)
	{
		return Matrix4f(
			R11, R12, R13, R14,
			R21, R22, R23, R24,
			R31, R32, R33, R34,
			R41, R42, R43, R44,

			R11, R12, R13, R14,
			R21, R22, R23, R24,
			R31, R32, R33, R34,
			R41, R42, R43, R44
		);
	}

	Matrix4f() :
		M11(0), M12(0), M13(0), M14(0),
		M21(0), M22(0), M23(0), M24(0),
		M31(0), M32(0), M33(0), M34(0),
		M41(0), M42(0), M43(0), M44(0),
		R11(0), R12(0), R13(0), R14(0),
		R21(0), R22(0), R23(0), R24(0),
		R31(0), R32(0), R33(0), R34(0),
		R41(0), R42(0), R43(0), R44(0)
	{
	}

	static Matrix4f Identity()
	{
		return Matrix4f(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,

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
			0, 0, 0, 1,

			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f Scale(const Vector3f &vector)
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
			0, 0, 0, 1,

			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4f Translate(const Vector3f &vector)
	{
		return Translate(vector.X, vector.Y, vector.Z);
	}

	Matrix4f operator *(const Matrix4f &other) const
	{
		return Matrix4f(
			other.M11 * M11 + other.M12 * M21 + other.M13 * M31 + other.M14 * M41,
			other.M11 * M12 + other.M12 * M22 + other.M13 * M32 + other.M14 * M42,
			other.M11 * M13 + other.M12 * M23 + other.M13 * M33 + other.M14 * M43,
			other.M11 * M14 + other.M12 * M24 + other.M13 * M34 + other.M14 * M44,
			other.M21 * M11 + other.M22 * M21 + other.M23 * M31 + other.M24 * M41,
			other.M21 * M12 + other.M22 * M22 + other.M23 * M32 + other.M24 * M42,
			other.M21 * M13 + other.M22 * M23 + other.M23 * M33 + other.M24 * M43,
			other.M21 * M14 + other.M22 * M24 + other.M23 * M34 + other.M24 * M44,
			other.M31 * M11 + other.M32 * M21 + other.M33 * M31 + other.M34 * M41,
			other.M31 * M12 + other.M32 * M22 + other.M33 * M32 + other.M34 * M42,
			other.M31 * M13 + other.M32 * M23 + other.M33 * M33 + other.M34 * M43,
			other.M31 * M14 + other.M32 * M24 + other.M33 * M34 + other.M34 * M44,
			other.M41 * M11 + other.M42 * M21 + other.M43 * M31 + other.M44 * M41,
			other.M41 * M12 + other.M42 * M22 + other.M43 * M32 + other.M44 * M42,
			other.M41 * M13 + other.M42 * M23 + other.M43 * M33 + other.M44 * M43,
			other.M41 * M14 + other.M42 * M24 + other.M43 * M34 + other.M44 * M44,

			other.R11 * R11 + other.R12 * R21 + other.R13 * R31 + other.R14 * R41,
			other.R11 * R12 + other.R12 * R22 + other.R13 * R32 + other.R14 * R42,
			other.R11 * R13 + other.R12 * R23 + other.R13 * R33 + other.R14 * R43,
			other.R11 * R14 + other.R12 * R24 + other.R13 * R34 + other.R14 * R44,
			other.R21 * R11 + other.R22 * R21 + other.R23 * R31 + other.R24 * R41,
			other.R21 * R12 + other.R22 * R22 + other.R23 * R32 + other.R24 * R42,
			other.R21 * R13 + other.R22 * R23 + other.R23 * R33 + other.R24 * R43,
			other.R21 * R14 + other.R22 * R24 + other.R23 * R34 + other.R24 * R44,
			other.R31 * R11 + other.R32 * R21 + other.R33 * R31 + other.R34 * R41,
			other.R31 * R12 + other.R32 * R22 + other.R33 * R32 + other.R34 * R42,
			other.R31 * R13 + other.R32 * R23 + other.R33 * R33 + other.R34 * R43,
			other.R31 * R14 + other.R32 * R24 + other.R33 * R34 + other.R34 * R44,
			other.R41 * R11 + other.R42 * R21 + other.R43 * R31 + other.R44 * R41,
			other.R41 * R12 + other.R42 * R22 + other.R43 * R32 + other.R44 * R42,
			other.R41 * R13 + other.R42 * R23 + other.R43 * R33 + other.R44 * R43,
			other.R41 * R14 + other.R42 * R24 + other.R43 * R34 + other.R44 * R44
		);
	}
	Vector3f operator *(const Vector3f &other) const
	{
		return Vector3f(
			M11 * other.X + M12 * other.Y + M13 * other.Z + M14,
			M21 * other.X + M22 * other.Y + M23 * other.Z + M24,
			M31 * other.X + M32 * other.Y + M33 * other.Z + M34
		);
	}
	Matrix4f& operator *=(const Matrix4f &other)
	{
		Matrix4f matrix = *this * other;

		M11 = matrix.M11;
		M12 = matrix.M12;
		M13 = matrix.M13;
		M14 = matrix.M14;
		M21 = matrix.M21;
		M22 = matrix.M22;
		M23 = matrix.M23;
		M24 = matrix.M24;
		M31 = matrix.M31;
		M32 = matrix.M32;
		M33 = matrix.M33;
		M34 = matrix.M34;
		M41 = matrix.M41;
		M42 = matrix.M42;
		M43 = matrix.M43;
		M44 = matrix.M44;

		R11 = matrix.R11;
		R12 = matrix.R12;
		R13 = matrix.R13;
		R14 = matrix.R14;
		R21 = matrix.R21;
		R22 = matrix.R22;
		R23 = matrix.R23;
		R24 = matrix.R24;
		R31 = matrix.R31;
		R32 = matrix.R32;
		R33 = matrix.R33;
		R34 = matrix.R34;
		R41 = matrix.R41;
		R42 = matrix.R42;
		R43 = matrix.R43;
		R44 = matrix.R44;

		return *this;
	}
};