#include "Example_SphereMap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	SphereMapExample example = SphereMapExample(1280, 720);
	example.Run();
	return TRUE;
}

SphereMapExample::SphereMapExample(int32 width, int32 height) : ExampleBase(width, height, "Sphere Mapping")
{
	RenderStates->ViewMatrix = Matrix4::Translate(0, -1, 3) * Matrix4::RotateX(-20);
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(40, 40, 40);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Intensity = .7f;

	LoadScene();

	RotationStopwatch = Stopwatch::StartNew();
}

void SphereMapExample::Run()
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

void SphereMapExample::HandleInput()
{
	HandleBaseInput();

	if (Input::GetKeyPressed(Scancode::Space))
	{
		RotationStopwatch.Toggle();
	}

	if (Input::GetKeyPressed(Scancode::M))
	{
		UseHighPolyModel = !UseHighPolyModel;
		FPSCounter->ResetMinFrameTime();
	}
}
void SphereMapExample::Render()
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
void SphereMapExample::DrawHud()
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
				"M",
				"High Poly Mesh",
				UseHighPolyModel ? 1 : 0,
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

void SphereMapExample::LoadScene()
{
	Texture *texture = Texture::FromFile("Assets\\Textures\\Spheremap3.jpg");
	texture->Type = TextureType::SphereMap;

	Mesh = Mesh::Load("Assets\\Models\\StanfordDragon\\StanfordDragon.obj");
	Mesh->FitToBoundingBox(Box3f(3), true);
	Mesh->SetTexture(texture);
	Mesh->SetSpecular(50, 1.5f);

	MeshHighPoly = Mesh::Load("Assets\\Models\\StanfordDragon\\StanfordDragon-HighPoly.obj");
	MeshHighPoly->FitToBoundingBox(Box3f(3), true);
	MeshHighPoly->SetTexture(texture);
	MeshHighPoly->SetSpecular(50, 1.5f);
}
void SphereMapExample::DrawScene(::RenderStates &renderStates, WorkPartition workPartition)
{
	RenderUnit->DrawMesh(renderStates, workPartition, UseHighPolyModel ? *MeshHighPoly : *Mesh, Matrix4::RotateY(RotationStopwatch.ElapsedMilliseconds * .03f + 180));
}