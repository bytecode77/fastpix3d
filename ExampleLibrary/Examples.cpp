#include "ExampleLibrary.h"

const int32 Examples::DefaultWidth = 1280;
const int32 Examples::DefaultHeight = 720;
const int32 Examples::DefaultRectangularResolution = 720;

void Examples::DrawRenderingInformation(Font *font, FPSCounter *fpsCounter, int triangleCount, bool fillBackGround)
{
	if (fillBackGround) Graphics::FillRectangle(10, 10, 200, 99, Color(0, 0, 0), .25f);
	Graphics::DrawString(20, 20, "FPS:", font);
	Graphics::DrawString(20, 41, "Frame time:", font);
	Graphics::DrawString(20, 62, "Triangles:", font);
	Graphics::DrawString(130, 20, Convert::ToString(fpsCounter->getFPS()), font);
	Graphics::DrawString(130, 41, Convert::ToString(fpsCounter->getMedianFrameTime()) + " ms", font);
	Graphics::DrawString(130, 62, Convert::ToString(triangleCount), font);
}
void Examples::DrawInputHint(ExampleType exampleType, Font *font)
{
	switch (exampleType)
	{
		case ExampleType::Freelook:
			Graphics::FillRectangle(220, 10, 280, 99, Color(0, 0, 0), .25f);
			Graphics::DrawString(230, 20, "Mouse", font);
			Graphics::DrawString(230, 41, "WSAD", font);
			Graphics::DrawString(230, 62, "Shift", font);
			Graphics::DrawString(230, 83, "Key 1-5", font);
			Graphics::DrawString(320, 20, "- rotate camera", font);
			Graphics::DrawString(320, 41, "- move camera", font);
			Graphics::DrawString(320, 62, "- run", font);
			Graphics::DrawString(320, 83, "- choose map", font);
			break;
		case ExampleType::Fountain:
			Graphics::FillRectangle(220, 10, 280, 78, Color(0, 0, 0), .25f);
			Graphics::DrawString(230, 20, "Mouse", font);
			Graphics::DrawString(230, 41, "WSAD", font);
			Graphics::DrawString(230, 62, "Shift", font);
			Graphics::DrawString(320, 20, "- rotate camera", font);
			Graphics::DrawString(320, 41, "- move camera", font);
			Graphics::DrawString(320, 62, "- run", font);
			break;
		case ExampleType::Mesh:
			Graphics::FillRectangle(220, 10, 300, 120, Color(0, 0, 0), .25f);
			Graphics::DrawString(230, 20, "Mouse", font);
			Graphics::DrawString(230, 41, "Scroll wheel", font);
			Graphics::DrawString(230, 62, "LMB", font);
			Graphics::DrawString(230, 83, "RMB", font);
			Graphics::DrawString(230, 104, "Keys 1-6", font);
			Graphics::DrawString(340, 20, "- move object (XY)", font);
			Graphics::DrawString(340, 41, "- move object (Z)", font);
			Graphics::DrawString(340, 62, "- rotate object (Y)", font);
			Graphics::DrawString(340, 83, "- rotate object (XYZ)", font);
			Graphics::DrawString(340, 104, "- choose mesh", font);
			break;
	}
}
void Examples::DrawPosition(Font *font, Vector3f position)
{
	Graphics::FillRectangle(510, 10, 200, 99, Color(0, 0, 0), .25f);
	Graphics::DrawString(520, 20, "X", font);
	Graphics::DrawString(520, 41, "Y", font);
	Graphics::DrawString(520, 62, "Z", font);
	Graphics::DrawString(560, 20, Convert::ToString(int32(position.X)), font);
	Graphics::DrawString(560, 41, Convert::ToString(int32(position.Y)), font);
	Graphics::DrawString(560, 62, Convert::ToString(int32(position.Z)), font);
}