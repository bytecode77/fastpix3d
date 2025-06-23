#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class FreeLookExample : public ExampleBase
{
private:
	Mesh *Map = nullptr;
	Mesh *Skybox = nullptr;
	int32 CurrentMap = 0;

public:
	explicit FreeLookExample(int32 width, int32 height);

	void Run() override;

private:
	void HandleInput();
	void Render();

	void LoadScene(int32 mapNumber);
	void DrawScene(::RenderUnit &renderUnit, int32 part);
};