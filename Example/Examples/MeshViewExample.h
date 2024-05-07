#pragma once
#include "../Example.h"

class MeshViewExample : public Example
{
private:
	Thread *Threads[2] = { };
	Mesh *Meshes[4] = { };
	int32 CurrentMesh = 0;
	Vector3f Position;
	Matrix4f Rotation;

public:
	explicit MeshViewExample(int32 width, int32 height);
	void Run() override;

private:
	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit);
	void ThreadFunc(int32 threadNumber);
};