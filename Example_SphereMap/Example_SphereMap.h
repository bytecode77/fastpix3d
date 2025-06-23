#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class SphereMapExample : public ExampleBase
{
private:
	::Mesh *Mesh = nullptr;
	::Mesh *MeshHighPoly = nullptr;
	bool UseHighPolyModel = false;
	Stopwatch RotationStopwatch;

public:
	explicit SphereMapExample(int32 width, int32 height);

	void Run() override;

private:
	void HandleInput();
	void Render();

	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit);
};