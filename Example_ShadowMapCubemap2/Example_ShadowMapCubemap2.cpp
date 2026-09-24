#include "Example_ShadowMapCubemap2.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	ShadowMapCubemap2Example example = ShadowMapCubemap2Example(1280, 720);
	example.Run();
	return TRUE;
}

ShadowMapCubemap2Example::ShadowMapCubemap2Example(int32 width, int32 height) : ExampleBase(width, height, "Shadow Mapping")
{
	RenderStates->ShadowMap = RenderTarget(1024, 1024 * 6, ShadowMap->Data);
	RenderStates->ClipNear = .1f;
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(120, 130, 160);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Type = LightType::Point;
	RenderStates->Lights[0].Intensity = 50;
	RenderStates->Lights[0].Color = Color(255, 240, 180);
	RenderStates->ShadowMapFunc = ShadowMapFunc::Point;
	RenderStates->ShadowMapProjection = ShadowMapProjection::Cubemap;
	RenderStates->ShadowMapDepthBias = 1.0075f;

	LoadScene();

	FreeLook->Speed = .2f;
	LightMovementStopwatch = Stopwatch::StartNew();
	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}

void ShadowMapCubemap2Example::Run()
{
	while (!Input::HasExited && !Input::GetKeyPressed(Scancode::Escape))
	{
		HandleInput();
		Render();
		DrawHud();

		Window->Flip();
		Input::Update();
		Input::CenterMouse(*Window);
		FPSCounter->Frame();
	}
}

void ShadowMapCubemap2Example::HandleInput()
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
		RenderStates->ShadowMap = RenderTarget(512, 512 * 6, ShadowMap->Data);
		RenderStates->ShadowMapDepthBias = 1.01f;
		ShadowMapResolutionChanged = true;
		FPSCounter->ResetMinFrameTime();
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderStates->ShadowMap = RenderTarget(1024, 1024 * 6, ShadowMap->Data);
		RenderStates->ShadowMapDepthBias = 1.0075f;
		ShadowMapResolutionChanged = true;
		FPSCounter->ResetMinFrameTime();
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderStates->ShadowMap = RenderTarget(2048, 2048 * 6, ShadowMap->Data);
		RenderStates->ShadowMapDepthBias = 1.005f;
		ShadowMapResolutionChanged = true;
		FPSCounter->ResetMinFrameTime();
	}

	if (Input::GetKeyPressed(Scancode::P))
	{
		RenderStates->ShadowMapFunc = RenderStates->ShadowMapFunc == ShadowMapFunc::Pcf ? ShadowMapFunc::Point : ShadowMapFunc::Pcf;
		FPSCounter->ResetMinFrameTime();
	}
}
void ShadowMapCubemap2Example::Render()
{
	bool renderShadowMap = ShadowMapResolutionChanged || LightMovementStopwatch.IsRunning;
	ShadowMapResolutionChanged = false;

	RenderUnit->Statistics.Clear();

	ThreadPool::Run({
		[&] { RenderUnit->ClearFrameBuffer(*RenderStates); },
		[&] { RenderUnit->ClearDepthBuffer(*RenderStates); },
		[&]
		{
			if (renderShadowMap)
			{
				RenderUnit->ClearShadowMap(*RenderStates);
			}
		}
		});

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

	// Render all 6 faces of the shadow cubemap.
	if (renderShadowMap)
	{
		ThreadPool::Run(6, [this](int32 threadIndex)
		{
			::RenderStates shadowMapRenderStates = *RenderStates;
			shadowMapRenderStates.ShadowMap = RenderTarget(shadowMapRenderStates.ShadowMap.Width, shadowMapRenderStates.ShadowMap.Width, RenderStates->ShadowMap.GetBuffer<float>(shadowMapRenderStates.ShadowMap.Width * shadowMapRenderStates.ShadowMap.Width * threadIndex));
			shadowMapRenderStates.Rasterizer = Rasterizer::ShadowMap;
			shadowMapRenderStates.Lights[0].Rotation = GetCubemapDirection(threadIndex);
			DrawScene(shadowMapRenderStates, WorkPartition(), 0);
		});
	}

	// Render scene and project shadow map.
	ThreadPool::Run(4, [this](WorkPartition workPartition)
	{
		::RenderStates sceneRenderStates = *RenderStates;
		DrawScene(sceneRenderStates, workPartition, 0);
		DrawScene(sceneRenderStates, workPartition, 1);

		if (Wireframe)
		{
			sceneRenderStates.Rasterizer = Rasterizer::Wireframe;
			DrawScene(sceneRenderStates, workPartition, 0);
			DrawScene(sceneRenderStates, workPartition, 1);
		}
	});
}
void ShadowMapCubemap2Example::DrawHud()
{
	ThreadPool::Run({
		[&] { DrawPerformanceBox(10, 10, FreeLook->Position); },
		[&]
		{
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
		},
		[&]
		{
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
		}
		});
}

void ShadowMapCubemap2Example::LoadScene()
{
	Map = Mesh::Load("Assets\\Maps\\Warehouse\\Warehouse.obj");
	Map->SetSpecularIntensity(.015f);
	Map->FitToBoundingBox(Box3f(20), true);

	FreeLook->Position = vfloat3(0, -1, -8);

	LightBulb = PrimitiveFactory::Plane(1, 1);
	LightBulb->SetTexture(Texture::FromFile("Assets\\Textures\\LightSprite.png"));
	LightBulb->Surfaces[0]->BlendMode = BlendMode::Add;
}
void ShadowMapCubemap2Example::DrawScene(::RenderStates &renderStates, WorkPartition workPartition, int32 part)
{
	switch (part)
	{
		case 0:
		{
			RenderUnit->DrawMesh(renderStates, workPartition, *Map, Matrix4::Identity());
			break;
		}
		case 1:
		{
			RenderUnit->DrawMesh(renderStates, workPartition, *LightBulb, Matrix4::Scale(.2f) * Matrix4::RotateX(-90) * renderStates.ViewMatrix.RotationPart.Transpose() * Matrix4::Translate(renderStates.Lights[0].Position));
			break;
		}
	}
}