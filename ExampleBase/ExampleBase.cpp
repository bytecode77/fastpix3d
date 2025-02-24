#include "ExampleBase.h"
#include <2D/Graphics.h>
#include <Helper/PrimitiveFactory.h>
#include <Interop/Input.h>
#include <Math/Color.h>
#include <Math/Math_.h>
#include <Math/Matrix4f.h>
#include <Mesh/Mesh.h>
#include <Texture.h>

ExampleBase::ExampleBase(int32 width, int32 height, const char *name)
{
	char title[100];
	lstrcpyA(title, "FastPix3D: ");
	lstrcatA(title, name);

	Window = new ::Window(width, height, title);

	RenderUnit = new ::RenderUnit();
	RenderUnit->RenderStates.FrameBuffer = RenderTarget(*Window);
	RenderUnit->RenderStates.DepthBuffer = RenderTarget(Window->Width, Window->Height, _aligned_malloc(Window->Width * Window->Height * 4, 32));
	RenderUnit->RenderStates.WireframeDepthBias = 1.01f;

	FreeLook = new ::FreeLook();
	FPSCounter = new ::FPSCounter(500);
	Font = new ::Font("Assets\\Fonts\\16x16.png");
}
ExampleBase::~ExampleBase()
{
	delete Window;

	_aligned_free(RenderUnit->RenderStates.DepthBuffer.Buffer);
	delete RenderUnit;

	delete FreeLook;
	delete FPSCounter;
	delete Font;
}

void ExampleBase::HandleBaseInput()
{
	if (Input::GetKeyPressed(Scancode::X))
	{
		Wireframe = !Wireframe;
	}

	if (Input::GetKeyPressed(Scancode::T))
	{
		RenderUnit->RenderStates.TextureFilteringEnable = !RenderUnit->RenderStates.TextureFilteringEnable;
	}

	if (Input::GetKeyPressed(Scancode::R))
	{
		FPSCounter->ResetMinFrameTime();
	}
}

void ExampleBase::DrawStatisticsBox(int32 x, int32 y) const
{
	char text1[100];
	char text2[100];
	char text3[100];
	char tmp[100];

	_itoa(FPSCounter->MinFrameTime / 1000, text2, 10);
	lstrcatA(text2, ".");
	lstrcatA(text2, _itoa(FPSCounter->MinFrameTime % 1000, tmp, 10));
	lstrcatA(text2, " ms");

	_itoa(RenderUnit->Statistics.TotalTriangleCount, text3, 10);
	lstrcatA(text3, " (");
	lstrcatA(text3, _itoa(RenderUnit->Statistics.RenderedTriangleCount, tmp, 10));
	lstrcatA(text3, ")");

	DrawFieldSet(
		x,
		y,
		230,
		0,
		20,
		"FPS:",
		_itoa(FPSCounter->FPS, text1, 10),
		"min. Time:",
		text2,
		"Triangles:",
		text3,
		nullptr
	);
}
void ExampleBase::DrawPositionBox(int32 x, int32 y, const vfloat3 &position) const
{
	char buffer[100];
	sprintf(buffer, "%d, %d, %d", (int32)position.X, (int32)position.Y, (int32)position.Z);

	DrawFieldSet(x, y, 190, 0, 20, "Camera:", buffer, "", "", nullptr);
}
void ExampleBase::DrawFieldSet(int32 x, int32 y, int32 minWidth, int32 minHeight, int32 labelTextSpace, ...) const
{
	va_list args;
	va_start(args, labelTextSpace);

	Graphics g = Graphics(*Window);
	g.Font = Font;

	int32 rows;
	int32 labelWidth = 0;
	int32 textWidth = 0;
	const int32 paddingX = 10;
	const int32 paddingY = 5;

	const char *labels[10];
	const char *texts[10];

	for (rows = 0; rows < 10; rows++)
	{
		labels[rows] = va_arg(args, const char*);
		texts[rows] = va_arg(args, const char*);

		if (!labels[rows] || !texts[rows])
		{
			break;
		}

		labelWidth = Math::Max(labelWidth, g.MeasureString(labels[rows]));
		textWidth = Math::Max(textWidth, g.MeasureString(texts[rows]));
	}

	va_end(args);

	g.FillRectangle(
		x,
		y,
		Math::Max(paddingX * 2 + labelWidth + labelTextSpace + textWidth, minWidth),
		Math::Max(paddingY * 2 + rows * 20, minHeight),
		Color(),
		.5f
	);

	for (int32 i = 0; i < rows; i++)
	{
		g.DrawString(x + paddingX, y + paddingY + i * 20, labels[i]);
		g.DrawString(x + paddingX + labelWidth + labelTextSpace, y + paddingY + i * 20, texts[i]);
	}
}

Mesh* ExampleBase::CreateSkybox(const char *path) const
{
	char fileName[MAX_PATH];
	int32 sideIndex = (int32)(StrStrA(path, "XX") - path); // "XX" will be replaced with UP, DN, LF, RT, FT, BK.

	Mesh *skybox = PrimitiveFactory::Cube();
	skybox->FlipTriangles();
	skybox->SetTextureSize(vfloat2(-1, 1));

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "UP", 2);
	skybox->GetSurface(0)->Texture = Texture::FromFile(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "DN", 2);
	skybox->GetSurface(1)->Texture = Texture::FromFile(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "LF", 2);
	skybox->GetSurface(2)->Texture = Texture::FromFile(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "RT", 2);
	skybox->GetSurface(3)->Texture = Texture::FromFile(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "FT", 2);
	skybox->GetSurface(4)->Texture = Texture::FromFile(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "BK", 2);
	skybox->GetSurface(5)->Texture = Texture::FromFile(fileName);

	return skybox;
}