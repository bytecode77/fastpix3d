#include "Example_ShadowMapCubemap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	ShadowMapCubemapExample example = ShadowMapCubemapExample(1280, 720);
	example.Run();
	return TRUE;
}

ShadowMapCubemapExample::ShadowMapCubemapExample(int32 width, int32 height) : ExampleBase(width, height, "Shadow Mapping")
{
	RenderStates->ShadowMap = RenderTarget(1024, 1024 * 6, _aligned_malloc(2048 * 2048 * 6 * 4, 32));
	RenderStates->ClipNear = .1f;
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(120, 70, 0);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Type = LightType::Point;
	RenderStates->Lights[0].Intensity = 10;
	RenderStates->Lights[0].Color = Color(255, 240, 120);
	RenderStates->ShadowMapFunc = ShadowMapFunc::Point;
	RenderStates->ShadowMapProjection = ShadowMapProjection::Cubemap;
	RenderStates->ShadowMapDepthBias = .05f;

	LoadScene();

	FreeLook->Speed = .2f;
	LightMovementStopwatch = Stopwatch::StartNew();
	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}

void ShadowMapCubemapExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawPerformanceBox(10, 10, FreeLook->Position);
		DrawControlsBox(
			"Controls",
			10,
			-10,
			"WSAD",
			"Move",
			-1,
			"Space",
			"Light Movement",
			LightMovementStopwatch.IsRunning ? 1 : 0,
			nullptr);
		DrawControlsBox(
			"Render",
			-10,
			-10,
			"1 - 3",
			"Shadow Map Resolution",
			-1,
			"P",
			"PCF",
			RenderStates->ShadowMapFunc == ShadowMapFunc::Pcf ? 1 : 0,
			"T",
			"Texture Filtering",
			RenderStates->TextureFilteringEnable ? 1 : 0,
			"X",
			"Wireframe",
			Wireframe ? 1 : 0,
			nullptr);

		Window->Unlock();
		Window->Flip();

		Input::Update();
		Input::CenterMouse(*Window);
		FPSCounter->Frame();
	}
}

void ShadowMapCubemapExample::HandleInput()
{
	HandleBaseInput();

	FreeLook->Update();

	if (Input::GetKeyPressed(Scancode::Space))
	{
		LightMovementStopwatch.Toggle();
		ShadowMapResolutionChanged = true;
	}

	if (Input::GetKeyPressed(Scancode::D1))
	{
		RenderStates->ShadowMap = RenderTarget(512, 512 * 6, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .08f;
		ShadowMapResolutionChanged = true;
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderStates->ShadowMap = RenderTarget(1024, 1024 * 6, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .05f;
		ShadowMapResolutionChanged = true;
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderStates->ShadowMap = RenderTarget(2048, 2048 * 6, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .03f;
		ShadowMapResolutionChanged = true;
	}

	if (Input::GetKeyPressed(Scancode::P))
	{
		RenderStates->ShadowMapFunc = RenderStates->ShadowMapFunc == ShadowMapFunc::Pcf ? ShadowMapFunc::Point : ShadowMapFunc::Pcf;
	}
}
void ShadowMapCubemapExample::Render()
{
	bool renderShadowMap = ShadowMapResolutionChanged || LightMovementStopwatch.IsRunning;
	ShadowMapResolutionChanged = false;

	RenderUnit->Statistics.Clear();
	RenderUnit->ClearFrameBuffer(*RenderStates);
	RenderUnit->ClearDepthBuffer(*RenderStates);
	if (renderShadowMap) RenderUnit->ClearShadowMap(*RenderStates);

	RenderStates->ViewMatrix = FreeLook->ViewMatrix;
	RenderStates->Lights[0].Position = vfloat3(
		Math::Cos(LightMovementStopwatch.ElapsedMilliseconds * .01f) * 6 + 1.5f,
		Math::Sin(LightMovementStopwatch.ElapsedMilliseconds * .03f) - 3,
		Math::Sin(LightMovementStopwatch.ElapsedMilliseconds * .01f) * 6 - .5f);

	int32 threadIds[6];

	// Render all 6 faces of shadow cubemap.
	if (renderShadowMap)
	{
		for (int32 i = 0; i < 6; i++)
		{
			threadIds[i] = ThreadPool::Start([this, i]
			{
				::RenderStates shadowMapRenderStates = *RenderStates;
				shadowMapRenderStates.ShadowMap = RenderTarget(shadowMapRenderStates.ShadowMap.Width, shadowMapRenderStates.ShadowMap.Width, RenderStates->ShadowMap.GetBuffer<float>(shadowMapRenderStates.ShadowMap.Width * shadowMapRenderStates.ShadowMap.Width * i));
				shadowMapRenderStates.Rasterizer = Rasterizer::ShadowMap;
				shadowMapRenderStates.Lights[0].Rotation = GetCubemapDirection(i);
				shadowMapRenderStates.CountTotalTriangles = false;
				DrawScene(shadowMapRenderStates, 0);
			});
		}

		for (int32 i = 0; i < 6; i++)
		{
			ThreadPool::Join(threadIds[i]);
		}
	}

	// Render scene with and project shadow map.
	for (int32 i = 0; i < 4; i++)
	{
		threadIds[i] = ThreadPool::Start([this, i]
		{
			::RenderStates sceneRenderStates = *RenderStates;
			sceneRenderStates.SetWorkload(i, 4);
			DrawScene(sceneRenderStates, 0);
			DrawScene(sceneRenderStates, 1);

			if (Wireframe)
			{
				sceneRenderStates.Rasterizer = Rasterizer::Wireframe;
				sceneRenderStates.CountTotalTriangles = false;
				sceneRenderStates.CountRenderedTriangles = false;
				DrawScene(sceneRenderStates, 0);
				DrawScene(sceneRenderStates, 1);
			}
		});
	}

	for (int32 i = 0; i < 4; i++)
	{
		ThreadPool::Join(threadIds[i]);
	}
}

void ShadowMapCubemapExample::LoadScene()
{
	Map = Mesh::Load("Assets\\Maps\\hl_c1a0b\\hl_c1a0.obj");
	Map->FitToBoundingBox(Box3f(20), true);
	Map->SetSpecular(0, 0);

	for (int32 i = 0; i < Map->SurfaceCount; i++)
	{
		Surface *surface = Map->GetSurface(i);
		if (surface->Texture && !lstrcmpA(surface->Texture->FileName, "material_37_2_baseColor.png"))
		{
			surface->BlendMode = BlendMode::Add;
		}
	}

	FreeLook->Position = vfloat3(-4, -4, 3);
	FreeLook->Rotation = vfloat2(110, 0);

	for (int32 i = 0; i < 8; i++)
	{
		char path[MAX_PATH];
		lstrcpyA(path, "Assets\\Textures\\LightSprite ");
		path[lstrlenA(path) - 1] = '1' + i;
		lstrcatA(path, ".png");

		LightBulbTextures[i] = Texture::FromFile(path);
	}

	LightBulb = PrimitiveFactory::Plane(1, 1);
	LightBulb->GetSurface(0)->BlendMode = BlendMode::Add;
}
void ShadowMapCubemapExample::DrawScene(::RenderStates &renderStates, int32 part)
{
	switch (part)
	{
		case 0:
		{
			RenderUnit->DrawMesh(renderStates, *Map, Matrix4f::Identity());
			break;
		}
		case 1:
		{
			LightBulb->SetTexture(LightBulbTextures[LightMovementStopwatch.ElapsedMicroseconds / 100000 % 8]);

			for (int32 i = 0; i < 2; i++)
			{
				RenderUnit->DrawMesh(renderStates, *LightBulb, Matrix4f::Scale(1.5f) * Matrix4f::RotateX(-90) * renderStates.ViewMatrix.RotationPart.Transpose() * Matrix4f::Translate(renderStates.Lights[0].Position));
			}
			break;
		}
	}
}