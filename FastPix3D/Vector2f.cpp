#include "FastPix3D.h"

Vector2f::Vector2f()
{
	X = 0;
	Y = 0;
}
Vector2f::Vector2f(float x, float y)
{
	X = x;
	Y = y;
}

float Vector2f::getLength()
{
	return sqrtf(X * X + Y * Y);
}

Vector2f Vector2f::Normalize()
{
	float d = 1 / sqrt(X * X + Y * Y);
	return Vector2f(X * d, Y * d);
}
Vector2f Vector2f::Normalize(float length)
{
	float d = length / sqrtf(X * X + Y * Y);
	return Vector2f(X * d, Y * d);
}
float Vector2f::DotProduct(Vector2f a)
{
	return X * a.X + Y * a.Y;
}

Vector2f Vector2f::operator +(Vector2f a)
{
	return Vector2f(X + a.X, Y + a.Y);
}
Vector2f Vector2f::operator +()
{
	return Vector2f(X, Y);
}
Vector2f Vector2f::operator -(Vector2f a)
{
	return Vector2f(X - a.X, Y - a.Y);
}
Vector2f Vector2f::operator -()
{
	return Vector2f(-X, -Y);
}
Vector2f Vector2f::operator *(Vector2f a)
{
	return Vector2f(X * a.X, Y * a.Y);
}
Vector2f Vector2f::operator *(float a)
{
	return Vector2f(X * a, Y * a);
}
Vector2f Vector2f::operator /(float a)
{
	return Vector2f(X / a, Y / a);
}
void Vector2f::operator +=(Vector2f a)
{
	X += a.X;
	Y += a.Y;
}
void Vector2f::operator -=(Vector2f a)
{
	X -= a.X;
	Y -= a.Y;
}
void Vector2f::operator *=(float a)
{
	X *= a;
	Y *= a;
}
void Vector2f::operator /=(float a)
{
	X /= a;
	Y /= a;
}
bool Vector2f::operator ==(Vector2f a)
{
	return X == a.X && Y == a.Y;
}
bool Vector2f::operator !=(Vector2f a)
{
	return X != a.X || Y != a.Y;
}