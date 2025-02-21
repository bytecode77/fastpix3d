#include "SphereMapExample.h"

SphereMapExample::SphereMapExample(int32 width, int32 height) : Example(width, height, "Sphere Mapping")
{
	RenderUnit->RenderStates.CameraSpace = Matrix4f::Translate(0, 0, 3);
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(20, 20, 20);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Rotation = Vector3f(-30, 20, 0);

	for (int32 i = 0; i < 4; i++)
	{
		Threads[i] = new Thread([](void *sender, void *parameter)
		{
			((SphereMapExample*)sender)->ThreadFunc((int32)parameter);
		}, this, (void*)i);
	}
}
void SphereMapExample::Run()
{
	LoadScene();

	while (!Input::HasExited() && !Input::GetKeyDown(Scancode::Escape))
	{
		Window->Lock();
		RenderUnit->ClearFrameBuffer();
		RenderUnit->ClearDepthBuffer();
		RenderUnit->Statistics.Clear();

		for (int32 i = 0; i < 4; i++) Threads[i]->Start();
		for (int32 i = 0; i < 4; i++) Threads[i]->Join();

		DrawStatisticsBox(10, 10);
		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();
	}
}

void SphereMapExample::LoadScene()
{
	Texture *texture = Texture::Load("Assets\\Textures\\Spheremap.jpg");
	texture->Type = TextureType::SphereMap;

	Mesh = Mesh::Load("Assets\\Models\\Teapot.obj");
	Mesh->SetTexture(texture);
}
void SphereMapExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(*Mesh, Matrix4f::RotateY(Math::Time() * .05f) * Matrix4f::RotateX(Math::Sin(Math::Time() * .05f) * 90 - 40));
}
void SphereMapExample::ThreadFunc(int32 threadNumber)
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
}