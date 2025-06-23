#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class PrimitivesExample : public ExampleBase
{
private:
	Mesh *Meshes[8] = { };
	Matrix4f Rotation;
	vfloat2 RotationVelocity;

public:
	explicit PrimitivesExample(int32 width, int32 height);

	void Run() override;

private:
	void HandleInput();
	void Render();

	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit, int32 meshIndex);
};