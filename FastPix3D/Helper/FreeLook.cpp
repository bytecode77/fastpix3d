#include "FreeLook.h"
#include "../Interop/Input.h"
#include "../Interop/System.h"
#include "../Math/Math_.h"

FreeLook::FreeLook() :
	_LastTime(System::Microseconds()),
	_Speed(1)
{
}
FreeLook::FreeLook(const vfloat3 &position) :
	_LastTime(System::Microseconds()),
	_Position(position),
	_Speed(1)
{
}
FreeLook::FreeLook(const vfloat3 &position, const vfloat2 &rotation) :
	_LastTime(System::Microseconds()),
	_Position(position),
	_Rotation(rotation),
	_Speed(1)
{
}
FreeLook::FreeLook(const vfloat3 &position, const vfloat2 &rotation, float speed) :
	_LastTime(System::Microseconds()),
	_Position(position),
	_Rotation(rotation),
	_Speed(speed)
{
}

void FreeLook::Update()
{
	int64 time = System::Microseconds();
	float speed = (time - _LastTime) * _Speed * .000003f * (Input::GetKeyDown(Scancode::LeftShift) + 1);
	_LastTime = time;

	Matrix4f walkMatrix =
		Matrix4f::Translate(
			(Input::GetKeyDown(Scancode::D) - Input::GetKeyDown(Scancode::A)) * speed,
			0,
			(Input::GetKeyDown(Scancode::W) - Input::GetKeyDown(Scancode::S)) * speed
		) *
		Matrix4f::RotateX(_Rotation.Y) *
		Matrix4f::RotateY(_Rotation.X);

	_PositionVelocity = (_PositionVelocity + walkMatrix * vfloat3()) * .8f;
	_Position += _PositionVelocity;

	_RotationVelocity = (_RotationVelocity - vfloat2(-Input::GetMouseSpeed().X * .1f, -Input::GetMouseSpeed().Y * .1f)) * .75f;
	_Rotation += _RotationVelocity;
	_Rotation.Y = Math::Clamp(_Rotation.Y, -90.0f, 90.0f);
}
void FreeLook::ResetVelocities()
{
	_PositionVelocity = vfloat3();
	_RotationVelocity = vfloat2();
}