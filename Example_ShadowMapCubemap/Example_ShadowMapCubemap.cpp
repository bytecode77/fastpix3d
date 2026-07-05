#include "Example_ShadowMapCubemap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	ShadowMapCubemapExample example = ShadowMapCubemapExample(1280, 720);
	example.Run();
	return TRUE;
}

ShadowMapCubemapExample::ShadowMapCubemapExample(int32 width, int32 height) : ExampleBase(width, height, "Shadow Mapping")
{
	RenderUnit->RenderStates.ShadowMap = RenderTarget(1024, 1024 * 6, _aligned_malloc(2048 * 2048 * 6 * 4, 32));
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(120, 70, 0);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Type = LightType::Point;
	RenderUnit->RenderStates.Lights[0].Intensity = 10;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 240, 120);
	RenderUnit->RenderStates.ShadowMapFunc = ShadowMapFunc::Point;
	RenderUnit->RenderStates.ShadowMapProjection = ShadowMapProjection::Cubemap;
	RenderUnit->RenderStates.ShadowMapDepthBias = .05f;

	LoadScene();

	FreeLook->Speed = .2f;
	LightMovementStopwatch = Stopwatch::StartNew();
	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}
ShadowMapCubemapExample::~ShadowMapCubemapExample()
{
	_aligned_free(RenderUnit->RenderStates.ShadowMap.Buffer);
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
			RenderUnit->RenderStates.ShadowMapFunc == ShadowMapFunc::Pcf ? 1 : 0,
			"T",
			"Texture Filtering",
			RenderUnit->RenderStates.TextureFilteringEnable ? 1 : 0,
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
		RenderUnit->RenderStates.ShadowMap = RenderTarget(512, 512 * 6, RenderUnit->RenderStates.ShadowMap.Buffer);
		RenderUnit->RenderStates.ShadowMapDepthBias = .08f;
		ShadowMapResolutionChanged = true;
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(1024, 1024 * 6, RenderUnit->RenderStates.ShadowMap.Buffer);
		RenderUnit->RenderStates.ShadowMapDepthBias = .05f;
		ShadowMapResolutionChanged = true;
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(2048, 2048 * 6, RenderUnit->RenderStates.ShadowMap.Buffer);
		RenderUnit->RenderStates.ShadowMapDepthBias = .03f;
		ShadowMapResolutionChanged = true;
	}

	if (Input::GetKeyPressed(Scancode::P))
	{
		RenderUnit->RenderStates.ShadowMapFunc = RenderUnit->RenderStates.ShadowMapFunc == ShadowMapFunc::Pcf ? ShadowMapFunc::Point : ShadowMapFunc::Pcf;
	}
}
void ShadowMapCubemapExample::Render()
{
	bool renderShadowMap = ShadowMapResolutionChanged || LightMovementStopwatch.IsRunning;
	ShadowMapResolutionChanged = false;

	RenderUnit->ClearFrameBuffer();
	RenderUnit->ClearDepthBuffer();
	if (renderShadowMap) RenderUnit->ClearShadowMap();
	RenderUnit->Statistics.Clear();

	RenderUnit->RenderStates.ViewMatrix = FreeLook->ViewMatrix;
	RenderUnit->RenderStates.Lights[0].Position = vfloat3(
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
				::RenderUnit renderUnitCopy = *RenderUnit;
				renderUnitCopy.Statistics.Clear();

				float directionX = 0;
				float directionY = 0;

				switch (i)
				{
					case 0:
						directionX = 90;
						directionY = 0;
						break;
					case 1:
						directionX = -90;
						directionY = 0;
						break;
					case 2:
						directionX = 0;
						directionY = -90;
						break;
					case 3:
						directionX = 0;
						directionY = 90;
						break;
					case 4:
						directionX = 0;
						directionY = 0;
						break;
					case 5:
						directionX = 180;
						directionY = 0;
						break;
				}

				int32 size = renderUnitCopy.RenderStates.ShadowMap.Width;
				renderUnitCopy.RenderStates.ShadowMap = RenderTarget(size, size, RenderUnit->RenderStates.ShadowMap.GetBuffer<float>(size * size * i));
				renderUnitCopy.RenderStates.Rasterizer = Rasterizer::ShadowMap;
				renderUnitCopy.RenderStates.Lights[0].Rotation = vfloat3(directionX, directionY, 0);
				DrawScene(renderUnitCopy, 0);

				RenderUnit->Statistics.Merge(renderUnitCopy.Statistics, false, true);
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
			::RenderUnit renderUnitCopy = *RenderUnit;
			renderUnitCopy.Statistics.Clear();

			renderUnitCopy.RenderStates.SetWorkload(i, 4);
			DrawScene(renderUnitCopy, 0);

			renderUnitCopy.RenderStates.ShadowMapFunc = ShadowMapFunc::None;
			DrawScene(renderUnitCopy, 1);

			RenderUnit->Statistics.Merge(renderUnitCopy.Statistics, true, true);

			if (Wireframe)
			{
				renderUnitCopy.RenderStates.Rasterizer = Rasterizer::Wireframe;
				DrawScene(renderUnitCopy, 0);
				DrawScene(renderUnitCopy, 1);
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
void ShadowMapCubemapExample::DrawScene(::RenderUnit &renderUnit, int32 part)
{
	switch (part)
	{
		case 0:
		{
			renderUnit.DrawMesh(*Map, Matrix4f::Identity());
			break;
		}
		case 1:
		{
			LightBulb->SetTexture(LightBulbTextures[LightMovementStopwatch.ElapsedMicroseconds / 100000 % 8]);

			for (int32 i = 0; i < 2; i++)
			{
				renderUnit.DrawMesh(*LightBulb, Matrix4f::Scale(1.5f) * Matrix4f::RotateX(-90) * renderUnit.RenderStates.ViewMatrix.RotationPart.Transpose() * Matrix4f::Translate(renderUnit.RenderStates.Lights[0].Position));
			}
			break;
		}
	}
}