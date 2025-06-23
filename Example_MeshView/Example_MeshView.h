#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class MeshViewExample : public ExampleBase
{
private:
	Mesh *Meshes[4] = { };
	int32 CurrentMesh = 0;
	vfloat3 Position;
	Matrix4f Rotation;
	vfloat2 RotationVelocity;

public:
	explicit MeshViewExample(int32 width, int32 height);

	void Run() override;

private:
	void HandleInput();
	void Render();

	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit);
};