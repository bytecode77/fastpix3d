#include "FreeLook.h"
#include "Input.h"
#include "Math_.h"

FreeLook::FreeLook() :
	_LastTime(Math::Time()),
	_Speed(1)
{
}
FreeLook::FreeLook(const Vector3f &position) :
	_LastTime(Math::Time()),
	_Position(position),
	_Speed(1)
{
}
FreeLook::FreeLook(const Vector3f &position, const Vector2f &rotation) :
	_LastTime(Math::Time()),
	_Position(position),
	_Rotation(rotation),
	_Speed(1)
{
}
FreeLook::FreeLook(const Vector3f &position, const Vector2f &rotation, float speed) :
	_LastTime(Math::Time()),
	_Position(position),
	_Rotation(rotation),
	_Speed(speed)
{
}

void FreeLook::Update()
{
	int64 time = Math::Time();
	float speed = (time - _LastTime) * _Speed * .003f * (Input::GetKeyDown(Scancode::LeftShift) + 1);
	_LastTime = time;

	Matrix4f walkMatrix =
		Matrix4f::Translate(
			(Input::GetKeyDown(Scancode::D) - Input::GetKeyDown(Scancode::A)) * speed,
			0,
			(Input::GetKeyDown(Scancode::W) - Input::GetKeyDown(Scancode::S)) * speed
		) *
		Matrix4f::RotateX(_Rotation.Y) *
		Matrix4f::RotateY(_Rotation.X);

	_PositionVelocity = (_PositionVelocity + walkMatrix * Vector3f()) * .8f;
	_Position += _PositionVelocity;

	_RotationVelocity = (_RotationVelocity - Vector2f(-Input::GetMouseSpeed().X * .1f, -Input::GetMouseSpeed().Y * .1f)) * .75f;
	_Rotation += _RotationVelocity;
	_Rotation.Y = Math::Clamp(_Rotation.Y, -90.0f, 90.0f);
}
void FreeLook::ResetVelocities()
{
	_PositionVelocity = Vector3f();
	_RotationVelocity = Vector2f();
}