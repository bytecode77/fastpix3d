#include "FastPix3D.h"

const int32 RenderStates::MaximumLightCount = 8;

bool RenderStates::EnableZWrites;
bool RenderStates::EnableStencilMask;
bool RenderStates::EnableLights;
CullMode RenderStates::CullMode;
float RenderStates::ClipNear;
float RenderStates::Zoom;
Matrix RenderStates::CameraSpace;
Texture *RenderStates::CurrentTexture;
Light **RenderStates::Lights;

void RenderStates::Initialize()
{
	Lights = new Light*[MaximumLightCount];
	for (int32 i = 0; i < MaximumLightCount; i++) Lights[i] = new Light(false);
	Reset();
}
void RenderStates::Destroy()
{
	delete[] Lights;
}

void RenderStates::Reset()
{
	EnableZWrites = true;
	EnableStencilMask = false;
	EnableLights = true;
	CullMode = CullMode::Back;
	ClipNear = 1;
	Zoom = 1;
	CameraSpace = Matrix::Identity();
	CurrentTexture = NULL;
}