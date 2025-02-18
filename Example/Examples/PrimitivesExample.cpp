#include "PrimitivesExample.h"

PrimitivesExample::PrimitivesExample(int32 width, int32 height) : Example(width, height, "Primitives")
{
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(40, 40, 40);
	RenderUnit->RenderStates.Lights[0].Enabled = true;

	for (int32 i = 0; i < 4; i++)
	{
		Threads[i] = new Thread([](void *sender, void *parameter)
		{
			((PrimitivesExample*)sender)->ThreadFunc((int32)parameter);
		}, this, (void*)i);
	}
}
void PrimitivesExample::Run()
{
	LoadScene();

	Input::CenterMouse(*Window);
	while (!Input::HasExited() && !Input::GetKeyDown(SDLK_ESCAPE))
	{
		Window->Lock();
		RenderUnit->ClearFrameBuffer(0, 100, 170);
		RenderUnit->ClearDepthBuffer();
		RenderUnit->Statistics.Clear();

		// Since none of the meshes intersect, this makes it an embarrassingly parallel problem,
		// by rendering each mesh in a seperate thread.
		for (int32 i = 0; i < 4; i++) Threads[i]->Start();
		for (int32 i = 0; i < 4; i++) Threads[i]->Join();

		DrawStatisticsBox(10, 10);
		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();
	}
}

void PrimitivesExample::LoadScene()
{
	Texture *texture = Texture::Load("Assets\\Textures\\Test64.png");
	Meshes[0] = PrimitiveFactory::Cube();
	Meshes[1] = PrimitiveFactory::Cylinder(16);
	Meshes[2] = PrimitiveFactory::Tube(16, .6f);
	Meshes[3] = PrimitiveFactory::Cone(16);
	Meshes[4] = PrimitiveFactory::Sphere(24, 12);
	Meshes[5] = PrimitiveFactory::Torus(24, 12, .25f);
	Meshes[6] = PrimitiveFactory::Tube(16, 1, .9f, 1, .2f);
	Meshes[7] = PrimitiveFactory::Cylinder(16, .5f, 1);

	for (int32 i = 0; i < 8; i++)
	{
		Meshes[i]->SetTexture(texture);
	}
}
void PrimitivesExample::DrawScene(::RenderUnit &renderUnit, int32 meshIndex)
{
	int32 x = meshIndex % 4;
	int32 y = meshIndex / 4;
	Matrix4f rotation = Matrix4f::RotateY(Window->Width / 2 - Input::GetMousePosition().X) * Matrix4f::RotateX(Window->Height / 2 - Input::GetMousePosition().Y);

	renderUnit.DrawMesh(*Meshes[meshIndex], rotation * Matrix4f::Translate((x - 1.5f) * 2, (y - .5f) * -2, 5));
}
void PrimitivesExample::ThreadFunc(int32 threadNumber)
{
	::RenderUnit renderUnitCopy = *RenderUnit;

	DrawScene(renderUnitCopy, threadNumber * 2);
	DrawScene(renderUnitCopy, threadNumber * 2 + 1);

	RenderUnit->Statistics.Merge(renderUnitCopy.Statistics);
}