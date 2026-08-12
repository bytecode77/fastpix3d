#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class ShadowMapCubemap2Example : public ExampleBase
{
private:
	Mesh *Map = nullptr;
	Mesh *LightBulb = nullptr;
	bool ShadowMapResolutionChanged = false;
	Stopwatch LightMovementStopwatch;

public:
	explicit ShadowMapCubemap2Example(int32 width, int32 height);

	void Run() override;

private:
	void HandleInput();
	void Render();
	void DrawHud();

	void LoadScene();
	void DrawScene(::RenderStates &renderStates, WorkPartition workPartition, int32 part);
};