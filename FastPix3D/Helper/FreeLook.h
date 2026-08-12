#pragma once
#include "../FastPix3D.h"
#include "../Math/Matrix4.h"
#include "../Math/VectorMath.h"

class FASTPIX3D_API FreeLook
{
private:
	int64 _LastTime;
	vfloat3 _Position;
	vfloat2 _Rotation;
	vfloat3 _PositionVelocity;
	vfloat2 _RotationVelocity;
	float _Speed;

public:
	property_get(vfloat3, Position)
	{
		return _Position;
	}
	property_set(const vfloat3&, Position)
	{
		_Position = value;
	}
	property_get(vfloat2, Rotation)
	{
		return _Rotation;
	}
	property_set(const vfloat2&, Rotation)
	{
		_Rotation = value;
	}
	property_get(float, Speed)
	{
		return _Speed;
	}
	property_set(float, Speed)
	{
		_Speed = value;
	}
	readonly_property(Matrix4, ViewMatrix)
	{
		return Matrix4::Translate(-_Position) * Matrix4::RotateY(-_Rotation.X) * Matrix4::RotateX(-_Rotation.Y);
	}

	FreeLook();
	explicit FreeLook(const vfloat3 &position);
	explicit FreeLook(const vfloat3 &position, const vfloat2 &rotation);
	explicit FreeLook(const vfloat3 &position, const vfloat2 &rotation, float speed);

	void Update();
	void ResetVelocities();
};