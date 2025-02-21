#pragma once
#include "../Example.h"

class PrimitivesExample : public Example
{
private:
	bool UseTextures = true;
	Thread *Threads[4] = { };
	Texture *Texture = nullptr;
	Mesh *Meshes[8] = { };

public:
	explicit PrimitivesExample(int32 width, int32 height);
	void Run() override;

private:
	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit, int32 meshIndex);
	void ThreadFunc(int32 threadNumber);
};