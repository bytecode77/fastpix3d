#include "FastPix3D.h"

Vector3f::Vector3f()
{
	X = 0;
	Y = 0;
	Z = 0;
}
Vector3f::Vector3f(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

float Vector3f::getLength()
{
	return sqrtf(X * X + Y * Y + Z * Z);
}

Vector3f Vector3f::Normalize()
{
	float d = 1 / sqrt(X * X + Y * Y + Z * Z);
	return Vector3f(X * d, Y * d, Z * d);
}
Vector3f Vector3f::Normalize(float length)
{
	float d = length / sqrtf(X * X + Y * Y + Z * Z);
	return Vector3f(X * d, Y * d, Z * d);
}
float Vector3f::DotProduct(Vector3f vector)
{
	return X * vector.X + Y * vector.Y + Z * vector.Z;
}
Vector3f Vector3f::CrossProduct(Vector3f vector)
{
	return Vector3f(Y * vector.Z - Z * vector.Y, Z * vector.X - X * vector.Z, X * vector.Y - Y * vector.X);
}

Vector3f Vector3f::operator +(Vector3f vector)
{
	return Vector3f(X + vector.X, Y + vector.Y, Z + vector.Z);
}
Vector3f Vector3f::operator +()
{
	return Vector3f(X, Y, Z);
}
Vector3f Vector3f::operator -(Vector3f vector)
{
	return Vector3f(X - vector.X, Y - vector.Y, Z - vector.Z);
}
Vector3f Vector3f::operator -()
{
	return Vector3f(-X, -Y, -Z);
}
Vector3f Vector3f::operator *(Vector3f vector)
{
	return Vector3f(X * vector.X, Y * vector.Y, Z * vector.Z);
}
Vector3f Vector3f::operator *(float a)
{
	return Vector3f(X * a, Y * a, Z * a);
}
Vector3f Vector3f::operator /(float a)
{
	return Vector3f(X / a, Y / a, Z / a);
}
void Vector3f::operator +=(Vector3f vector)
{
	X += vector.X;
	Y += vector.Y;
	Z += vector.Z;
}
void Vector3f::operator -=(Vector3f vector)
{
	X -= vector.X;
	Y -= vector.Y;
	Z -= vector.Z;
}
void Vector3f::operator *=(float a)
{
	X *= a;
	Y *= a;
	Z *= a;
}
void Vector3f::operator /=(float a)
{
	X /= a;
	Y /= a;
	Z /= a;
}
bool Vector3f::operator ==(Vector3f vector)
{
	return X == vector.X && Y == vector.Y && Z == vector.Z;
}
bool Vector3f::operator !=(Vector3f vector)
{
	return X != vector.X || Y != vector.Y || Z != vector.Z;
}