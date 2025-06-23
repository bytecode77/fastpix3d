#include "Example_HighPolyBenchmark.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	HighPolyBenchmarkExample example = HighPolyBenchmarkExample(1280, 720);
	example.Run();
	return TRUE;
}

HighPolyBenchmarkExample::HighPolyBenchmarkExample(int32 width, int32 height) : ExampleBase(width, height, "High Poly Benchmark")
{
	RenderUnit->RenderStates.ViewMatrix = Matrix4f::Translate(0, -.6f, 3) * Matrix4f::RotateX(-15);
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(15, 20, 50);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 255, 180);

	LoadScene();
}

void HighPolyBenchmarkExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawStatisticsBox(10, 10);
		DrawFieldSet(250, 10, 200, 0, 20, "Space", RotationStopwatch.IsRunning ? "Animation ON " : "Animation OFF", nullptr);
		DrawFieldSet(460, 10, 0, 0, 20, "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);

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
	RenderUnit->ClearFrameBuffer(0, 100, 170);
	RenderUnit->ClearDepthBuffer();
	RenderUnit->Statistics.Clear();

	int32 threadIds[4];
	for (int32 i = 0; i < 4; i++)
	{
		threadIds[i] = ThreadPool::Start([this, i]
		{
			::RenderUnit renderUnitCopy = *RenderUnit;
			renderUnitCopy.Statistics.Clear();

			renderUnitCopy.RenderStates.SetWorkload(i, 4);

			DrawScene(renderUnitCopy);

			RenderUnit->Statistics.Merge(renderUnitCopy.Statistics);

			if (Wireframe)
			{
				renderUnitCopy.RenderStates.Rasterizer = Rasterizer::Wireframe;
				DrawScene(renderUnitCopy);
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
void HighPolyBenchmarkExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(*Mesh, Matrix4f::RotateY(RotationStopwatch.ElapsedMilliseconds * .03f + 180));
}