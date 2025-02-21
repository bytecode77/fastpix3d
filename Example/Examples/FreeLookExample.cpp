#include "FreeLookExample.h"

FreeLookExample::FreeLookExample(int32 width, int32 height) : Example(width, height, "Freelook")
{
	RenderUnit->RenderStates.ClipNear = .5f;

	for (int32 i = 0; i < 4; i++)
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

	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
	while (!Input::HasExited() && !Input::GetKeyDown(Scancode::Escape))
	{
		Window->Lock();
		RenderUnit->ClearDepthBuffer();
		RenderUnit->Statistics.Clear();

		if (!Skybox) RenderUnit->ClearFrameBuffer();
		RenderUnit->RenderStates.CameraSpace = FreeLook->CameraSpace;

		for (int32 i = 0; i < 4; i++) Threads[i]->Start();
		for (int32 i = 0; i < 4; i++) Threads[i]->Join();

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
			if (Input::GetKeyDown((Scancode)((int32)Scancode::D1 - 1 + i)) && currentMap != i - 1)
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
	if (Skybox)
	{
		::RenderUnit skyboxRenderUnit = *RenderUnit;
		skyboxRenderUnit.Statistics.Clear();

		skyboxRenderUnit.RenderStates.CameraSpace = FreeLook->CameraSpace.RotationPart;
		skyboxRenderUnit.RenderStates.ZEnable = false;
		skyboxRenderUnit.RenderStates.ZWriteEnable = false;

		switch (threadNumber)
		{
			case 0: skyboxRenderUnit.RenderStates.Workload = Workload::Quarter1; break;
			case 1: skyboxRenderUnit.RenderStates.Workload = Workload::Quarter2; break;
			case 2: skyboxRenderUnit.RenderStates.Workload = Workload::Quarter3; break;
			case 3: skyboxRenderUnit.RenderStates.Workload = Workload::Quarter4; break;
		}

		DrawScene(skyboxRenderUnit, 0);

		if (threadNumber == 0)
		{
			RenderUnit->Statistics.Merge(skyboxRenderUnit.Statistics);
		}
	}

	::RenderUnit mapRenderUnitCopy = *RenderUnit;
	mapRenderUnitCopy.Statistics.Clear();

	switch (threadNumber)
	{
		case 0: mapRenderUnitCopy.RenderStates.Workload = Workload::Quarter1; break;
		case 1: mapRenderUnitCopy.RenderStates.Workload = Workload::Quarter2; break;
		case 2: mapRenderUnitCopy.RenderStates.Workload = Workload::Quarter3; break;
		case 3: mapRenderUnitCopy.RenderStates.Workload = Workload::Quarter4; break;
	}

	DrawScene(mapRenderUnitCopy, 1);

	mapRenderUnitCopy.RenderDeferredPass();

	if (threadNumber == 0)
	{
		RenderUnit->Statistics.Merge(mapRenderUnitCopy.Statistics);
	}
}