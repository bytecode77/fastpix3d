#pragma once
#include "../Example.h"

class SphereMapExample : public Example
{
private:
	Thread *Threads[4] = { };
	Mesh *Mesh = nullptr;

public:
	explicit SphereMapExample(int32 width, int32 height);
	void Run() override;

private:
	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit);
	void ThreadFunc(int32 threadNumber);
};