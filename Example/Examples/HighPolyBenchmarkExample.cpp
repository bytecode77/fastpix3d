#include "HighPolyBenchmarkExample.h"

HighPolyBenchmarkExample::HighPolyBenchmarkExample(int32 width, int32 height) : Example(width, height, "High Poly Benchmark")
{
	RenderUnit->RenderStates.CameraSpace = Matrix4f::Translate(0, -.2f, 1) * Matrix4f::RotateX(-15);
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(15, 20, 50);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 255, 180);

	for (int32 i = 0; i < 4; i++)
	{
		Threads[i] = new Thread([](void *sender, void *parameter)
		{
			((HighPolyBenchmarkExample*)sender)->ThreadFunc((int32)parameter);
		}, this, (void*)i);
	}
}
void HighPolyBenchmarkExample::Run()
{
	LoadScene();

	bool rotate = false;
	int64 rotationStart = 0;

	while (!Input::HasExited() && !Input::GetKeyDown(SDLK_ESCAPE))
	{
		Window->Lock();
		RenderUnit->ClearFrameBuffer(0, 100, 170);
		RenderUnit->ClearDepthBuffer();
		RenderUnit->Statistics.Clear();

		if (rotate)
		{
			Angle = (Math::Time() - rotationStart) * .03f;
		}

		for (int32 i = 0; i < 4; i++) Threads[i]->Start();
		for (int32 i = 0; i < 4; i++) Threads[i]->Join();

		DrawStatisticsBox(10, 10);
		DrawFieldSet(360, 10, 0, 0, 30, "Space", "start animation ", nullptr);
		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();

		if (Input::GetKeyDown(SDLK_SPACE))
		{
			rotate = true;
			rotationStart = Math::Time();
		}
	}
}

void HighPolyBenchmarkExample::LoadScene()
{
	Mesh = Mesh::Load("Assets\\Models\\Lion\\Lion.obj");
	Mesh->FitToBoundingBox(Box3f(Vector3f(-.5f), Vector3f(.5f)), true);
}
void HighPolyBenchmarkExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(*Mesh, Matrix4f::RotateY(Angle + 180));
}
void HighPolyBenchmarkExample::ThreadFunc(int32 threadNumber)
{
	::RenderUnit renderUnitCopy = *RenderUnit;
	renderUnitCopy.Statistics.Clear();

	switch (threadNumber)
	{
		case 0: renderUnitCopy.RenderStates.Workload = Workload::Quarter1; break;
		case 1: renderUnitCopy.RenderStates.Workload = Workload::Quarter2; break;
		case 2: renderUnitCopy.RenderStates.Workload = Workload::Quarter3; break;
		case 3: renderUnitCopy.RenderStates.Workload = Workload::Quarter4; break;
	}

	DrawScene(renderUnitCopy);

	if (threadNumber == 0)
	{
		RenderUnit->Statistics.Merge(renderUnitCopy.Statistics);
	}
	else
	{
		RenderUnit->Statistics.RenderedTriangleCount += renderUnitCopy.Statistics.RenderedTriangleCount;
	}
}