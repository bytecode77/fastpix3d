#include "Example_Primitives.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	PrimitivesExample example = PrimitivesExample(1280, 720);
	example.Run();
	return TRUE;
}

PrimitivesExample::PrimitivesExample(int32 width, int32 height) : ExampleBase(width, height, "Primitives")
{
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(40, 40, 40);
	RenderUnit->RenderStates.Lights[0].Enabled = true;

	LoadScene();

	Rotation = Matrix4f::Identity();
	Input::CenterMouse(*Window);
}

void PrimitivesExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawStatisticsBox(10, 10);
		DrawFieldSet(250, 10, 0, 0, 20, "Space", RenderUnit->RenderStates.TextureEnable ? "Textures are ON" : "Textures are OFF", "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);

		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();
	}
}

void PrimitivesExample::HandleInput()
{
	HandleBaseInput();

	float speed = Math::Max(FPSCounter->LastFrameTime, 1) * .00002f;

	RotationVelocity = (RotationVelocity - vfloat2(
		(float)Input::GetMouseSpeed().X,
		(float)Input::GetMouseSpeed().Y
	) * speed) * .95f;

	Rotation *=
		Matrix4f::RotateY(RotationVelocity.X) *
		Matrix4f::RotateX(RotationVelocity.Y);

	if (Input::GetKeyPressed(Scancode::Space))
	{
		RenderUnit->RenderStates.TextureEnable = !RenderUnit->RenderStates.TextureEnable;

		if (RenderUnit->RenderStates.TextureEnable)
		{
			for (int32 i = 0; i < 8; i++)
			{
				Meshes[i]->SetVertexColors(Color(255, 255, 255));
			}
		}
		else
		{
			Meshes[0]->SetVertexColors(8, 145, 179);
			Meshes[1]->SetVertexColors(100, 163, 14);
			Meshes[2]->SetVertexColors(250, 115, 23);
			Meshes[3]->SetVertexColors(95, 234, 213);
			Meshes[4]->SetVertexColors(16, 185, 130);
			Meshes[5]->SetVertexColors(186, 28, 29);
			Meshes[6]->SetVertexColors(31, 65, 175);
			Meshes[7]->SetVertexColors(253, 224, 71);
		}
	}
}
void PrimitivesExample::Render()
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

			// Since none of the meshes intersect, this makes it an embarrassingly parallel problem,
			// by rendering each mesh in a seperate thread.

			DrawScene(renderUnitCopy, i * 2);
			DrawScene(renderUnitCopy, i * 2 + 1);

			RenderUnit->Statistics.Merge(renderUnitCopy.Statistics);

			if (Wireframe)
			{
				renderUnitCopy.RenderStates.Rasterizer = Rasterizer::Wireframe;
				DrawScene(renderUnitCopy, i * 2);
				DrawScene(renderUnitCopy, i * 2 + 1);
			}
		});
	}

	for (int32 i = 0; i < 4; i++)
	{
		ThreadPool::Join(threadIds[i]);
	}
}

void PrimitivesExample::LoadScene()
{
	// Adjust segment count for all primitives:
	const int32 segmentFactor = 4;

	Meshes[0] = PrimitiveFactory::Cube();
	Meshes[1] = PrimitiveFactory::Cylinder(8 * segmentFactor);
	Meshes[2] = PrimitiveFactory::Tube(8 * segmentFactor, .6f);
	Meshes[3] = PrimitiveFactory::Cone(8 * segmentFactor);
	Meshes[4] = PrimitiveFactory::Sphere(12 * segmentFactor, 6 * segmentFactor);
	Meshes[5] = PrimitiveFactory::Torus(12 * segmentFactor, 6 * segmentFactor, .25f);
	Meshes[6] = PrimitiveFactory::Tube(8 * segmentFactor, 1, .9f, 1, .2f);
	Meshes[7] = PrimitiveFactory::Cylinder(8 * segmentFactor, .5f, 1);

	Texture *texture = Texture::FromFile("Assets\\Textures\\Test64.png");
	for (int32 i = 0; i < 8; i++)
	{
		Meshes[i]->SetTexture(texture);
		Meshes[i]->SetSpecular(20, .75f);
	}
}
void PrimitivesExample::DrawScene(::RenderUnit &renderUnit, int32 meshIndex)
{
	int32 x = meshIndex % 4;
	int32 y = meshIndex / 4;

	renderUnit.DrawMesh(*Meshes[meshIndex], Rotation * Matrix4f::Translate((x - 1.5f) * 1.7f, (y - .5f) * -1.7f, 3.8f));
}