#include "Example_ShadowMap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	ShadowMapExample example = ShadowMapExample(1280, 720);
	example.Run();
	return TRUE;
}

ShadowMapExample::ShadowMapExample(int32 width, int32 height) : ExampleBase(width, height, "Shadow Mapping")
{
	RenderStates->ShadowMap = RenderTarget(2048, 2048, _aligned_malloc(4096 * 4096 * 4, 32));
	RenderStates->ClipNear = .1f;
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(120, 130, 160);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Type = LightType::Point;
	RenderStates->Lights[0].Intensity = 4;
	RenderStates->Lights[0].Color = Color(255, 240, 180);
	RenderStates->ShadowMapFunc = ShadowMapFunc::Point;
	RenderStates->ShadowLightZoom = .25f;
	RenderStates->ShadowMapDepthBias = .03f;

	LoadScene();

	FreeLook->Speed = .1f;
	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}

void ShadowMapExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawPerformanceBox(10, 10, FreeLook->Position);
		DrawControlsBox(
			"Controls",
			10,
			-10,
			"Mouse",
			"Rotate",
			-1,
			"Space",
			"Freelook",
			IsFreeLook ? 1 : 0,
			nullptr);
		DrawControlsBox(
			"Render",
			-10,
			-10,
			"1 - 3",
			"Shadow Map Resolution",
			-1,
			"P",
			"PCF",
			RenderStates->ShadowMapFunc == ShadowMapFunc::Pcf ? 1 : 0,
			"T",
			"Texture Filtering",
			RenderStates->TextureFilteringEnable ? 1 : 0,
			"X",
			"Wireframe",
			Wireframe ? 1 : 0,
			nullptr);

		DrawShadowMapImage(Window->Width - 10 - 256, 10, 256, 256, .3f, 5);

		Window->Unlock();
		Window->Flip();

		Input::Update();
		FPSCounter->Frame();
	}
}

void ShadowMapExample::HandleInput()
{
	HandleBaseInput();

	if (IsFreeLook)
	{
		FreeLook->Update();
		Input::CenterMouse(*Window);
	}

	if (Input::GetKeyPressed(Scancode::D1))
	{
		RenderStates->ShadowMap = RenderTarget(1024, 1024, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .04f;
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderStates->ShadowMap = RenderTarget(2048, 2048, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .03f;
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderStates->ShadowMap = RenderTarget(4096, 4096, RenderStates->ShadowMap.Buffer);
		RenderStates->ShadowMapDepthBias = .01f;
	}

	if (Input::GetKeyPressed(Scancode::Space))
	{
		IsFreeLook = !IsFreeLook;
		FreeLook->Position = vfloat3(0, 1, -2.2f);
		FreeLook->Rotation = vfloat2(0, 20);
	}

	if (Input::GetKeyPressed(Scancode::P))
	{
		RenderStates->ShadowMapFunc = RenderStates->ShadowMapFunc == ShadowMapFunc::Pcf ? ShadowMapFunc::Point : ShadowMapFunc::Pcf;
	}
}
void ShadowMapExample::Render()
{
	int64 time = System::Milliseconds() + 5000;

	RenderUnit->Statistics.Clear();
	if (IsFreeLook) RenderUnit->ClearFrameBuffer(*RenderStates);
	RenderUnit->ClearDepthBuffer(*RenderStates);
	RenderUnit->ClearShadowMap(*RenderStates);

	RenderStates->Lights[0].Position = vfloat3(Math::Cos(time * .02f) * .7f, 1.4f, Math::Sin(time * .02f) * .7f);
	RenderStates->Lights[0].Rotation = vfloat3(270 - time * .02f, 80, 0);

	if (IsFreeLook)
	{
		RenderStates->ViewMatrix = FreeLook->ViewMatrix;
	}
	else
	{
		RenderStates->ViewMatrix =
			Matrix4f::RotateY((Input::GetMousePosition().X - Window->Width / 2) * 360.0f / Window->Width) *
			Matrix4f::Translate(0, -1, 2.5f) *
			Matrix4f::RotateX(-20);
	}

	// Animate Houndeye's eye ;)
	int32 houndEyeAnimationFrame = (time / 30) % 100;
	int32 houndEyeTextureIndex = houndEyeAnimationFrame >= 9 && houndEyeAnimationFrame <= 11 || houndEyeAnimationFrame >= 22 && houndEyeAnimationFrame <= 24
		? 2
		: houndEyeAnimationFrame >= 7 && houndEyeAnimationFrame <= 13 || houndEyeAnimationFrame >= 20 && houndEyeAnimationFrame <= 26
		? 1
		: 0;
	HoundEyeEye->Texture = HoundEyeEyeTexture[houndEyeTextureIndex];

	int32 threadIds[4];

	// Render shadow map.
	for (int32 i = 0; i < 4; i++)
	{
		threadIds[i] = ThreadPool::Start([this, i]
		{
			::RenderStates sceneRenderStates = *RenderStates;
			sceneRenderStates.SetWorkload(i, 4);
			sceneRenderStates.Rasterizer = Rasterizer::ShadowMap;
			sceneRenderStates.CountTotalTriangles = false;
			DrawScene(sceneRenderStates, 0);
		});
	}

	for (int32 i = 0; i < 4; i++)
	{
		ThreadPool::Join(threadIds[i]);
	}

	// Render scene with and project shadow map.
	for (int32 i = 0; i < 4; i++)
	{
		threadIds[i] = ThreadPool::Start([this, i]
		{
			::RenderStates sceneRenderStates = *RenderStates;
			sceneRenderStates.SetWorkload(i, 4);
			DrawScene(sceneRenderStates, 0);
			DrawScene(sceneRenderStates, 1);

			if (Wireframe)
			{
				sceneRenderStates.Rasterizer = Rasterizer::Wireframe;
				sceneRenderStates.CountTotalTriangles = false;
				sceneRenderStates.CountRenderedTriangles = false;
				DrawScene(sceneRenderStates, 0);
				DrawScene(sceneRenderStates, 1);
			}
		});
	}

	for (int32 i = 0; i < 4; i++)
	{
		ThreadPool::Join(threadIds[i]);
	}
}

void ShadowMapExample::LoadScene()
{
	Floor = PrimitiveFactory::Plane(10);
	Floor->SetTexture(Texture::FromFile("Assets\\Textures\\hlmp_30.png"));
	Floor->SetTextureSize(.2f, .2f);

	Ceiling = PrimitiveFactory::Plane(10);
	Ceiling->SetTexture(Texture::FromFile("Assets\\Textures\\c0a0_material_33.png"));
	Ceiling->SetTextureSize(.1f, .1f);

	Wall = PrimitiveFactory::Plane();
	Wall->SetTexture(Texture::FromFile("Assets\\Textures\\hlmp_8.png"));
	Wall->SetTextureSize(.333f, 1);

	Door = PrimitiveFactory::Cube();
	Door->SetTexture(Texture::FromFile("Assets\\Textures\\hlmp_111.png"));

	Fence = PrimitiveFactory::Cube(1, 2, 8);
	Fence->SetTexture(Texture::FromFile("Assets\\Textures\\c0a0_material_46.png"));
	Fence->GetSurface(0)->TextureSize = vfloat2(1, 10);
	Fence->GetSurface(2)->TextureSize = vfloat2(.198f, 1);
	Fence->GetSurface(3)->TextureSize = vfloat2(.198f, 1);
	Fence->GetSurface(4)->TextureSize = vfloat2(1, 10);
	Fence->GetSurface(5)->TextureSize = vfloat2(1, 10);
	Fence->SetSpecular(20, .6f);

	Cage = PrimitiveFactory::Cube(4);
	Cage->SetCullMode(CullMode::None);
	Cage->SetTexture(Texture::FromFile("Assets\\Textures\\grid1.png"));
	Cage->GetSurface(0)->Texture = Texture::FromFile("Assets\\Textures\\grid2.png");
	Cage->GetSurface(0)->TextureSize = vfloat2(.25f, .25f);
	Cage->SetSpecular(40, .4f);

	CageGround = PrimitiveFactory::Cube();
	for (int32 i = 0; i <= 1; i++)
	{
		CageGround->GetSurface(i)->Texture = Texture::FromFile("Assets\\Textures\\c1a0a_material_17.png");
	}
	for (int32 i = 2; i <= 5; i++)
	{
		CageGround->GetSurface(i)->Texture = Texture::FromFile("Assets\\Textures\\crete2_flr03c.png");
		CageGround->GetSurface(i)->TextureSize = vfloat2(.3f, 1);
	}

	Crate = PrimitiveFactory::Cube();
	Crate->SetTexture(Texture::FromFile("Assets\\Textures\\c1a2_material_93.png"));

	Crate2 = PrimitiveFactory::Cube();
	Crate2->SetTexture(Texture::FromFile("Assets\\Textures\\crate02b.png"));

	Scientist1 = Mesh::Load("Assets\\Models\\half-life-scientist-einstein\\einstein.obj");
	Scientist1->FitToBoundingBox(Box3f(1), true);
	Scientist2 = Mesh::Load("Assets\\Models\\half-life-scientist-luther\\luther.obj");
	Scientist2->FitToBoundingBox(Box3f(1), true);
	Scientist3 = Mesh::Load("Assets\\Models\\half-life-scientist-walter\\walter.obj");
	Scientist3->FitToBoundingBox(Box3f(1), true);
	HoundEye = Mesh::Load("Assets\\Models\\half-life-houndeye\\half-life-houndeye.obj");
	HoundEye->FitToBoundingBox(Box3f(1), true);
	HoundEye->TransformVertices(Matrix4f::RotateY(180));
	HoundEye->SetSpecular(0, 0);
	HoundEyeEye = HoundEye->GetSurface("eye");

	HoundEyeEyeTexture[0] = Texture::FromFile("Assets\\Models\\half-life-houndeye\\eye.png");
	HoundEyeEyeTexture[1] = Texture::FromFile("Assets\\Models\\half-life-houndeye\\eye1.png");
	HoundEyeEyeTexture[2] = Texture::FromFile("Assets\\Models\\half-life-houndeye\\eye2.png");

	LightBulb = PrimitiveFactory::Plane(1, 1);
	LightBulb->SetTexture(Texture::FromFile("Assets\\Textures\\LightSprite.png"));
	LightBulb->GetSurface(0)->BlendMode = BlendMode::Add;
	LightBulb->GetSurface(0)->Alpha = .75f;
}
void ShadowMapExample::DrawScene(::RenderStates &renderStates, int32 part)
{
	switch (part)
	{
		case 0:
		{
			RenderUnit->DrawMesh(renderStates, *Floor, Matrix4f::Scale(5, 0, 5));
			RenderUnit->DrawMesh(renderStates, *Ceiling, Matrix4f::Scale(5, 0, 5) * Matrix4f::RotateX(180) * Matrix4f::Translate(0, 1.5f, 0));
			RenderUnit->DrawMesh(renderStates, *Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateY(180) * Matrix4f::RotateZ(90) * Matrix4f::Translate(2.5f, .75f, 0));
			RenderUnit->DrawMesh(renderStates, *Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(-2.5f, .75f, 0));
			RenderUnit->DrawMesh(renderStates, *Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateX(-90) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(0, .75f, 2.5f));
			RenderUnit->DrawMesh(renderStates, *Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateX(90) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(0, .75f, -2.5f));
			RenderUnit->DrawMesh(renderStates, *Door, Matrix4f::Scale(.7f, 1.2f, .1f) * Matrix4f::Translate(-1.7f, .6f, 2.5f));

			Matrix4f crateMatrix = Matrix4f::Translate(.4f, 0, 1.6f);
			RenderUnit->DrawMesh(renderStates, *Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(5) * Matrix4f::Translate(.4f, .15f, 0) * crateMatrix);
			RenderUnit->DrawMesh(renderStates, *Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(-10) * Matrix4f::Translate(0, .15f, 0) * crateMatrix);
			RenderUnit->DrawMesh(renderStates, *Crate, Matrix4f::Scale(.3f) * Matrix4f::RotateY(8) * Matrix4f::Translate(.2f, .45f, 0) * crateMatrix);

			RenderUnit->DrawMesh(renderStates, *Crate2, Matrix4f::Scale(.3f) * Matrix4f::RotateY(15) * Matrix4f::Translate(-1.4f, .15f, -.3f));

			RenderUnit->DrawMesh(renderStates, *Scientist1, Matrix4f::RotateX(-90) * Matrix4f::RotateY(180) * Matrix4f::Translate(-.7f, .5f, .7f));
			RenderUnit->DrawMesh(renderStates, *Scientist2, Matrix4f::RotateX(-90) * Matrix4f::RotateY(180) * Matrix4f::Translate(0, .5f, .7f));
			RenderUnit->DrawMesh(renderStates, *Scientist3, Matrix4f::RotateX(-90) * Matrix4f::RotateY(180) * Matrix4f::Translate(.7f, .5f, .7f));

			Matrix4f cageMatrix = Matrix4f::Translate(1.2f, .025f, -.7f);
			RenderUnit->DrawMesh(renderStates, *CageGround, Matrix4f::Scale(.7f, .05f, .7f) * cageMatrix);
			RenderUnit->DrawMesh(renderStates, *HoundEye, Matrix4f::Scale(.5f) * Matrix4f::Translate(0, .25f, 0) * cageMatrix);

			// Draw textures that have a transparency key last.
			RenderUnit->DrawMesh(renderStates, *Cage, Matrix4f::Scale(.69f, .5f, .69f) * Matrix4f::Translate(0, .27f, 0) * cageMatrix);
			RenderUnit->DrawMesh(renderStates, *Fence, Matrix4f::Scale(.05f, .5f, 3) * Matrix4f::Translate(-.975f, .25f, 1));
			break;
		}
		case 1:
		{
			RenderUnit->DrawMesh(renderStates, *LightBulb, Matrix4f::Scale(.05f) * Matrix4f::RotateX(-90) * renderStates.ViewMatrix.RotationPart.Transpose() * Matrix4f::Translate(renderStates.Lights[0].Position));
			break;
		}
	}
}