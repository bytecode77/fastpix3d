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
	RenderUnit->RenderStates.AmbientLight = Color(120, 130, 160);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Type = LightType::Point;
	RenderUnit->RenderStates.Lights[0].Intensity = 50;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 240, 180);
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

		DrawStatisticsBox(10, 10);
		DrawFieldSet(250, 10, 310, 0, 20, "Move mouse", "rotation", "Key 1-3", "Shadow map resolution", nullptr);
		DrawFieldSet(570, 10, 0, 0, 20, "Space", LightMovementStopwatch.IsRunning ? "Light movement ON " : "Light movement OFF", "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "P", RenderUnit->RenderStates.ShadowMapFunc == ShadowMapFunc::Pcf ? "PCF is ON" : "PCF is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);

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
	}

	if (Input::GetKeyPressed(Scancode::D1))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(512, 512 * 6, RenderUnit->RenderStates.ShadowMap.Buffer);
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(1024, 1024 * 6, RenderUnit->RenderStates.ShadowMap.Buffer);
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(2048, 2048 * 6, RenderUnit->RenderStates.ShadowMap.Buffer);
	}

	if (Input::GetKeyPressed(Scancode::P))
	{
		RenderUnit->RenderStates.ShadowMapFunc = RenderUnit->RenderStates.ShadowMapFunc == ShadowMapFunc::Pcf ? ShadowMapFunc::Point : ShadowMapFunc::Pcf;
	}
}
void ShadowMapCubemapExample::Render()
{
	RenderUnit->ClearFrameBuffer();
	RenderUnit->ClearDepthBuffer();
	RenderUnit->ClearShadowMap();
	RenderUnit->Statistics.Clear();

	RenderUnit->RenderStates.ViewMatrix = FreeLook->ViewMatrix;

	int64 t1 = LightMovementStopwatch.ElapsedMilliseconds + 5000;
	float t2 = LightMovementStopwatch.ElapsedMilliseconds * .05f;
	float lightKeyframe = (t1 % (960 * 50)) / 50.0f;
	const float lightSquareWidth = 11;
	const float lightSquareHeight = 5;

	if (lightKeyframe < 300) RenderUnit->RenderStates.Lights[0].Position = vfloat3(-lightSquareHeight / 2, 0, lightKeyframe / 300.0f * lightSquareWidth - lightSquareWidth / 2);
	else if (lightKeyframe < 300 + 180) RenderUnit->RenderStates.Lights[0].Position = vfloat3((lightKeyframe - 300) / 180.0f * lightSquareHeight - lightSquareHeight / 2, 0, lightSquareWidth / 2);
	else if (lightKeyframe < 300 + 180 + 300) RenderUnit->RenderStates.Lights[0].Position = vfloat3(lightSquareHeight / 2, 0, lightSquareWidth / 2 - (lightKeyframe - 300 - 180) / 300.0f * lightSquareWidth);
	else RenderUnit->RenderStates.Lights[0].Position = vfloat3(lightSquareHeight / 2 - (lightKeyframe - 300 - 180 - 300) / 180.0f * lightSquareHeight, 0, -lightSquareWidth / 2);

	RenderUnit->RenderStates.Lights[0].Position += vfloat3(0, .5f, 0) + vfloat3(Math::Sin(t2), Math::Cos(t2), Math::Sin(t2)) * .3f;

	int32 threadIds[6];

	// Render all 6 faces of shadow cubemap.
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
	Map = Mesh::Load("Assets\\Maps\\Warehouse\\Warehouse.obj");
	Map->SetSpecularIntensity(.015f);
	Map->FitToBoundingBox(Box3f(20), true);

	FreeLook->Position = vfloat3(0, -1, -8);

	LightBulb = PrimitiveFactory::Plane(1, 1);
	LightBulb->SetTexture(Texture::FromFile("Assets\\Textures\\LightSprite.png"));
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
			renderUnit.DrawMesh(*LightBulb, Matrix4f::Scale(.2f) * Matrix4f::RotateX(-90) * renderUnit.RenderStates.ViewMatrix.RotationPart.Transpose() * Matrix4f::Translate(renderUnit.RenderStates.Lights[0].Position));
			break;
		}
	}
}