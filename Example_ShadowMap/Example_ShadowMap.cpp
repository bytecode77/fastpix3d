#include "Example_ShadowMap.h"

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLine, _In_ int showCmd)
{
	ShadowMapExample example = ShadowMapExample(1280, 720);
	example.Run();
	return TRUE;
}

ShadowMapExample::ShadowMapExample(int32 width, int32 height) : ExampleBase(width, height, "Shadow Mapping")
{
	RenderUnit->RenderStates.ShadowMap = RenderTarget(2048, 2048, _aligned_malloc(4096 * 4096 * 4, 32));
	RenderUnit->RenderStates.ClipNear = .1f;
	RenderUnit->RenderStates.LightsEnable = true;
	RenderUnit->RenderStates.AmbientLight = Color(120, 130, 160);
	RenderUnit->RenderStates.Lights[0].Enabled = true;
	RenderUnit->RenderStates.Lights[0].Type = LightType::Point;
	RenderUnit->RenderStates.Lights[0].Intensity = 4;
	RenderUnit->RenderStates.Lights[0].Color = Color(255, 240, 180);
	RenderUnit->RenderStates.ShadowMapFunc = ShadowMapFunc::Point;
	RenderUnit->RenderStates.ShadowLightZoom = .25f;
	RenderUnit->RenderStates.ShadowMapDepthBias = .03f;

	LoadScene();

	FreeLook->Speed = .1f;
	Window->SetRelativeMouseMode(true);
	Input::CenterMouse(*Window);
}
ShadowMapExample::~ShadowMapExample()
{
	_aligned_free(RenderUnit->RenderStates.ShadowMap.Buffer);
}

void ShadowMapExample::Run()
{
	while (!Input::HasExited() && !Input::GetKeyPressed(Scancode::Escape))
	{
		Window->Lock();

		HandleInput();
		Render();

		DrawStatisticsBox(10, 10);
		DrawFieldSet(250, 10, 310, 0, 20, "Move mouse", "rotation", "Key 1-3", "Shadow map resolution", nullptr);
		DrawFieldSet(570, 10, 0, 0, 20, "Space", IsFreeLook ? "Freelook is ON" : "Freelook is OFF", "T", RenderUnit->RenderStates.TextureFilteringEnable ? "Texture filtering is ON" : "Texture filtering is OFF", "P", RenderUnit->RenderStates.ShadowMapFunc == ShadowMapFunc::Pcf ? "PCF is ON" : "PCF is OFF", "X", Wireframe ? "Wireframe is ON" : "Wireframe is OFF", nullptr);
		DebugShadowMap(Window->Width - 10 - 256, 10, 256, 256, .3f, 5);

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
		RenderUnit->RenderStates.ShadowMap = RenderTarget(1024, 1024, RenderUnit->RenderStates.ShadowMap.Buffer);
	}
	else if (Input::GetKeyPressed(Scancode::D2))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(2048, 2048, RenderUnit->RenderStates.ShadowMap.Buffer);
	}
	else if (Input::GetKeyPressed(Scancode::D3))
	{
		RenderUnit->RenderStates.ShadowMap = RenderTarget(4096, 4096, RenderUnit->RenderStates.ShadowMap.Buffer);
	}

	if (Input::GetKeyPressed(Scancode::Space))
	{
		IsFreeLook = !IsFreeLook;
		FreeLook->Position = vfloat3(0, 1, -2.2f);
		FreeLook->Rotation = vfloat2(0, 20);
	}

	if (Input::GetKeyPressed(Scancode::P))
	{
		RenderUnit->RenderStates.ShadowMapFunc = RenderUnit->RenderStates.ShadowMapFunc == ShadowMapFunc::Pcf ? ShadowMapFunc::Point : ShadowMapFunc::Pcf;
	}
}
void ShadowMapExample::Render()
{
	int64 time = System::Milliseconds() + 5000;

	if (IsFreeLook) RenderUnit->ClearFrameBuffer();
	RenderUnit->ClearDepthBuffer();
	RenderUnit->ClearShadowMap();
	RenderUnit->Statistics.Clear();

	RenderUnit->RenderStates.Lights[0].Position = vfloat3(Math::Cos(time * .02f) * .7f, 1.4f, Math::Sin(time * .02f) * .7f);
	RenderUnit->RenderStates.Lights[0].Rotation = vfloat3(270 - time * .02f, 80, 0);

	if (IsFreeLook)
	{
		RenderUnit->RenderStates.ViewMatrix = FreeLook->ViewMatrix;
	}
	else
	{
		RenderUnit->RenderStates.ViewMatrix =
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
			::RenderUnit renderUnitCopy = *RenderUnit;
			renderUnitCopy.Statistics.Clear();

			renderUnitCopy.RenderStates.SetWorkload(i, 4);
			renderUnitCopy.RenderStates.Rasterizer = Rasterizer::ShadowMap;
			DrawScene(renderUnitCopy, 0);

			RenderUnit->Statistics.Merge(renderUnitCopy.Statistics, false, true);
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
			::RenderUnit renderUnitCopy = *RenderUnit;
			renderUnitCopy.Statistics.Clear();

			renderUnitCopy.RenderStates.SetWorkload(i, 4);
			DrawScene(renderUnitCopy, 0);

			renderUnitCopy.RenderStates.ShadowMapFunc = ShadowMapFunc::None;
			DrawScene(renderUnitCopy, 1);

			RenderUnit->Statistics.Merge(renderUnitCopy.Statistics, true, true);

			if (Wireframe)
			{
				renderUnitCopy.RenderStates.Rasterizer = Rasterizer::Wireframe;
				DrawScene(renderUnitCopy, 0);
				DrawScene(renderUnitCopy, 1);
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
	Cage->SetTexture(Texture::FromFile("Assets\\Textures\\c1a0a_material_75.png"));
	Cage->SetTextureSize(.72f, .97f);
	Cage->SetSpecular(40, .4f);

	CageGround = PrimitiveFactory::Cube();
	CageGround->SetTexture(Texture::FromFile("Assets\\Textures\\c1a0a_material_17.png"));
	CageGround->GetSurface(2)->TextureSize = vfloat2(1, 10);
	CageGround->GetSurface(3)->TextureSize = vfloat2(1, 10);
	CageGround->GetSurface(4)->TextureSize = vfloat2(1, 10);
	CageGround->GetSurface(5)->TextureSize = vfloat2(1, 10);

	Crate = PrimitiveFactory::Cube();
	Crate->SetTexture(Texture::FromFile("Assets\\Textures\\c1a2_material_93.png"));

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
void ShadowMapExample::DrawScene(::RenderUnit &renderUnit, int32 part)
{
	switch (part)
	{
		case 0:
		{
			renderUnit.DrawMesh(*Floor, Matrix4f::Scale(5, 0, 5));
			renderUnit.DrawMesh(*Ceiling, Matrix4f::Scale(5, 0, 5) * Matrix4f::RotateX(180) * Matrix4f::Translate(0, 1.5f, 0));
			renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateY(180) * Matrix4f::RotateZ(90) * Matrix4f::Translate(2.5f, .75f, 0));
			renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(-2.5f, .75f, 0));
			renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateX(-90) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(0, .75f, 2.5f));
			renderUnit.DrawMesh(*Wall, Matrix4f::Scale(1.5f, 0, 5) * Matrix4f::RotateX(90) * Matrix4f::RotateZ(-90) * Matrix4f::Translate(0, .75f, -2.5f));
			renderUnit.DrawMesh(*Door, Matrix4f::Scale(.7f, 1.2f, .1f) * Matrix4f::Translate(-1.7f, .6f, 2.5f));

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
			renderUnit.DrawMesh(*HoundEye, Matrix4f::Scale(.5f) * Matrix4f::Translate(0, .25f, 0) * cageMatrix);

			// Draw textures that have a transparency key last.
			renderUnit.DrawMesh(*Cage, Matrix4f::Scale(.69f, .5f, .69f) * Matrix4f::Translate(0, .29f, 0) * cageMatrix);
			renderUnit.DrawMesh(*Fence, Matrix4f::Scale(.05f, .5f, 3) * Matrix4f::Translate(-.975f, .25f, 1));
			break;
		}
		case 1:
		{
			renderUnit.DrawMesh(*LightBulb, Matrix4f::Scale(.05f) * Matrix4f::RotateX(-90) * renderUnit.RenderStates.ViewMatrix.RotationPart.Transpose() * Matrix4f::Translate(renderUnit.RenderStates.Lights[0].Position));
			break;
		}
	}
}
void ShadowMapExample::DebugShadowMap(int32 x, int32 y, int32 width, int32 height, float zFrom, float zTo) const
{
	width = Math::Min(width, RenderUnit->RenderStates.FrameBuffer.Width - 1);
	height = Math::Min(height, RenderUnit->RenderStates.FrameBuffer.Height - 1);
	zFrom = RenderUnit->RenderStates.ClipNear / zFrom;
	zTo = RenderUnit->RenderStates.ClipNear / zTo;

	int32 scale = 0;
	while (RenderUnit->RenderStates.ShadowMap.Width >> scale > width || RenderUnit->RenderStates.ShadowMap.Height >> scale > height)
	{
		scale++;
	}

	int32 renderWidth = RenderUnit->RenderStates.ShadowMap.Width >> scale;
	int32 renderHeight = RenderUnit->RenderStates.ShadowMap.Height >> scale;

	Color *frameBuffer = RenderUnit->RenderStates.FrameBuffer.GetBuffer<Color>(x + y * RenderUnit->RenderStates.FrameBuffer.Width);
	float *shadowMap = RenderUnit->RenderStates.ShadowMap.GetBuffer<float>();

	int32 frameBufferStrideY = RenderUnit->RenderStates.FrameBuffer.Width - renderWidth;
	int32 shadowMapStrideX = 1 << scale;
	int32 shadowMapStrideY = RenderUnit->RenderStates.ShadowMap.Width * (shadowMapStrideX - 1);

	for (int32 py = 0; py < renderHeight; py++)
	{
		for (int32 px = 0; px < renderWidth; px++)
		{
			if (*shadowMap > 0)
			{
				int32 color = (int32)Math::Interpolate(*shadowMap, zFrom, zTo, 255.0f, 0.0f) & 0xff;
				frameBuffer->B = color;
				frameBuffer->G = color;
				frameBuffer->R = color;
			}
			else
			{
				frameBuffer->B >>= 1;
				frameBuffer->G >>= 1;
				frameBuffer->R >>= 1;
			}

			frameBuffer++;
			shadowMap += shadowMapStrideX;
		}

		frameBuffer += frameBufferStrideY;
		shadowMap += shadowMapStrideY;
	}

	char title[100];
	char buffer[100];
	lstrcpyA(title, "Shadow Map ");
	lstrcatA(title, _itoa(RenderUnit->RenderStates.ShadowMap.Width, buffer, 10));
	lstrcatA(title, "x");
	lstrcatA(title, _itoa(RenderUnit->RenderStates.ShadowMap.Height, buffer, 10));

	Graphics g = Graphics(*Window);
	g.Font = Font;
	g.DrawString(x + 10, y + 10, title);
}