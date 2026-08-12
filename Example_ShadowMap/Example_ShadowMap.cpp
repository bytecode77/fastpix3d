#include "Example_ShadowMap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	ShadowMapExample example = ShadowMapExample(1280, 720);
	example.Run();
	return TRUE;
}

ShadowMapExample::ShadowMapExample(int32 width, int32 height) : ExampleBase(width, height, "Shadow Mapping")
{
	RenderStates->ShadowMap = RenderTarget(2048, 2048, ShadowMap->Data);
	RenderStates->ClipNear = .1f;
	RenderStates->LightsEnable = true;
	RenderStates->AmbientLight = Color(120, 130, 160);
	RenderStates->Lights[0].Enabled = true;
	RenderStates->Lights[0].Type = LightType::Point;
	RenderStates->Lights[0].Intensity = 4;
	RenderStates->Lights[0].Color = Color(255, 240, 180);
	RenderStates->ShadowMapFunc = ShadowMapFunc::Point;
	RenderStates->ShadowLightZoom = .25f;
	RenderStates->ShadowMapDepthBias = 1.02f;

	LoadScene();

	FreeLook->Speed = .1f;
	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}

void ShadowMapExample::Run()
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
		RenderStates->ShadowMap = RenderTarget(1024, 1024, ShadowMap->Data);
		RenderStates->ShadowMapDepthBias = 1.02f;
		FPSCounter->ResetMinFrameTime();
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderStates->ShadowMap = RenderTarget(2048, 2048, ShadowMap->Data);
		RenderStates->ShadowMapDepthBias = 1.02f;
		FPSCounter->ResetMinFrameTime();
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderStates->ShadowMap = RenderTarget(4096, 4096, ShadowMap->Data);
		RenderStates->ShadowMapDepthBias = 1.01f;
		FPSCounter->ResetMinFrameTime();
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
		FPSCounter->ResetMinFrameTime();
	}
}
void ShadowMapExample::Render()
{
	RenderUnit->Statistics.Clear();

	ThreadPool::Run({
		[&]
		{
			if (IsFreeLook)
			{
				RenderUnit->ClearFrameBuffer(*RenderStates);
			}
		},
		[&] { RenderUnit->ClearDepthBuffer(*RenderStates); },
		[&] { RenderUnit->ClearShadowMap(*RenderStates); }
		});

	int64 time = System::Milliseconds() + 5000;
	RenderStates->Lights[0].Position = vfloat3(Math::Cos(time * .02f) * .7f, 1.4f, Math::Sin(time * .02f) * .7f);
	RenderStates->Lights[0].Rotation = vfloat3(270 - time * .02f, 80, 0);

	if (IsFreeLook)
	{
		RenderStates->ViewMatrix = FreeLook->ViewMatrix;
	}
	else
	{
		RenderStates->ViewMatrix =
			Matrix4::RotateY((Input::MousePosition.X - Window->Width / 2) * 360.0f / Window->Width) *
			Matrix4::Translate(0, -1, 2.5f) *
			Matrix4::RotateX(-20);
	}

	// Animate Houndeye's eye ;)
	int32 houndEyeAnimationFrame = (time / 30) % 100;
	int32 houndEyeTextureIndex = houndEyeAnimationFrame >= 9 && houndEyeAnimationFrame <= 11 || houndEyeAnimationFrame >= 22 && houndEyeAnimationFrame <= 24
		? 2
		: houndEyeAnimationFrame >= 7 && houndEyeAnimationFrame <= 13 || houndEyeAnimationFrame >= 20 && houndEyeAnimationFrame <= 26
		? 1
		: 0;
	HoundEyeEye->Texture = HoundEyeEyeTexture[houndEyeTextureIndex];

	// Render shadow map.
	ThreadPool::Run(4, [this](WorkPartition workPartition)
	{
		::RenderStates sceneRenderStates = *RenderStates;
		sceneRenderStates.Rasterizer = Rasterizer::ShadowMap;
		DrawScene(sceneRenderStates, workPartition, 0);
	});

	// Render scene and project shadow map.
	ThreadPool::Run(4, [this](WorkPartition workPartition)
	{
		::RenderStates sceneRenderStates = *RenderStates;
		DrawScene(sceneRenderStates, workPartition, 0);
		DrawScene(sceneRenderStates, workPartition, 1);

		if (Wireframe)
		{
			sceneRenderStates.Rasterizer = Rasterizer::Wireframe;
			DrawScene(sceneRenderStates, workPartition, 0);
			DrawScene(sceneRenderStates, workPartition, 1);
		}
	});
}
void ShadowMapExample::DrawHud()
{
	ThreadPool::Run({
		[&] { DrawPerformanceBox(10, 10, FreeLook->Position); },
		[&]
		{
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
		},
		[&]
		{
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
		},
		[&] { DrawShadowMapImage(Window->Width - 10 - 256, 10, 256, 256, .3f, 5); }
		});
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
	Fence->Surfaces[0]->TextureSize = vfloat2(1, 10);
	Fence->Surfaces[2]->TextureSize = vfloat2(.198f, 1);
	Fence->Surfaces[3]->TextureSize = vfloat2(.198f, 1);
	Fence->Surfaces[4]->TextureSize = vfloat2(1, 10);
	Fence->Surfaces[5]->TextureSize = vfloat2(1, 10);
	Fence->SetSpecular(20, .6f);

	Cage = PrimitiveFactory::Cube(4);
	Cage->SetCullMode(CullMode::None);
	Cage->SetTexture(Texture::FromFile("Assets\\Textures\\grid1.png"));
	Cage->Surfaces[0]->Texture = Texture::FromFile("Assets\\Textures\\grid2.png");
	Cage->Surfaces[0]->TextureSize = vfloat2(.25f, .25f);
	Cage->SetSpecular(40, .4f);

	CageGround = PrimitiveFactory::Cube();
	for (int32 i = 0; i <= 1; i++)
	{
		CageGround->Surfaces[i]->Texture = Texture::FromFile("Assets\\Textures\\c1a0a_material_17.png");
	}
	for (int32 i = 2; i <= 5; i++)
	{
		CageGround->Surfaces[i]->Texture = Texture::FromFile("Assets\\Textures\\crete2_flr03c.png");
		CageGround->Surfaces[i]->TextureSize = vfloat2(.3f, 1);
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
	HoundEye->TransformVertices(Matrix4::RotateY(180));
	HoundEye->SetSpecular(0, 0);
	HoundEyeEye = HoundEye->FindSurface("eye");

	HoundEyeEyeTexture[0] = Texture::FromFile("Assets\\Models\\half-life-houndeye\\eye.png");
	HoundEyeEyeTexture[1] = Texture::FromFile("Assets\\Models\\half-life-houndeye\\eye1.png");
	HoundEyeEyeTexture[2] = Texture::FromFile("Assets\\Models\\half-life-houndeye\\eye2.png");

	LightBulb = PrimitiveFactory::Plane(1, 1);
	LightBulb->SetTexture(Texture::FromFile("Assets\\Textures\\LightSprite.png"));
	LightBulb->Surfaces[0]->BlendMode = BlendMode::Add;
	LightBulb->Surfaces[0]->Alpha = .75f;
}
void ShadowMapExample::DrawScene(::RenderStates &renderStates, WorkPartition workPartition, int32 part)
{
	switch (part)
	{
		case 0:
		{
			RenderUnit->DrawMesh(renderStates, workPartition, *Floor, Matrix4::Scale(5, 0, 5));
			RenderUnit->DrawMesh(renderStates, workPartition, *Ceiling, Matrix4::Scale(5, 0, 5) * Matrix4::RotateX(180) * Matrix4::Translate(0, 1.5f, 0));
			RenderUnit->DrawMesh(renderStates, workPartition, *Wall, Matrix4::Scale(1.5f, 0, 5) * Matrix4::RotateY(180) * Matrix4::RotateZ(90) * Matrix4::Translate(2.5f, .75f, 0));
			RenderUnit->DrawMesh(renderStates, workPartition, *Wall, Matrix4::Scale(1.5f, 0, 5) * Matrix4::RotateZ(-90) * Matrix4::Translate(-2.5f, .75f, 0));
			RenderUnit->DrawMesh(renderStates, workPartition, *Wall, Matrix4::Scale(1.5f, 0, 5) * Matrix4::RotateX(-90) * Matrix4::RotateZ(-90) * Matrix4::Translate(0, .75f, 2.5f));
			RenderUnit->DrawMesh(renderStates, workPartition, *Wall, Matrix4::Scale(1.5f, 0, 5) * Matrix4::RotateX(90) * Matrix4::RotateZ(-90) * Matrix4::Translate(0, .75f, -2.5f));
			RenderUnit->DrawMesh(renderStates, workPartition, *Door, Matrix4::Scale(.7f, 1.2f, .1f) * Matrix4::Translate(-1.7f, .6f, 2.5f));

			Matrix4 crateMatrix = Matrix4::Translate(.4f, 0, 1.6f);
			RenderUnit->DrawMesh(renderStates, workPartition, *Crate, Matrix4::Scale(.3f) * Matrix4::RotateY(5) * Matrix4::Translate(.4f, .15f, 0) * crateMatrix);
			RenderUnit->DrawMesh(renderStates, workPartition, *Crate, Matrix4::Scale(.3f) * Matrix4::RotateY(-10) * Matrix4::Translate(0, .15f, 0) * crateMatrix);
			RenderUnit->DrawMesh(renderStates, workPartition, *Crate, Matrix4::Scale(.3f) * Matrix4::RotateY(8) * Matrix4::Translate(.2f, .45f, 0) * crateMatrix);

			RenderUnit->DrawMesh(renderStates, workPartition, *Crate2, Matrix4::Scale(.3f) * Matrix4::RotateY(15) * Matrix4::Translate(-1.4f, .15f, -.3f));

			RenderUnit->DrawMesh(renderStates, workPartition, *Scientist1, Matrix4::RotateX(-90) * Matrix4::RotateY(180) * Matrix4::Translate(-.7f, .5f, .7f));
			RenderUnit->DrawMesh(renderStates, workPartition, *Scientist2, Matrix4::RotateX(-90) * Matrix4::RotateY(180) * Matrix4::Translate(0, .5f, .7f));
			RenderUnit->DrawMesh(renderStates, workPartition, *Scientist3, Matrix4::RotateX(-90) * Matrix4::RotateY(180) * Matrix4::Translate(.7f, .5f, .7f));

			Matrix4 cageMatrix = Matrix4::Translate(1.2f, .025f, -.7f);
			RenderUnit->DrawMesh(renderStates, workPartition, *CageGround, Matrix4::Scale(.7f, .05f, .7f) * cageMatrix);
			RenderUnit->DrawMesh(renderStates, workPartition, *HoundEye, Matrix4::Scale(.5f) * Matrix4::Translate(0, .25f, 0) * cageMatrix);

			// Draw textures that have a transparency key last.
			RenderUnit->DrawMesh(renderStates, workPartition, *Cage, Matrix4::Scale(.69f, .5f, .69f) * Matrix4::Translate(0, .27f, 0) * cageMatrix);
			RenderUnit->DrawMesh(renderStates, workPartition, *Fence, Matrix4::Scale(.05f, .5f, 3) * Matrix4::Translate(-.975f, .25f, 1));
			break;
		}
		case 1:
		{
			RenderUnit->DrawMesh(renderStates, workPartition, *LightBulb, Matrix4::Scale(.05f) * Matrix4::RotateX(-90) * renderStates.ViewMatrix.RotationPart.Transpose() * Matrix4::Translate(renderStates.Lights[0].Position));
			break;
		}
	}
}