#pragma comment(lib, "ExampleLibrary.lib")
#include <ExampleLibrary.h>

int main(int argc, char *argv[])
{
	Device::Initialize(Examples::DefaultRectangularResolution, Examples::DefaultRectangularResolution);
	RenderStates::ClipNear = .1f;
	RenderStates::Zoom = 2;

	FPSCounter *fpsCounter = new FPSCounter(500);

	Font *font = new Font("Media\\Fonts\\16x16.png");
	Mesh *gear1 = Mesh::FromFile("Media\\Meshes\\Gear1.fp3d");
	Mesh *gear2 = Mesh::FromFile("Media\\Meshes\\Gear2.fp3d");
	Mesh *gear3 = Mesh::FromFile("Media\\Meshes\\Gear3.fp3d");

	RenderStates::CameraSpace = Matrix::RotateY(-30) * Matrix::RotateX(-20) * Matrix::Translate(Vector3f(0, 0, 20));

	while (!Input::getQuit() && !Input::getKeyDown(SDLK_ESCAPE))
	{
		float time = SDL_GetTicks() * .05f;
		Device::ClearBackBuffer(Color(0, 0, 0));
		Device::ClearDepthBuffer();

		gear1->Draw(Matrix::RotateX(-90) * Matrix::RotateZ(time) * Matrix::Translate(Vector3f(-3, -2, 0)));
		gear2->Draw(Matrix::RotateX(-90) * Matrix::RotateZ(time * -2 + 25) * Matrix::Translate(Vector3f(3.1f, -2, 0)));
		gear3->Draw(Matrix::RotateX(-90) * Matrix::RotateZ(time * -2 + 10) * Matrix::Translate(Vector3f(-3.1f, 4.1f, 0)));

		fpsCounter->Frame();
		Examples::DrawRenderingInformation(font, fpsCounter, gear1->getTriangleCount() + gear2->getTriangleCount() + gear3->getTriangleCount(), false);
		Device::Present();
		Input::Update();
	}

	delete fpsCounter, font;
	delete gear1, gear2, gear3;

	Device::Destroy();
	return 0;
}