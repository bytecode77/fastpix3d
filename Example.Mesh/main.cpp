#pragma comment(lib, "ExampleLibrary.lib")
#include <ExampleLibrary.h>

void LoadMesh(int32 index, Mesh **mesh)
{
	if (index >= 1 && index <= 6)
	{
		if (*mesh != NULL) delete *mesh;
		switch (index)
		{
			case 1:
				*mesh = Mesh::FromFile("Media\\Meshes\\TestObject.fp3d");
				(*mesh)->TransformVertices(Matrix::Scale(Vector3f(.4f, .4f, .4f)));
				break;
			case 2:
				*mesh = Mesh::FromFile("Media\\Meshes\\Crate.fp3d");
				(*mesh)->TransformVertices(Matrix::Scale(Vector3f(.03f, .03f, .03f)));
				break;
			case 3:
				*mesh = Mesh::FromFile("Media\\Meshes\\Wheel.fp3d");
				(*mesh)->TransformVertices(Matrix::Scale(Vector3f(.4f, .4f, .4f)));
				break;
			case 4:
				*mesh = Mesh::FromFile("Media\\Meshes\\Warrior.fp3d");
				(*mesh)->TransformVertices(Matrix::Scale(Vector3f(.4f, .4f, .4f)) * Matrix::Translate(Vector3f(0, -2, 0)));
				break;
			case 5:
				*mesh = Mesh::FromFile("Media\\Meshes\\GDI_MammothTank.fp3d");
				(*mesh)->TransformVertices(Matrix::Scale(Vector3f(.15f, .15f, .15f)) * Matrix::Translate(Vector3f(0, -1, 0)));
				break;
			case 6:
				*mesh = Mesh::FromFile("Media\\Meshes\\M9.fp3d");
				(*mesh)->TransformVertices(Matrix::Scale(Vector3f(.4f, .4f, .4f)));
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	Device::Initialize(Examples::DefaultWidth, Examples::DefaultHeight);
	RenderStates::ClipNear = .1f;

	FPSCounter *fpsCounter = new FPSCounter(500);

	Font *font = new Font("Media\\Fonts\\16x16.png");
	Mesh *mesh = NULL;
	int32 currentIndex = 2;
	LoadMesh(currentIndex, &mesh);

	RenderStates::Lights[0]->Enabled = true;
	RenderStates::Lights[0]->DiffuseColor = Color(255, 252, 245);
	Light::AmbientColor = Color(19, 21, 23);

	Input::setMousePosition(Device::getWidth() / 2, Device::getHeight() / 2, false);
	Matrix rotation = Matrix::Identity();
	float x = 0, y = 0, z = 10, vrx = 0, vry = 0, vrz = 0;

	while (!Input::getQuit() && !Input::getKeyDown(SDLK_ESCAPE))
	{
		for (int32 i = 1; i <= 9; i++)
		{
			if (i != currentIndex && Input::getKeyDown('0' + i))
			{
				LoadMesh(currentIndex = i, &mesh);
			}
		}

		Device::ClearBackBuffer(Color(0, 74, 127));
		Device::ClearDepthBuffer();

		float vx = (Input::getMouseX() - Device::getWidth() / 2) * 10.f / Device::getWidth();
		float vy = (Device::getHeight() / 2 - Input::getMouseY()) * 10.f / Device::getWidth();
		float vz = Input::getMouseZ() * .5f + 10;
		vrx = (vrx + Input::getMouseDown(MouseButton::Right) * .2f) * .8f;
		vry = (vry + (Input::getMouseDown(MouseButton::Left) || Input::getMouseDown(MouseButton::Right)) * .2f) * .8f;
		vrz = (vrz + Input::getMouseDown(MouseButton::Right) * .2f) * .8f;
		x = x * .8f + vx * .2f;
		y = y * .8f + vy * .2f;
		z = z * .8f + vz * .2f;
		rotation = rotation * Matrix::RotateXYZ(Vector3f(vrx, vry, vrz));

		mesh->Draw(rotation * Matrix::Translate(Vector3f(x, y, z)));

		fpsCounter->Frame();
		Examples::DrawRenderingInformation(font, fpsCounter, mesh->getTriangleCount(), true);
		Examples::DrawInputHint(ExampleType::Mesh, font);
		Device::Present();
		Input::Update();
	}

	delete fpsCounter, font;
	delete mesh;

	Device::Destroy();
	return 0;
}