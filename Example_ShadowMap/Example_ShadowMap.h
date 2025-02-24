#pragma once
#include "ExampleBase.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd);

class ShadowMapExample : public ExampleBase
{
private:
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
	Surface *HoundEyeEye = nullptr;
	Texture *HoundEyeEyeTexture[3];
	Mesh *LightBulb = nullptr;
	bool IsFreeLook = false;

public:
	explicit ShadowMapExample(int32 width, int32 height);
	~ShadowMapExample();

	void Run() override;

private:
	void HandleInput();
	void Render();

	void LoadScene();
	void DrawScene(::RenderUnit &renderUnit, int32 part);
	void DebugShadowMap(int32 x, int32 y, int32 width, int32 height, float zFrom, float zTo) const;
};