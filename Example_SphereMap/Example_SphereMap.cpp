#include "Example_SphereMap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	SphereMapExample example = SphereMapExample(1280, 720);
	example.Run();
	return TRUE;
}

SphereMapExample::SphereMapExample(int32 width, int32 height) : ExampleBase(width, height, "Sphere Mapping")
{
	RenderStates->ViewMatrix = Matrix4f::Translate(0, -1, 3) * Matrix4f::RotateX(-20);
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(40, 40, 40);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Intensity = .7f;

	LoadScene();

	RotationStopwatch = Stopwatch::StartNew();
}

void SphereMapExample::Run()
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
			"M",
			"High Poly Mesh",
			UseHighPolyModel ? 1 : 0,
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
	}
}
void SphereMapExample::Render()
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
void SphereMapExample::DrawScene(::RenderStates &renderStates)
{
	RenderUnit->DrawMesh(renderStates, UseHighPolyModel ? *MeshHighPoly : *Mesh, Matrix4f::RotateY(RotationStopwatch.ElapsedMilliseconds * .03f + 180));
}