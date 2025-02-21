#include "ShadowMapExample.h"

ShadowMapExample::ShadowMapExample(int32 width, int32 height) : Example(width, height, "Shadow Mapping")
{
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(120, 130, 160);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Type = LightType::Point;
	RenderUnit->RenderStates.Lights[0].Intensity = 4;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 240, 180);
	RenderUnit->RenderStates.ShadowLightZoom = .25f;
	RenderUnit->RenderStates.ShadowMapDepthBias = .05f;

	for (int32 i = 0; i < 8; i++)
	{
		Threads[i] = new Thread([](void *sender, void *parameter)
		{
			((ShadowMapExample*)sender)->ThreadFunc((int32)parameter);
		}, this, (void*)i);
	}
}
void ShadowMapExample::Run()
{
	LoadScene();

	bool isFreeLook = false;
	FreeLook->Speed = .1f;

	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
	while (!Input::HasExited() && !Input::GetKeyDown(Scancode::Escape))
	{
		int64 time = Math::Time() + 5000;

		Window->Lock();
		if (isFreeLook) RenderUnit->ClearFrameBuffer();
		RenderUnit->ClearDepthBuffer();
		RenderUnit->ClearStencilBuffer();
		RenderUnit->ClearShadowMap();
		RenderUnit->Statistics.Clear();

		RenderUnit->RenderStates.Lights[0].Position = Vector3f(Math::Cos(time * .02f) * .7f, 1.4f, Math::Sin(time * .02f) * .7f);
		RenderUnit->RenderStates.Lights[0].Rotation = Vector3f(270 - time * .02f, 80, 0);
		RenderUnit->RenderStates.SetShadowLightMatrixFromLight(0);

		if (isFreeLook)
		{
			RenderUnit->RenderStates.CameraSpace = FreeLook->CameraSpace;
		}
		else
		{
			RenderUnit->RenderStates.CameraSpace =
				Matrix4f::RotateY((Input::GetMousePosition().X - Window->Width / 2) * 360 / Window->Width) *
				Matrix4f::Translate(0, -1, 2.5f) *
				Matrix4f::RotateX(-20);
		}

		Threads[0]->Start(); // Fragments with ambient light only (1)
		Threads[1]->Start(); // (2)
		Threads[2]->Start(); // Shadow map (1)
		Threads[3]->Start(); // (2)
		Threads[0]->Join();
		Threads[1]->Join();
		Threads[2]->Join();
		Threads[3]->Join();

		Threads[4]->Start(); // Project shadow map and render with lights enabled
		Threads[5]->Start();
		Threads[6]->Start();
		Threads[7]->Start();
		Threads[4]->Join();
		Threads[5]->Join();
		Threads[6]->Join();
		Threads[7]->Join();

		// Render Light bulb
		RenderUnit->RenderStates.LightsEnable = false;
		RenderUnit->DrawMesh(*LightBulb, Matrix4f::Scale(.05f) * Matrix4f::Translate(RenderUnit->RenderStates.Lights[0].Position));
		RenderUnit->RenderStates.LightsEnable = true;

		DrawStatisticsBox(10, 10);
		DrawFieldSet(360, 10, 0, 0, 30, "Move mouse", "rotation", "Key 1-3", "Shadow map resolution ", "Space", "Switch to freelook", nullptr);
		DebugShadowMap(Window->Width - 10 - 256, 10, 256, 256, .3f, 5);
		Window->Unlock();
		Window->Flip();

		Input::Update();
		if (isFreeLook)
		{
			FreeLook->Update();
			Input::CenterMouse(*Window);
		}
		FPSCounter->Frame();

		if (Input::GetKeyDown(Scancode::D1)) RenderUnit->RenderStates.ShadowMap = RenderTarget(1024, 1024, ShadowMapBuffer);
		else if (Input::GetKeyDown(Scancode::D2)) RenderUnit->RenderStates.ShadowMap = RenderTarget(2048, 2048, ShadowMapBuffer);
		else if (Input::GetKeyDown(Scancode::D3)) RenderUnit->RenderStates.ShadowMap = RenderTarget(4096, 4096, ShadowMapBuffer);

		if (Input::GetKeyDown(Scancode::Space) && !isFreeLook)
		{
			isFreeLook = true;
			FreeLook->Position = Vector3f(0, 1, -2.2f);
			FreeLook->Rotation = Vector2f(0, 20);
		}
	}
}

void ShadowMapExample::LoadScene()
{
	Floor = PrimitiveFactory::Plane(10);
	Floor->SetTexture(Texture::Load("Assets\\Textures\\hlmp_30.png"));
	Floor->SetTextureSize(.2f, .2f);

	Ceiling = PrimitiveFactory::Plane(10);
	Ceiling->SetTexture(Texture::Load("Assets\\Textures\\hlmp_10.png"));
	Ceiling->SetTextureSize(.2f, .2f);

	Wall = PrimitiveFactory::Plane();
	Wall->SetTexture(Texture::Load("Assets\\Textures\\hlmp_8.png"));
	Wall->SetTextureSize(.333f, 1);

	Door = PrimitiveFactory::Cube();
	Door->SetTexture(Texture::Load("Assets\\Textures\\hlmp_111.png"));

	Fence = PrimitiveFactory::Cube();
	Fence->SetTexture(Texture::Load("Assets\\Textures\\c0a0_material_46.png"));
	Fence->GetSurface(0)->TextureSize = Vector2f(1, 10);
	Fence->GetSurface(2)->TextureSize = Vector2f(.198f, 1);
	Fence->GetSurface(3)->TextureSize = Vector2f(.198f, 1);
	Fence->GetSurface(4)->TextureSize = Vector2f(1, 10);
	Fence->GetSurface(5)->TextureSize = Vector2f(1, 10);

	Cage = PrimitiveFactory::Cube();
	Cage->SetCullMode(CullMode::None);
	Cage->SetTexture(Texture::Load("Assets\\Textures\\c1a0d_material_151.png"));
	Cage->SetTextureSize(.72f, .97f);

	CageGround = PrimitiveFactory::Cube();
	CageGround->SetTexture(Texture::Load("Assets\\Textures\\c1a0a_material_17.png"));
	CageGround->GetSurface(2)->TextureSize = Vector2f(1, 10);
	CageGround->GetSurface(3)->TextureSize = Vector2f(1, 10);
	CageGround->GetSurface(4)->TextureSize = Vector2f(1, 10);
	CageGround->GetSurface(5)->TextureSize = Vector2f(1, 10);

	Crate = PrimitiveFactory::Cube();
	Crate->SetTexture(Texture::Load("Assets\\Textures\\c1a2_material_93.png"));

	Scientist1 = Mesh::Load("Assets\\Models\\half-life-scientist-einstein\\einstein.obj");
	Scientist1->FitToBoundingBox(Box3f(Vector3f(-.5f), Vector3f(.5f)), true);
	Scientist2 = Mesh::Load("Assets\\Models\\half-life-scientist-luther\\luther.obj");
	Scientist2->FitToBoundingBox(Box3f(Vector3f(-.5f), Vector3f(.5f)), true);
	Scientist3 = Mesh::Load("Assets\\Models\\half-life-scientist-walter\\walter.obj");
	Scientist3->FitToBoundingBox(Box3f(Vector3f(-.5f), Vector3f(.5f)), true);
	HoundEye = Mesh::Load("Assets\\Models\\half-life-houndeye\\half-life-houndeye.obj");
	HoundEye->FitToBoundingBox(Box3f(Vector3f(-.5f), Vector3f(.5f)), true);
	HoundEye->TransformVertices(Matrix4f::RotateY(180));

	LightBulb = PrimitiveFactory::Sphere(12, 6);
	LightBulb->SetVertexColors(RenderUnit->RenderStates.Lights[0].Color);
}
void ShadowMapExample::DrawScene(::RenderUnit &renderUnit)
{
	renderUnit.DrawMesh(*Floor, Matrix4f::Scale(5, 0, 5));
	renderUnit.DrawMesh(*Ceiling, Matrix4f::Scale(5, 0, 5) * Matrix4f::RotateX(180) * Matrix4f::Translate(0, 1.5f, 0));
	renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateY(180) * Matrix4f::RotateZ(90) * Matrix4f::Translate(2.5f, .75f, 0));
	renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(-2.5f, .75f, 0));
	renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateX(-90) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(0, .75f, 2.5f));
	renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateX(90) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(0, .75f, -2.5f));
	renderUnit.DrawMesh(*Door, Matrix4f::Scale(.7f, 1.2f, .1f) * Matrix4f::Translate(-1.7f, .6f, 2.5f));
	renderUnit.DrawMesh(*Fence, Matrix4f::Scale(.05f, .5f, 3) * Matrix4f::Translate(-.975f, .25f, 1));

	Matrix4f crateMatrix = Matrix4f::Translate(.4f, 0, 1.6f);
	renderUnit.DrawMesh(*Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(5) * Matrix4f::Translate(.4f, .15f, 0) * crateMatrix);
	renderUnit.DrawMesh(*Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(-10) * Matrix4f::Translate(0, .15f, 0) * crateMatrix);
	renderUnit.DrawMesh(*Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(8) * Matrix4f::Translate(.2f, .45f, 0) * crateMatrix);

	renderUnit.DrawMesh(*Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(15) * Matrix4f::Translate(-1.4f, .15f, -.3f));

	renderUnit.DrawMesh(*Scientist1, Matrix4f::RotateX(-90) * Matrix4f::RotateY(180) * Matrix4f::Translate(-.7f, .5f, .7f));
	renderUnit.DrawMesh(*Scientist2, Matrix4f::RotateX(-90) * Matrix4f::RotateY(180) * Matrix4f::Translate(0, .5f, .7f));
	renderUnit.DrawMesh(*Scientist3, Matrix4f::RotateX(-90) * Matrix4f::RotateY(180) * Matrix4f::Translate(.7f, .5f, .7f));

	Matrix4f cageMatrix = Matrix4f::Translate(1.2f, 0, -.7f);
	renderUnit.DrawMesh(*CageGround, Matrix4f::Scale(.7f, .1f, .7f) * cageMatrix);
	renderUnit.DrawMesh(*Cage, Matrix4f::Scale(.7f, .5f, .7f) * Matrix4f::Translate(0, .29f, 0) * cageMatrix);
	renderUnit.DrawMesh(*HoundEye, Matrix4f::Scale(.5f) * Matrix4f::Translate(0, .25f, 0) * cageMatrix);
}
void ShadowMapExample::ThreadFunc(int32 threadNumber)
{
	::RenderUnit renderUnitCopy = *RenderUnit;
	renderUnitCopy.Statistics.Clear();

	switch (threadNumber)
	{
		case 0:
		case 1:
			renderUnitCopy.RenderStates.Workload = threadNumber == 0 ? Workload::Half1 : Workload::Half2;
			renderUnitCopy.RenderStates.Lights[0].Enabled = false;
			DrawScene(renderUnitCopy);
			break;
		case 2:
		case 3:
			renderUnitCopy.RenderStates.Workload = threadNumber == 2 ? Workload::Half1 : Workload::Half2;
			renderUnitCopy.RenderStates.RenderPass = RenderPass::ShadowMap;
			DrawScene(renderUnitCopy);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			switch (threadNumber)
			{
				case 4: renderUnitCopy.RenderStates.Workload = Workload::Quarter1; break;
				case 5: renderUnitCopy.RenderStates.Workload = Workload::Quarter2; break;
				case 6: renderUnitCopy.RenderStates.Workload = Workload::Quarter3; break;
				case 7: renderUnitCopy.RenderStates.Workload = Workload::Quarter4; break;
			}

			renderUnitCopy.RenderStates.RenderPass = RenderPass::ShadowMapStencil;
			DrawScene(renderUnitCopy);

			renderUnitCopy.RenderStates.RenderPass = RenderPass::Fragments;
			renderUnitCopy.RenderStates.StencilFunc = StencilFunc::Zero;
			renderUnitCopy.RenderStates.ZWriteEnable = false;
			DrawScene(renderUnitCopy);
			break;
	}

	if (threadNumber == 0)
	{
		RenderUnit->Statistics.TotalTriangleCount += renderUnitCopy.Statistics.TotalTriangleCount;
	}

	if (threadNumber == 0 || threadNumber == 2 || threadNumber == 4)
	{
		RenderUnit->Statistics.RenderedTriangleCount += renderUnitCopy.Statistics.RenderedTriangleCount;
	}
}