#pragma comment(lib, "FastPix3D.lib")
#pragma comment(lib, "ExampleLibrary.lib")
#include <ExampleLibrary.h>

void LoadMap(int32 index, Mesh **map, Mesh **skybox, FreelookManager *freelookManager)
{
	if (index >= 1 && index <= 5)
	{
		if (*map != NULL) delete *map;
		if (*skybox != NULL)
		{
			delete *skybox;
			*skybox = NULL;
		}

		switch (index)
		{
			case 1:
				*map = Mesh::FromFile("Media\\Maps\\DoomMap\\DoomMap.fp3d");
				(*map)->TransformVertices(Matrix::Scale(Vector3f(.01f, .01f, .01f)));
				freelookManager->setPosition(Vector3f(0, -1.5f, -1));
				freelookManager->setRotation(Vector3f(0, 0, 0));
				break;
			case 2:
				*map = Mesh::FromFile("Media\\Maps\\Warehouse\\Warehouse.fp3d");
				(*map)->TransformVertices(Matrix::Scale(Vector3f(.025f, .025f, .025f)));
				freelookManager->setPosition(Vector3f(0, -5, 20));
				freelookManager->setRotation(Vector3f(0, 0, 0));
				break;
			case 3:
				*map = Mesh::FromFile("Media\\Maps\\de_inferno\\de_inferno.fp3d");
				(*map)->TransformVertices(Matrix::Scale(Vector3f(.025f, .025f, .025f)) * Matrix::RotateX(90));
				*skybox = Mesh::FromFile("Media\\Skyboxes\\Sky1.fp3d");
				freelookManager->setPosition(Vector3f(-60, -4, -58));
				freelookManager->setRotation(Vector3f(0, 45, 0));
				break;
			case 4:
				*map = Mesh::FromFile("Media\\Maps\\cs_siege\\cs_siege.fp3d");
				(*map)->TransformVertices(Matrix::Scale(Vector3f(.025f, .025f, .025f)) * Matrix::RotateX(90));
				*skybox = Mesh::FromFile("Media\\Skyboxes\\Sky2.fp3d");
				freelookManager->setPosition(Vector3f(-28, -2, -18));
				freelookManager->setRotation(Vector3f(0, 90, 0));
				break;
			case 5:
				*map = Mesh::FromFile("Media\\Maps\\HalfLifeMap\\HalfLifeMap.fp3d");
				(*map)->TransformVertices(Matrix::Scale(Vector3f(.025f, .025f, .025f)) * Matrix::RotateX(90));

				freelookManager->setPosition(Vector3f(-2.5f, 4, -2.5f));
				freelookManager->setRotation(Vector3f(0, 45, 0));
				break;
		}

		freelookManager->StopVelocities();
	}
}

int main(int argc, char *argv[])
{
	Device::Initialize(Examples::DefaultLargeWidth, Examples::DefaultLargeHeight);
	Light::AmbientColor = Color(255, 255, 255);

	FPSCounter *fpsCounter = new FPSCounter(500);
	FreelookManager *freelookManager = new FreelookManager();

	Font *font = new Font("Media\\Fonts\\16x16.png");
	Mesh *map = NULL, *skybox = NULL;
	int32 currentIndex = 1;
	LoadMap(currentIndex, &map, &skybox, freelookManager);

	Input::setMousePosition(Device::getWidth() / 2, Device::getHeight() / 2, false);
	Input::Clear();
	while (!Input::getQuit() && !Input::getKeyDown(SDLK_ESCAPE))
	{
		for (int32 i = 1; i <= 9; i++)
		{
			if (i != currentIndex && Input::getKeyDown('0' + i))
			{
				LoadMap(currentIndex = i, &map, &skybox, freelookManager);
				break;
			}
		}

		float time = SDL_GetTicks() * .05f;
		Device::ClearDepthBuffer();

		freelookManager->Update();
		RenderStates::CameraSpace = freelookManager->getCameraSpaceRotationOnly();
		if (skybox == NULL)
		{
			Device::ClearBackBuffer(Color(0, 0, 0));
		}
		else
		{
			RenderStates::EnableZWrites = false;
			RenderStates::ClipNear = 10;
			skybox->Draw(Matrix::Identity());
		}
		RenderStates::CameraSpace = freelookManager->getCameraSpace();
		RenderStates::EnableZWrites = true;
		RenderStates::ClipNear = .25f;
		map->Draw(Matrix::Identity());

		fpsCounter->Frame();
		Examples::DrawRenderingInformation(font, fpsCounter, map->getTriangleCount(), true);
		Examples::DrawInputHint(ExampleType::Freelook, font);
		Examples::DrawPosition(font, freelookManager->getPosition());
		Device::Present();
		Input::Update();

		Input::setMousePosition(Device::getWidth() / 2, Device::getHeight() / 2, true);
		Input::Clear();
	}

	delete fpsCounter, freelookManager, font;
	delete map;

	Device::Destroy();
	return 0;
}