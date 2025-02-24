#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class ShadowMapCubemapExample : public ExampleBase
{
private:
	Mesh *Map = nullptr;
	Mesh *LightBulb = nullptr;
	Stopwatch LightMovementStopwatch;

public:
	explicit ShadowMapCubemapExample(int32 width, int32 height);
	~ShadowMapCubemapExample();

	void Run() override;

private:
	void HandleInput();
	void Render();

	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit, int32 part);
};