#include "Example_SphereMap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	SphereMapExample example = SphereMapExample(1280, 720);
	example.Run();
	return TRUE;
}

SphereMapExample::SphereMapExample(int32 width, int32 height) : ExampleBase(width, height, "Sphere Mapping")
{
	RenderUnit->RenderStates.ViewMatrix = Matrix4f::Translate(0, -1, 3) * Matrix4f::RotateX(-20);
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(40, 40, 40);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Intensity = .7f;

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

		DrawStatisticsBox(10, 10);
		DrawFieldSet(250, 10, 200, 0, 20, "Space", RotationStopwatch.IsRunning ? "Animation ON " : "Animation OFF", nullptr);
		DrawFieldSet(460, 10, 0, 0, 20, "M", UseHighPolyModel ? "High poly model" : "Low poly model", "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);

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
void SphereMapExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(UseHighPolyModel ? *MeshHighPoly : *Mesh, Matrix4f::RotateY(RotationStopwatch.ElapsedMilliseconds * .03f + 180));
}