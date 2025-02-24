#include "Example_MeshView.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	MeshViewExample example = MeshViewExample(1280, 720);
	example.Run();
	return TRUE;
}

MeshViewExample::MeshViewExample(int32 width, int32 height) : ExampleBase(width, height, "Meshes")
{
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(19, 21, 23);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 252, 245);

	LoadScene();

	Rotation = Matrix4f::Identity();
	Input::CenterMouse(*Window);
}

void MeshViewExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawStatisticsBox(10, 10);
		DrawFieldSet(250, 10, 250, 0, 20, "Move mouse", "move object", "Click right/left", "rotate X/Y", "Key 1-4", "choose mesh", nullptr);
		DrawFieldSet(510, 10, 0, 0, 20, "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);

		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();
	}
}

void MeshViewExample::HandleInput()
{
	HandleBaseInput();

	float speed = Math::Max(FPSCounter->LastFrameTime, 1) * .000005f;

	vfloat3 positionVelocity = vfloat3(
		(Input::GetMousePosition().X - Window->Width / 2) * 10.0f / Window->Width,
		(Window->Height / 2 - Input::GetMousePosition().Y) * 10.0f / Window->Width,
		Input::GetMousePosition().Z * .25f
	);

	RotationVelocity = (RotationVelocity + vfloat2(
		(float)Input::GetMouseDown(MouseButton::Right),
		(float)Input::GetMouseDown(MouseButton::Left)
	) * speed) * .95f;

	Position = Position * (1 - speed) + positionVelocity * speed;
	Rotation *= Matrix4f::RotateY(RotationVelocity.Y) * Matrix4f::RotateX(RotationVelocity.X);

	for (int32 i = 1; i <= sizeof(Meshes) / sizeof(Mesh*); i++)
	{
		if (Input::GetKeyPressed((Scancode)((int32)Scancode::D1 - 1 + i)) && CurrentMesh != i - 1)
		{
			CurrentMesh = i - 1;
			Rotation = Matrix4f::Identity();
		}
	}
}
void MeshViewExample::Render()
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

void MeshViewExample::LoadScene()
{
	Meshes[0] = Mesh::Load("Assets\\Models\\crate\\crate.obj");
	Meshes[0]->FitToBoundingBox(Box3f(2), true);

	Meshes[1] = Mesh::Load("Assets\\Models\\Tyre\\Tyre.obj");
	Meshes[1]->FitToBoundingBox(Box3f(2), true);
	Meshes[1]->TransformVertices(Matrix4f::RotateY(-90));
	Meshes[1]->NormalizeNormals();

	Meshes[2] = Mesh::Load("Assets\\Models\\half-life-headcrab\\half-life-headcrab.obj");
	Meshes[2]->FitToBoundingBox(Box3f(2), true);
	Meshes[2]->TransformVertices(Matrix4f::RotateY(180));

	Meshes[3] = Mesh::Load("Assets\\Models\\half-life-houndeye\\half-life-houndeye.obj");
	Meshes[3]->FitToBoundingBox(Box3f(2), true);
	Meshes[3]->TransformVertices(Matrix4f::RotateY(180));
}
void MeshViewExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(*Meshes[CurrentMesh], Rotation * Matrix4f::Translate(Position + vfloat3(0, 0, 4)));
}