#include "MeshViewExample.h"

MeshViewExample::MeshViewExample(int32 width, int32 height) : Example(width, height, "Meshes")
{
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(19, 21, 23);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 252, 245);

	for (int32 i = 0; i < 2; i++)
	{
		Threads[i] = new Thread([](void *sender, void *parameter)
		{
			((MeshViewExample*)sender)->ThreadFunc((int32)parameter);
		}, this, (void*)i);
	}
}
void MeshViewExample::Run()
{
	LoadScene();

	Rotation = Matrix4f::Identity();
	Vector3f positionVelocity;
	Vector2f rotationVelocity;

	Input::CenterMouse(*Window);
	while (!Input::HasExited() && !Input::GetKeyDown(SDLK_ESCAPE))
	{
		Window->Lock();
		RenderUnit->ClearFrameBuffer(0, 100, 170);
		RenderUnit->ClearDepthBuffer();
		RenderUnit->Statistics.Clear();

		float speed = Math::Max(FPSCounter->FrameTime, 1) * .005f;

		positionVelocity = Vector3f(
			(Input::GetMousePosition().X - Window->Width / 2) * 10.0f / Window->Width,
			(Window->Height / 2 - Input::GetMousePosition().Y) * 10.0f / Window->Width,
			Input::GetMousePosition().Z * .25f
		);

		rotationVelocity = (rotationVelocity + Vector2f(
			Input::GetMouseDown(MouseButton::Right),
			Input::GetMouseDown(MouseButton::Left)
		) * speed) * .95f;

		Position = Position * (1 - speed) + positionVelocity * speed;
		Rotation *= Matrix4f::RotateY(rotationVelocity.Y) * Matrix4f::RotateX(rotationVelocity.X);

		for (int32 i = 0; i < 2; i++) Threads[i]->Start();
		for (int32 i = 0; i < 2; i++) Threads[i]->Join();

		DrawStatisticsBox(10, 10);
		DrawFieldSet(360, 10, 0, 0, 30, "Move mouse", "move object ", "Click right/left", "rotate X/Y", "Key 1-4", "choose mesh ", nullptr);
		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();

		for (int32 i = 1; i <= sizeof(Meshes) / sizeof(Mesh*); i++)
		{
			if (Input::GetKeyDown(SDLK_0 + i) && CurrentMesh != i - 1)
			{
				CurrentMesh = i - 1;
				Rotation = Matrix4f::Identity();
			}
		}
	}
}

void MeshViewExample::LoadScene()
{
	Meshes[0] = Mesh::Load("Assets\\Models\\crate\\crate.obj");
	Meshes[0]->FitToBoundingBox(Box3f(Vector3f(-1), Vector3f(1)), true);

	Meshes[1] = Mesh::Load("Assets\\Models\\OilDrum\\OilDrum.obj");
	Meshes[1]->FitToBoundingBox(Box3f(Vector3f(-1), Vector3f(1)), true);
	Meshes[1]->TransformVertices(Matrix4f::RotateY(-90));

	Meshes[2] = Mesh::Load("Assets\\Models\\Chest\\Chest.obj");
	Meshes[2]->FitToBoundingBox(Box3f(Vector3f(-1), Vector3f(1)), true);
	Meshes[2]->NormalizeNormals();

	Meshes[3] = Mesh::Load("Assets\\Models\\half-life-houndeye\\half-life-houndeye.obj");
	Meshes[3]->FitToBoundingBox(Box3f(Vector3f(-1), Vector3f(1)), true);
	Meshes[3]->TransformVertices(Matrix4f::RotateY(180));
}
void MeshViewExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(*Meshes[CurrentMesh], Rotation * Matrix4f::Translate(Position + Vector3f(0, 0, 4)));
}
void MeshViewExample::ThreadFunc(int32 threadNumber)
{
	::RenderUnit renderUnitCopy = *RenderUnit;
	renderUnitCopy.Statistics.Clear();

	renderUnitCopy.RenderStates.Workload = threadNumber == 0 ? Workload::Half1 : Workload::Half2;
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