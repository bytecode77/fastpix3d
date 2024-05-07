#pragma once
#include "../Example.h"

class HighPolyBenchmarkExample : public Example
{
private:
	Thread *Threads[4] = { };
	Mesh *Mesh = nullptr;
	float Angle = 0;

public:
	explicit HighPolyBenchmarkExample(int32 width, int32 height);
	void Run() override;

private:
	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit);
	void ThreadFunc(int32 threadNumber);
};