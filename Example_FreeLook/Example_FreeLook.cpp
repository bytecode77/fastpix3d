#include "Example_FreeLook.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	FreeLookExample example = FreeLookExample(1280, 720);
	example.Run();
	return TRUE;
}

FreeLookExample::FreeLookExample(int32 width, int32 height) : ExampleBase(width, height, "Freelook")
{
	RenderStates->ClipNear = .5f;

	LoadScene(CurrentMap);

	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}

void FreeLookExample::Run()
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

void FreeLookExample::HandleInput()
{
	HandleBaseInput();

	FreeLook->Update();

	for (int32 i = 1; i <= 8; i++)
	{
		if (Input::GetKeyPressed((Scancode)((int32)Scancode::D1 - 1 + i)) && CurrentMap != i - 1)
		{
			CurrentMap = i - 1;
			LoadScene(CurrentMap);
			FPSCounter->ResetMinFrameTime();
		}
	}
}
void FreeLookExample::Render()
{
	RenderUnit->Statistics.Clear();

	if (Skybox)
	{
		RenderUnit->ClearDepthBuffer(*RenderStates);
	}
	else
	{
		ThreadPool::Run({
			[&] { RenderUnit->ClearFrameBuffer(*RenderStates); },
			[&] { RenderUnit->ClearDepthBuffer(*RenderStates); }
			});
	}

	RenderStates->ViewMatrix = FreeLook->ViewMatrix;

	ThreadPool::Run(4, [this](WorkPartition workPartition)
	{
		if (Skybox)
		{
			::RenderStates skyboxRenderStates = *RenderStates;
			skyboxRenderStates.ViewMatrix = skyboxRenderStates.ViewMatrix.RotationPart;
			skyboxRenderStates.DepthMode = DepthMode::None;
			DrawScene(skyboxRenderStates, workPartition, 0);

			if (Wireframe)
			{
				skyboxRenderStates.Rasterizer = Rasterizer::Wireframe;
				DrawScene(skyboxRenderStates, workPartition, 0);
			}
		}

		::RenderStates mapRenderStates = *RenderStates;
		DrawScene(mapRenderStates, workPartition, 1);

		if (Wireframe)
		{
			mapRenderStates.Rasterizer = Rasterizer::Wireframe;
			mapRenderStates.DepthMode = mapRenderStates.FogEnable ? DepthMode::ReadWrite : DepthMode::Read;
			DrawScene(mapRenderStates, workPartition, 1);
		}

		RenderUnit->RenderFog(mapRenderStates, workPartition);
	});
}
void FreeLookExample::DrawHud()
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
				"Shift",
				"Run",
				-1,
				"1 - 8",
				"Select Map",
				-1,
				nullptr);
		},
		[&]
		{
			DrawControlsBox(
				"Render",
				-10,
				-10,
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

void FreeLookExample::LoadScene(int32 mapNumber)
{
	if (Map)
	{
		delete Map;
		Map = nullptr;
	}

	if (Skybox)
	{
		delete Skybox;
		Skybox = nullptr;
	}

	RenderStates->FogEnable = false;

	switch (mapNumber)
	{
		case 0:
		{
			Map = Mesh::Load("Assets\\Maps\\de_dust2\\de_dust2.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1) * Matrix4::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Dust_XX.png");

			FreeLook->Position = vfloat3(63, 0, -30);
			FreeLook->Rotation = vfloat2(220, 0);
			break;
		}
		case 1:
		{
			Map = Mesh::Load("Assets\\Maps\\hl_c1a0\\hl_c1a0.obj");
			Map->FitToBoundingBox(Box3f(100), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1) * Matrix4::RotateX(-90));
			Map->FlipTriangles();

			FreeLook->Position = vfloat3(-25, -9, -11);
			FreeLook->Rotation = vfloat2(135, 0);
			break;
		}
		case 2:
		{
			Map = Mesh::Load("Assets\\Maps\\de_inferno\\de_inferno.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1) * Matrix4::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Inferno_XX.png");

			FreeLook->Position = vfloat3(-83, 6, -49);
			FreeLook->Rotation = vfloat2(50, 0);
			break;
		}
		case 3:
		{
			Map = Mesh::Load("Assets\\Maps\\cs_siege\\cs_siege.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1) * Matrix4::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Siege_XX.png");

			FreeLook->Position = vfloat3(-12, 5, 16);
			FreeLook->Rotation = vfloat2(90, 0);
			break;
		}
		case 4:
		{
			Map = Mesh::Load("Assets\\Maps\\cs_italy\\cs_italy.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1) * Matrix4::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Inferno_XX.png");

			FreeLook->Position = vfloat3(-26, -1, -82);
			FreeLook->Rotation = vfloat2(90, 0);
			break;
		}
		case 5:
		{
			Map = Mesh::Load("Assets\\Maps\\cs_office\\cs_office.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1) * Matrix4::RotateX(-90));
			Map->FlipTriangles();
			Map->FindSurface("office_material_153")->BlendMode = BlendMode::Add;

			Skybox = CreateSkybox("Assets\\Skyboxes\\Office_XX.png");

			FreeLook->Position = vfloat3(49, 5, 12);
			FreeLook->Rotation = vfloat2(-90, 0);

			RenderStates->FogEnable = true;
			RenderStates->FogNear = 20;
			RenderStates->FogFar = 100;
			RenderStates->FogColor = Color(230, 225, 220);
			break;
		}
		case 6:
		{
			Map = Mesh::Load("Assets\\Maps\\Apartment\\Apartment.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-.3f, .3f, .3f));
			Map->FlipTriangles();

			int32 found = 0;
			for (int32 i = 0; i < Map->SurfaceCount; i++)
			{
				Surface *surface = Map->Surfaces[i];
				if (!lstrcmpA(surface->Texture->FileName, "door01a.jpg"))
				{
					if (found++ > 0)
					{
						Map->RemoveSurface(surface);
						i--;
					}
				}
			}

			FreeLook->Position = vfloat3(15, 2, 8);
			FreeLook->Rotation = vfloat2(180, 15);
			break;
		}
		case 7:
		{
			Map = Mesh::Load("Assets\\Maps\\DoomHangar\\DoomHangar.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Doom_XX.png");

			FreeLook->Position = vfloat3(-20, -1, -7);
			FreeLook->Rotation = vfloat2();

			RenderStates->FogEnable = true;
			RenderStates->FogNear = 20;
			RenderStates->FogFar = 100;
			RenderStates->FogColor = Color();
			break;
		}
	}
}
void FreeLookExample::DrawScene(::RenderStates &renderStates, WorkPartition workPartition, int32 part)
{
	switch (part)
	{
		case 0:
			RenderUnit->DrawMesh(renderStates, workPartition, *Skybox, Matrix4::Scale(10));
			break;
		case 1:
			RenderUnit->DrawMesh(renderStates, workPartition, *Map, Matrix4::Identity());
			break;
	}
}