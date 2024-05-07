#pragma once
#include "../Example.h"

class ShadowMapExample : public Example
{
private:
	Thread *Threads[8] = { };
	Mesh *Floor = nullptr;
	Mesh *Ceiling = nullptr;
	Mesh *Wall = nullptr;
	Mesh *Door = nullptr;
	Mesh *Fence = nullptr;
	Mesh *Cage = nullptr;
	Mesh *CageGround = nullptr;
	Mesh *Crate = nullptr;
	Mesh *Scientist1 = nullptr;
	Mesh *Scientist2 = nullptr;
	Mesh *Scientist3 = nullptr;
	Mesh *HoundEye = nullptr;
	Mesh *LightBulb = nullptr;

public:
	explicit ShadowMapExample(int32 width, int32 height);
	void Run() override;

private:
	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit);
	void ThreadFunc(int32 renderPass);
};