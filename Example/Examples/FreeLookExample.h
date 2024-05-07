#pragma once
#include "../Example.h"

class FreeLookExample : public Example
{
private:
	Thread *Threads[8] = { };
	Mesh *Map = nullptr;
	Mesh *Skybox = nullptr;

public:
	explicit FreeLookExample(int32 width, int32 height);
	void Run() override;

private:
	void LoadScene(int32 mapNumber);
	void DrawScene(::RenderUnit &renderUnit, int32 part);
	void ThreadFunc(int32 threadNumber);
};