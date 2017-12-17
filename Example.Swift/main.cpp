#pragma comment(lib, "ExampleLibrary.lib")
#include <ExampleLibrary.h>

int main(int argc, char *argv[])
{
	Device::Initialize(Examples::DefaultLargeWidth, Examples::DefaultLargeHeight);
	RenderStates::ClipNear = 3;
	RenderStates::Zoom = 1.5f;
	Light::AmbientColor = Color(80, 80, 130);

	FPSCounter *fpsCounter = new FPSCounter(500);

	Font *font = new Font("Media\\Fonts\\16x16.png");
	Mesh *scene = Mesh::FromFile("Media\\Scene\\Swift.fp3d");
	Mesh *beethoven = Mesh::FromFile("Media\\Scene\\Beethoven.fp3d");

	Matrix sceneMatrix = Matrix::Identity();

	ShadowVolume *sceneShadow = new ShadowVolume(scene);
	ShadowVolume *beethovenShadow = new ShadowVolume(beethoven);

	RenderStates::Lights[0]->Enabled = true;
	RenderStates::Lights[0]->Type = LightType::Point;
	RenderStates::Lights[0]->Intensity = 20;
	RenderStates::Lights[0]->DiffuseColor = Color(255, 255, 240);

	Input::Clear();
	while (!Input::getQuit() && !Input::getKeyDown(SDLK_ESCAPE))
	{
		float time = SDL_GetTicks() * .1f;
		Device::ClearBackBuffer();
		Device::ClearDepthBuffer();
		Device::ClearStencilBuffer();

		RenderStates::CameraSpace = Matrix::RotateY(time * .3f) * Matrix::Translate(Vector3f(0, -7, 25)) * Matrix::RotateX(-20);
		RenderStates::Lights[0]->Enabled = true;
		RenderStates::Lights[0]->Position = Vector3f(sinf(time * .0075f) * 8, 16 + cosf(time * .005f) * 8, 0);
		RenderStates::EnableStencilMask = false;
		RenderStates::CullMode = CullMode::Back;

		Matrix beethovenMatrix = Matrix::Scale(Vector3f(.25f, .25f, .25f)) * Matrix::RotateXYZ(Vector3f(-time * .3f, time * .3f, time * .3f)) * Matrix::Translate(Vector3f(2, 4, -2));
		scene->Draw(sceneMatrix);
		beethoven->Draw(beethovenMatrix);

		RenderStates::CullMode = CullMode::None;
		int32 shadowTriangleCount =
			sceneShadow->Draw(RenderStates::Lights[0], sceneMatrix, 20) +
			beethovenShadow->Draw(RenderStates::Lights[0], beethovenMatrix, 20);

		RenderStates::Lights[0]->Enabled = false;
		RenderStates::EnableStencilMask = true;
		RenderStates::CullMode = CullMode::Back;

		scene->Draw(sceneMatrix);

		fpsCounter->Frame();
		Examples::DrawRenderingInformation(font, fpsCounter, scene->getTriangleCount() + beethoven->getTriangleCount() + shadowTriangleCount, false);
		Device::Present();
		Input::Update();
	}

	delete fpsCounter, font;
	delete scene;
	delete sceneShadow;

	Device::Destroy();
	return 0;
}