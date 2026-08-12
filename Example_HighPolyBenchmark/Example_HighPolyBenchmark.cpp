#include "Example_HighPolyBenchmark.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	HighPolyBenchmarkExample example = HighPolyBenchmarkExample(1280, 720);
	example.Run();
	return TRUE;
}

HighPolyBenchmarkExample::HighPolyBenchmarkExample(int32 width, int32 height) : ExampleBase(width, height, "High Poly Benchmark")
{
	RenderStates->ViewMatrix = Matrix4::Translate(0, -.6f, 3) * Matrix4::RotateX(-15);
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(15, 20, 50);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Color = Color(255, 255, 180);

	LoadScene();
}

void HighPolyBenchmarkExample::Run()
{
	while (!Input::HasExited && !Input::GetKeyPressed(Scancode::Escape))
	{
		HandleInput();
		Render();
		DrawHud();

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

	ThreadPool::Run({
		[&] { RenderUnit->ClearFrameBuffer(*RenderStates, 0, 100, 170); },
		[&] { RenderUnit->ClearDepthBuffer(*RenderStates); }
		});

	ThreadPool::Run(4, [this](WorkPartition workPartition)
	{
		::RenderStates meshRenderStates = *RenderStates;
		DrawScene(meshRenderStates, workPartition);

		if (Wireframe)
		{
			meshRenderStates.Rasterizer = Rasterizer::Wireframe;
			DrawScene(meshRenderStates, workPartition);
		}
	});
}
void HighPolyBenchmarkExample::DrawHud()
{
	ThreadPool::Run({
		[&] { DrawPerformanceBox(10, 10); },
		[&]
		{
			DrawControlsBox(
				"Controls",
				10,
				-10,
				"Space",
				"Animation",
				RotationStopwatch.IsRunning ? 1 : 0,
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

void HighPolyBenchmarkExample::LoadScene()
{
	Mesh = Mesh::Load("Assets\\Models\\Lion\\Lion.obj");
	Mesh->FitToBoundingBox(Box3f(3), true);
	Mesh->SetSpecular(50, .5f);
}
void HighPolyBenchmarkExample::DrawScene(::RenderStates &renderStates, WorkPartition workPartition)
{
	RenderUnit->DrawMesh(renderStates, workPartition, *Mesh, Matrix4::RotateY(RotationStopwatch.ElapsedMilliseconds * .03f + 180));
}