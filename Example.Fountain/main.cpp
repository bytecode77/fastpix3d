#pragma comment(lib, "ExampleLibrary.lib")
#include <ExampleLibrary.h>

int main(int argc, char *argv[])
{
	Device::Initialize(Examples::DefaultWidth, Examples::DefaultHeight);
	RenderStates::ClipNear = .25f;

	FPSCounter *fpsCounter = new FPSCounter(500);
	FreelookManager *freelookManager = new FreelookManager();
	freelookManager->setPosition(Vector3f(-8, 0, 8));

	Font *font = new Font("Media\\Fonts\\16x16.png");
	Mesh *room = Mesh::FromFile("Media\\Fountain\\Room.fp3d");
	Mesh *railing = Mesh::FromFile("Media\\Fountain\\Railing.fp3d");
	Mesh *fountain = Mesh::FromFile("Media\\Fountain\\Fountain.fp3d");

	Matrix roomMatrix = Matrix::RotateX(-90);
	Matrix railingMatrix = Matrix::RotateX(-90);
	Matrix fountainMatrix = Matrix::RotateX(-90) * Matrix::Translate(Vector3f(0, .1f, 0));

	ShadowVolume *railingShadow = new ShadowVolume(railing);
	ShadowVolume *fountainShadow = new ShadowVolume(fountain);

	RenderStates::Lights[0]->Enabled = true;
	RenderStates::Lights[0]->Type = LightType::Spot;
	RenderStates::Lights[0]->Intensity = 40;
	RenderStates::Lights[0]->DiffuseColor = Color(165, 210, 255);

	for (int32 x = -1, i = 1; x <= 1; x += 2)
	{
		for (int32 z = -1; z <= 1; z += 2, i++)
		{
			RenderStates::Lights[i]->Enabled = true;
			RenderStates::Lights[i]->Type = LightType::Point;
			RenderStates::Lights[i]->Intensity = 2;
			RenderStates::Lights[i]->DiffuseColor = Color(255, 200, 155);
			RenderStates::Lights[i]->Position = Vector3f(x * 8, -.5f, z * 8 - (z < 0 ? 1.f : 0.f));
		}
	}

	Input::setMousePosition(Device::getWidth() / 2, Device::getHeight() / 2, false);
	Input::Clear();
	while (!Input::getQuit() && !Input::getKeyDown(SDLK_ESCAPE))
	{
		float time = SDL_GetTicks() * .05f;
		Device::ClearBackBuffer();
		Device::ClearDepthBuffer();
		Device::ClearStencilBuffer();

		RenderStates::CameraSpace = freelookManager->Update();
		RenderStates::Lights[0]->Enabled = true;
		RenderStates::Lights[0]->Position = Vector3f(cos(time * .01f) * 8.5f, 1.5f, sin(time * .01f) * 8.5f);
		RenderStates::Lights[0]->Rotation = Vector3f(0, (-1.5f - time * .01f) * 180 / float(M_PI), 0);
		RenderStates::EnableStencilMask = false;
		RenderStates::CullMode = CullMode::Back;
		Light::AmbientColor = Color(20, 20, 20);

		room->Draw(roomMatrix);
		railing->Draw(railingMatrix);
		fountain->Draw(fountainMatrix);

		RenderStates::CullMode = CullMode::None;
		int32 shadowTriangleCount =
			railingShadow->Draw(RenderStates::Lights[0], railingMatrix, 20) +
			fountainShadow->Draw(RenderStates::Lights[0], fountainMatrix, 20);

		RenderStates::Lights[0]->Enabled = false;
		RenderStates::EnableStencilMask = true;
		RenderStates::CullMode = CullMode::Back;

		room->Draw(roomMatrix);
		railing->Draw(railingMatrix);
		fountain->Draw(fountainMatrix);

		fpsCounter->Frame();
		Examples::DrawRenderingInformation(font, fpsCounter, room->getTriangleCount() + railing->getTriangleCount() + fountain->getTriangleCount() + shadowTriangleCount, true);
		Examples::DrawInputHint(ExampleType::Fountain, font);
		Examples::DrawPosition(font, freelookManager->getPosition());
		Device::Present();
		Input::Update();

		Input::setMousePosition(Device::getWidth() / 2, Device::getHeight() / 2, true);
		Input::Clear();
	}

	delete fpsCounter, freelookManager, font;
	delete room, fountain, railing;
	delete fountainShadow, railingShadow;

	Device::Destroy();
	return 0;
}