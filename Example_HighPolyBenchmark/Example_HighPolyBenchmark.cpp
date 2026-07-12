#include "Example_HighPolyBenchmark.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	HighPolyBenchmarkExample example = HighPolyBenchmarkExample(1280, 720);
	example.Run();
	return TRUE;
}

HighPolyBenchmarkExample::HighPolyBenchmarkExample(int32 width, int32 height) : ExampleBase(width, height, "High Poly Benchmark")
{
	RenderStates->ViewMatrix = Matrix4f::Translate(0, -.6f, 3) * Matrix4f::RotateX(-15);
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(15, 20, 50);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Color = Color(255, 255, 180);

	LoadScene();
}

void HighPolyBenchmarkExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawPerformanceBox(10, 10);
		DrawControlsBox(
			"Controls",
			10,
			-10,
			"Space",
			"Animation",
			RotationStopwatch.IsRunning ? 1 : 0,
			nullptr);
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

		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();
	}
}

void HighPolyBenchmarkExample::HandleInput()
{
	HandleBaseInput();

	if (Input::GetKeyPressed(Scancode::Space))
	{
		RotationStopwatch.Toggle();
	}
}
void HighPolyBenchmarkExample::Render()
{
	RenderUnit->Statistics.Clear();
	RenderUnit->ClearFrameBuffer(*RenderStates, 0, 100, 170);
	RenderUnit->ClearDepthBuffer(*RenderStates);

	int32 threadIds[4];
	for (int32 i = 0; i < 4; i++)
	{
		threadIds[i] = ThreadPool::Start([this, i]
		{
			::RenderStates meshRenderStates = *RenderStates;
			meshRenderStates.SetWorkload(i, 4);
			DrawScene(meshRenderStates);

			if (Wireframe)
			{
				meshRenderStates.Rasterizer = Rasterizer::Wireframe;
				meshRenderStates.CountTotalTriangles = false;
				meshRenderStates.CountRenderedTriangles = false;
				DrawScene(meshRenderStates);
			}
		});
	}

	for (int32 i = 0; i < 4; i++)
	{
		ThreadPool::Join(threadIds[i]);
	}
}

void HighPolyBenchmarkExample::LoadScene()
{
	Mesh = Mesh::Load("Assets\\Models\\Lion\\Lion.obj");
	Mesh->FitToBoundingBox(Box3f(3), true);
	Mesh->SetSpecular(50, .5f);
}
void HighPolyBenchmarkExample::DrawScene(::RenderStates &renderStates)
{
	RenderUnit->DrawMesh(renderStates, *Mesh, Matrix4f::RotateY(RotationStopwatch.ElapsedMilliseconds * .03f + 180));
}