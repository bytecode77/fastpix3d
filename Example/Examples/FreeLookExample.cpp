#include "FreeLookExample.h"

FreeLookExample::FreeLookExample(int32 width, int32 height) : Example(width, height, "Freelook")
{
	RenderUnit->RenderStates.ClipNear = .5f;

	for (int32 i = 0; i < 8; i++)
	{
		Threads[i] = new Thread([](void *sender, void *parameter)
		{
			((FreeLookExample*)sender)->ThreadFunc((int32)parameter);
		}, this, (void*)i);
	}
}
void FreeLookExample::Run()
{
	int32 currentMap = 0;
	LoadScene(currentMap);

	Input::SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
	while (!Input::HasExited() && !Input::GetKeyDown(SDLK_ESCAPE))
	{
		Window->Lock();
		RenderUnit->ClearDepthBuffer();
		RenderUnit->Statistics.Clear();

		if (Skybox)
		{
			RenderUnit->RenderStates.CameraSpace = FreeLook->CameraSpace.RotationPart;
			RenderUnit->RenderStates.ZEnable = false;
			RenderUnit->RenderStates.ZWriteEnable = false;

			for (int32 i = 0; i < 4; i++) Threads[i]->Start();
			for (int32 i = 0; i < 4; i++) Threads[i]->Join();

			RenderUnit->RenderStates.ZEnable = true;
			RenderUnit->RenderStates.ZWriteEnable = true;
		}
		else
		{
			RenderUnit->ClearFrameBuffer();
		}

		RenderUnit->RenderStates.CameraSpace = FreeLook->CameraSpace;

		for (int32 i = 4; i < 8; i++) Threads[i]->Start();
		for (int32 i = 4; i < 8; i++) Threads[i]->Join();

		DrawStatisticsBox(10, 10);
		DrawPositionBox(360, 10, FreeLook->Position);
		DrawFieldSet(570, 10, 0, 0, 30, "WSAD + Mouse", "move", "Shift", "run", "Key 1-7", "choose map ", nullptr);
		Window->Unlock();
		Window->Flip();

		Input::Update();
		FreeLook->Update();
		Input::CenterMouse(*Window);
		FPSCounter->Frame();

		for (int32 i = 1; i <= 7; i++)
		{
			if (Input::GetKeyDown(SDLK_0 + i) && currentMap != i - 1)
			{
				currentMap = i - 1;
				LoadScene(currentMap);
			}
		}
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
			Map->FitToBoundingBox(Box3f(Vector3f(-100), Vector3f(100)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Dust_XX.png");

			FreeLook->Position = Vector3f(63, 0, -30);
			FreeLook->Rotation = Vector2f(220, 0);
			break;
		case 1:
			Map = Mesh::Load("Assets\\Maps\\hl_c1a0\\hl_c1a0.obj");
			Map->FitToBoundingBox(Box3f(Vector3f(-50), Vector3f(50)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			FreeLook->Position = Vector3f(-25, -9, -11);
			FreeLook->Rotation = Vector2f(135, 0);
			break;
		case 2:
			Map = Mesh::Load("Assets\\Maps\\de_inferno\\de_inferno.obj");
			Map->FitToBoundingBox(Box3f(Vector3f(-100), Vector3f(100)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Inferno_XX.png");

			FreeLook->Position = Vector3f(-83, 6, -49);
			FreeLook->Rotation = Vector2f(50, 0);
			break;
		case 3:
			Map = Mesh::Load("Assets\\Maps\\cs_siege\\cs_siege.obj");
			Map->FitToBoundingBox(Box3f(Vector3f(-100), Vector3f(100)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Siege_XX.png");

			FreeLook->Position = Vector3f(-12, 5, 16);
			FreeLook->Rotation = Vector2f(90, 0);
			break;
		case 4:
			Map = Mesh::Load("Assets\\Maps\\cs_italy\\cs_italy.obj");
			Map->FitToBoundingBox(Box3f(Vector3f(-100), Vector3f(100)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Inferno_XX.png");

			FreeLook->Position = Vector3f(-26, -1, -82);
			FreeLook->Rotation = Vector2f(90, 0);
			break;
		case 5:
			Map = Mesh::Load("Assets\\Maps\\cs_office\\cs_office.obj");
			Map->FitToBoundingBox(Box3f(Vector3f(-100), Vector3f(100)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1));
			Map->FlipTriangles();

			Skybox = CreateSkybox("Assets\\Skyboxes\\Office_XX.png");

			FreeLook->Position = Vector3f(49, 5, 12);
			FreeLook->Rotation = Vector2f(-90, 0);
			break;
		case 6:
			Map = Mesh::Load("Assets\\Maps\\DoomHangar\\DoomHangar.obj");
			Map->FitToBoundingBox(Box3f(Vector3f(-100), Vector3f(100)), true);
			Map->TransformVertices(Matrix4f::Scale(-1, 1, 1) * Matrix4f::RotateX(90));
			Map->FlipTriangles();

			FreeLook->Position = Vector3f(-20, -1, -7);
			FreeLook->Rotation = Vector2f(0, 0);

			RenderUnit->RenderStates.FogEnable = true;
			RenderUnit->RenderStates.FogNear = 20;
			RenderUnit->RenderStates.FogFar = 50;
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
			renderUnit.DrawMesh(*Map, Matrix4f::RotateX(-90));
			break;
	}
}
void FreeLookExample::ThreadFunc(int32 threadNumber)
{
	::RenderUnit renderUnitCopy = *RenderUnit;
	renderUnitCopy.Statistics.Clear();

	switch (threadNumber % 4)
	{
		case 0: renderUnitCopy.RenderStates.Workload = Workload::Quarter1; break;
		case 1: renderUnitCopy.RenderStates.Workload = Workload::Quarter2; break;
		case 2: renderUnitCopy.RenderStates.Workload = Workload::Quarter3; break;
		case 3: renderUnitCopy.RenderStates.Workload = Workload::Quarter4; break;
	}

	DrawScene(renderUnitCopy, threadNumber / 4);
	renderUnitCopy.RenderDeferredPass();

	if (renderUnitCopy.RenderStates.Workload == Workload::Quarter1)
	{
		RenderUnit->Statistics.Merge(renderUnitCopy.Statistics);
	}
	else
	{
		RenderUnit->Statistics.RenderedTriangleCount += renderUnitCopy.Statistics.RenderedTriangleCount;
	}
}