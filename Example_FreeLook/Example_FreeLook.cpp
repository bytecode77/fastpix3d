#include "Example_FreeLook.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	FreeLookExample example = FreeLookExample(1280, 720);
	example.Run();
	return TRUE;
}

FreeLookExample::FreeLookExample(int32 width, int32 height) : ExampleBase(width, height, "Freelook")
{
	RenderUnit->RenderStates.ClipNear = .5f;

	LoadScene(CurrentMap);

	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}

void FreeLookExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawStatisticsBox(10, 10);
		DrawPositionBox(250, 10, FreeLook->Position);
		DrawFieldSet(450, 10, 250, 0, 20, "WSAD + Mouse", "move", "Shift", "run", "Key 1-8", "choose map", nullptr);
		DrawFieldSet(710, 10, 0, 0, 20, "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);

		Window->Unlock();
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
		}
	}
}
void FreeLookExample::Render()
{
	RenderUnit->ClearDepthBuffer();
	if (!Skybox) RenderUnit->ClearFrameBuffer();
	RenderUnit->Statistics.Clear();

	RenderUnit->RenderStates.ViewMatrix = FreeLook->ViewMatrix;

	int32 threadIds[4];
	for (int32 i = 0; i < 4; i++)
	{
		threadIds[i] = ThreadPool::Start([this, i]
		{
			if (Skybox)
			{
				::RenderUnit skyboxRenderUnit = *RenderUnit;
				skyboxRenderUnit.Statistics.Clear();

				skyboxRenderUnit.RenderStates.SetWorkload(i, 4);
				skyboxRenderUnit.RenderStates.ViewMatrix = skyboxRenderUnit.RenderStates.ViewMatrix.RotationPart;
				skyboxRenderUnit.RenderStates.ZEnable = false;
				skyboxRenderUnit.RenderStates.ZWriteEnable = false;

				DrawScene(skyboxRenderUnit, 0);

				RenderUnit->Statistics.Merge(skyboxRenderUnit.Statistics);

				if (Wireframe)
				{
					skyboxRenderUnit.RenderStates.Rasterizer = Rasterizer::Wireframe;
					DrawScene(skyboxRenderUnit, 0);
				}
			}

			::RenderUnit mapRenderUnitCopy = *RenderUnit;
			mapRenderUnitCopy.Statistics.Clear();

			mapRenderUnitCopy.RenderStates.SetWorkload(i, 4);

			DrawScene(mapRenderUnitCopy, 1);

			RenderUnit->Statistics.Merge(mapRenderUnitCopy.Statistics);

			if (Wireframe)
			{
				mapRenderUnitCopy.RenderStates.Rasterizer = Rasterizer::Wireframe;
				mapRenderUnitCopy.RenderStates.ZWriteEnable = mapRenderUnitCopy.RenderStates.FogEnable;
				DrawScene(mapRenderUnitCopy, 1);
			}

			mapRenderUnitCopy.RenderFog();
		});
	}

	for (int32 i = 0; i < 4; i++)
	{
		ThreadPool::Join(threadIds[i]);
	}
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

	RenderUnit->RenderStates.FogEnable = false;

	switch (mapNumber)
	{
		case 0:
			Map = Mesh::Load("Assets\\Maps\\de_dust2\\de_dust2.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Dust_XX.png");

			FreeLook->Position = vfloat3(63, 0, -30);
			FreeLook->Rotation = vfloat2(220, 0);
			break;
		case 1:
			Map = Mesh::Load("Assets\\Maps\\hl_c1a0\\hl_c1a0.obj");
			Map->FitToBoundingBox(Box3f(100), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(-90));
			Map->FlipTriangles();

			FreeLook->Position = vfloat3(-25, -9, -11);
			FreeLook->Rotation = vfloat2(135, 0);
			break;
		case 2:
			Map = Mesh::Load("Assets\\Maps\\de_inferno\\de_inferno.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Inferno_XX.png");

			FreeLook->Position = vfloat3(-83, 6, -49);
			FreeLook->Rotation = vfloat2(50, 0);
			break;
		case 3:
			Map = Mesh::Load("Assets\\Maps\\cs_siege\\cs_siege.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Siege_XX.png");

			FreeLook->Position = vfloat3(-12, 5, 16);
			FreeLook->Rotation = vfloat2(90, 0);
			break;
		case 4:
			Map = Mesh::Load("Assets\\Maps\\cs_italy\\cs_italy.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(-90));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Inferno_XX.png");

			FreeLook->Position = vfloat3(-26, -1, -82);
			FreeLook->Rotation = vfloat2(90, 0);
			break;
		case 5:
			Map = Mesh::Load("Assets\\Maps\\cs_office\\cs_office.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(-90));
			Map->FlipTriangles();

			for (int32 i = 0; i < Map->SurfaceCount; i++)
			{
				Surface *surface = Map->GetSurface(i);
				if (!lstrcmpA(surface->Texture->FileName, "office_material_153.png"))
				{
					// Overhead projector
					surface->BlendMode = BlendMode::Add;
				}
			}

			Skybox = CreateSkybox("Assets\\Skyboxes\\Office_XX.png");

			FreeLook->Position = vfloat3(49, 5, 12);
			FreeLook->Rotation = vfloat2(-90, 0);

			RenderUnit->RenderStates.FogEnable = true;
			RenderUnit->RenderStates.FogNear = 20;
			RenderUnit->RenderStates.FogFar = 100;
			RenderUnit->RenderStates.FogColor = Color(230, 225, 220);
			break;
		case 6:
			Map = Mesh::Load("Assets\\Maps\\Apartment\\Apartment.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-.3f, .3f, .3f));
			Map->FlipTriangles();

			FreeLook->Position = vfloat3(15, 2, 8);
			FreeLook->Rotation = vfloat2(180, 15);
			break;
		case 7:
			Map = Mesh::Load("Assets\\Maps\\DoomHangar\\DoomHangar.obj");
			Map->FitToBoundingBox(Box3f(200), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Doom_XX.png");

			FreeLook->Position = vfloat3(-20, -1, -7);
			FreeLook->Rotation = vfloat2();

			RenderUnit->RenderStates.FogEnable = true;
			RenderUnit->RenderStates.FogNear = 20;
			RenderUnit->RenderStates.FogFar = 100;
			RenderUnit->RenderStates.FogColor = Color();
			break;
	}
}
void FreeLookExample::DrawScene(::RenderUnit &renderUnit, int32 part)
{
	switch (part)
	{
		case 0:
			renderUnit.DrawMesh(*Skybox, Matrix4f::Scale(10));
			break;
		case 1:
			renderUnit.DrawMesh(*Map, Matrix4f::Identity());
			break;
	}
}