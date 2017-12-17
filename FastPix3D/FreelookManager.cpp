#include "FastPix3D.h"

FreelookManager::FreelookManager()
{
	OldTime = SDL_GetTicks();
	Position = Vector3f();
	Rotation = Vector3f();
	Translation = Vector3f();
	RotationTranslation = Vector3f();
	Speed = 1;
}
FreelookManager::FreelookManager(Vector3f position)
{
	OldTime = SDL_GetTicks();
	Position = position;
	Rotation = Vector3f();
	Translation = Vector3f();
	RotationTranslation = Vector3f();
	Speed = 1;
}
FreelookManager::FreelookManager(Vector3f position, Vector3f rotation)
{
	OldTime = SDL_GetTicks();
	Position = position;
	Rotation = rotation;
	Translation = Vector3f();
	RotationTranslation = Vector3f();
	Speed = 1;
}
FreelookManager::FreelookManager(Vector3f position, Vector3f rotation, float speed)
{
	OldTime = SDL_GetTicks();
	Position = position;
	Rotation = rotation;
	Translation = Vector3f();
	RotationTranslation = Vector3f();
	Speed = speed;
}

Matrix FreelookManager::Update()
{
	clock_t time = clock();
	float speed = (time - OldTime) * Speed * .003f * (Input::getKeyDown(SDLK_LSHIFT) + 1);
	OldTime = time;

	int32 walkX = Input::getKeyDown(SDLK_d) - Input::getKeyDown(SDLK_a);
	int32 walkZ = Input::getKeyDown(SDLK_w) - Input::getKeyDown(SDLK_s);

	Matrix walkMatrix = Matrix::Translate(Vector3f(-walkX * speed, 0, -walkZ * speed)) * Matrix::RotateX(-Rotation.X) * Matrix::RotateY(-Rotation.Y);
	Translation = (Translation + walkMatrix * Vector3f()) * .8f;
	Position += Translation;

	RotationTranslation = (RotationTranslation - Vector3f(Input::getMouseYSpeed() * .1f, Input::getMouseXSpeed() * .1f, 0)) * .75f;
	Rotation += RotationTranslation;
	Rotation.X = min(90, max(-90, Rotation.X));

	return getCameraSpace();
}
void FreelookManager::StopVelocities()
{
	Translation = Vector3f();
	RotationTranslation = Vector3f();
}

Matrix FreelookManager::getCameraSpace()
{
	return Matrix::Translate(Position) * getCameraSpaceRotationOnly();
}
Matrix FreelookManager::getCameraSpaceRotationOnly()
{
	return Matrix::RotateY(Rotation.Y) * Matrix::RotateX(Rotation.X) * Matrix::RotateZ(Rotation.Z);
}
Vector3f FreelookManager::getPosition()
{
	return Position;
}
Vector3f FreelookManager::getRotation()
{
	return Rotation;
}
float FreelookManager::getSpeed()
{
	return Speed;
}

void FreelookManager::setPosition(Vector3f position)
{
	Position = position;
}
void FreelookManager::setRotation(Vector3f rotation)
{
	Rotation = rotation;
}
void FreelookManager::setSpeed(float speed)
{
	Speed = speed;
}