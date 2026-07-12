#include "Example_ShadowMapCubemap2.h"

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
	RenderStates->AmbientLight = Color(120, 130, 160);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Type = LightType::Point;
	RenderStates->Lights[0].Intensity = 50;
	RenderStates->Lights[0].Color = Color(255, 240, 180);
	RenderStates->ShadowMapFunc = ShadowMapFunc::Point;
	RenderStates->ShadowMapProjection = ShadowMapProjection::Cubemap;
	RenderStates->ShadowMapDepthBias = .075f;

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
		RenderStates->ShadowMapDepthBias = .15f;
		ShadowMapResolutionChanged = true;
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderStates->ShadowMap = RenderTarget(1024, 1024 * 6, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .075f;
		ShadowMapResolutionChanged = true;
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderStates->ShadowMap = RenderTarget(2048, 2048 * 6, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .05f;
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

	int64 t1 = LightMovementStopwatch.ElapsedMilliseconds + 5000;
	float t2 = LightMovementStopwatch.ElapsedMilliseconds * .05f;
	float lightKeyframe = (t1 % (960 * 50)) / 50.0f;
	const float lightSquareWidth = 11;
	const float lightSquareHeight = 5;

	if (lightKeyframe < 300) RenderStates->Lights[0].Position = vfloat3(-lightSquareHeight / 2, 0, lightKeyframe / 300.0f * lightSquareWidth - lightSquareWidth / 2);
	else if (lightKeyframe < 300 + 180) RenderStates->Lights[0].Position = vfloat3((lightKeyframe - 300) / 180.0f * lightSquareHeight - lightSquareHeight / 2, 0, lightSquareWidth / 2);
	else if (lightKeyframe < 300 + 180 + 300) RenderStates->Lights[0].Position = vfloat3(lightSquareHeight / 2, 0, lightSquareWidth / 2 - (lightKeyframe - 300 - 180) / 300.0f * lightSquareWidth);
	else RenderStates->Lights[0].Position = vfloat3(lightSquareHeight / 2 - (lightKeyframe - 300 - 180 - 300) / 180.0f * lightSquareHeight, 0, -lightSquareWidth / 2);

	RenderStates->Lights[0].Position += vfloat3(0, .5f, 0) + vfloat3(Math::Sin(t2), Math::Cos(t2), Math::Sin(t2)) * .3f;

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
	Map = Mesh::Load("Assets\\Maps\\Warehouse\\Warehouse.obj");
	Map->SetSpecularIntensity(.015f);
	Map->FitToBoundingBox(Box3f(20), true);

	FreeLook->Position = vfloat3(0, -1, -8);

	LightBulb = PrimitiveFactory::Plane(1, 1);
	LightBulb->SetTexture(Texture::FromFile("Assets\\Textures\\LightSprite.png"));
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
			RenderUnit->DrawMesh(renderStates, *LightBulb, Matrix4f::Scale(.2f) * Matrix4f::RotateX(-90) * renderStates.ViewMatrix.RotationPart.Transpose() * Matrix4f::Translate(renderStates.Lights[0].Position));
			break;
		}
	}
}