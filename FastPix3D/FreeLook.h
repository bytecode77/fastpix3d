#pragma once
#include "FastPix3D.h"
#include "Vector3f.h"
#include "Matrix4f.h"

class FreeLook
{
private:
	int64 _LastTime;
	Vector3f _Position;
	Vector2f _Rotation;
	Vector3f _PositionVelocity;
	Vector2f _RotationVelocity;
	float _Speed;

public:
	property_getset(Vector3f, Position)
	{
		return _Position;
	}
	property_set(const Vector3f&, Position)
	{
		_Position = value;
	}
	property_getset(Vector2f, Rotation)
	{
		return _Rotation;
	}
	property_set(const Vector2f&, Rotation)
	{
		_Rotation = value;
	}
	property_getset(float, Speed)
	{
		return _Speed;
	}
	property_set(float, Speed)
	{
		_Speed = value;
	}
	property_get(Matrix4f, CameraSpace)
	{
		return Matrix4f::Translate(-_Position) * Matrix4f::RotateY(-_Rotation.X) * Matrix4f::RotateX(-_Rotation.Y);
	}

	FreeLook();
	explicit FreeLook(const Vector3f &position);
	explicit FreeLook(const Vector3f &position, const Vector2f &rotation);
	explicit FreeLook(const Vector3f &position, const Vector2f &rotation, float speed);

	void Update();
	void ResetVelocities();
};