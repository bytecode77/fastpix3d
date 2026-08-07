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
	// Apply this when benchmarking to reduce interference from other processes:
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	char title[100];
	lstrcpyA(title, "FastPix3D: ");
	lstrcatA(title, name);

	Window = new ::Window(width, height, title);
	RenderUnit = new ::RenderUnit();

	RenderStates = new ::RenderStates();
	RenderStates->FrameBuffer = RenderTarget(*Window);
	RenderStates->DepthBuffer = RenderTarget(Window->Width, Window->Height, _aligned_malloc(Window->Width * Window->Height * 4, 32));
	RenderStates->WireframeDepthBias = 1.01f;

	FreeLook = new ::FreeLook();
	FPSCounter = new ::FPSCounter(500);
	Font10 = new Font("Assets\\Fonts\\inter-10-light.png", 16, 6, 32, 0);
	Font12 = new Font("Assets\\Fonts\\inter-12-light.png", 16, 6, 32, 1);
	Font14 = new Font("Assets\\Fonts\\inter-14-light.png", 16, 6, 32, 1);
	Font16 = new Font("Assets\\Fonts\\inter-16-light.png", 16, 6, 32, 2);
	Font18 = new Font("Assets\\Fonts\\inter-18-light.png", 16, 6, 32, 2);
	Font20 = new Font("Assets\\Fonts\\inter-20-light.png", 16, 6, 32, 2);
	Font22 = new Font("Assets\\Fonts\\inter-22-light.png", 16, 6, 32, 3);
	Font24 = new Font("Assets\\Fonts\\inter-24-light.png", 16, 6, 32, 3);
	Font26 = new Font("Assets\\Fonts\\inter-26-light.png", 16, 6, 32, 4);
	Font28 = new Font("Assets\\Fonts\\inter-28-light.png", 16, 6, 32, 4);
	Font30 = new Font("Assets\\Fonts\\inter-30-light.png", 16, 6, 32, 4);
	Font10Bold = new Font("Assets\\Fonts\\inter-10-bold.png", 16, 6, 32, 0);
	Font12Bold = new Font("Assets\\Fonts\\inter-12-bold.png", 16, 6, 32, 1);
	Font14Bold = new Font("Assets\\Fonts\\inter-14-bold.png", 16, 6, 32, 1);
	Font16Bold = new Font("Assets\\Fonts\\inter-16-bold.png", 16, 6, 32, 2);
	Font18Bold = new Font("Assets\\Fonts\\inter-18-bold.png", 16, 6, 32, 2);
	Font20Bold = new Font("Assets\\Fonts\\inter-20-bold.png", 16, 6, 32, 2);
	Font22Bold = new Font("Assets\\Fonts\\inter-22-bold.png", 16, 6, 32, 3);
	Font24Bold = new Font("Assets\\Fonts\\inter-24-bold.png", 16, 6, 32, 3);
	Font26Bold = new Font("Assets\\Fonts\\inter-26-bold.png", 16, 6, 32, 4);
	Font28Bold = new Font("Assets\\Fonts\\inter-28-bold.png", 16, 6, 32, 4);
	Font30Bold = new Font("Assets\\Fonts\\inter-30-bold.png", 16, 6, 32, 4);
}
ExampleBase::~ExampleBase()
{
	delete Window;

	_aligned_free(RenderStates->DepthBuffer.Buffer);
	if (RenderStates->ShadowMap.Buffer) _aligned_free(RenderStates->ShadowMap.Buffer);

	delete RenderUnit;
	delete RenderStates;

	delete FreeLook;
	delete FPSCounter;
	delete Font10;
	delete Font12;
	delete Font14;
	delete Font16;
	delete Font18;
	delete Font20;
	delete Font22;
	delete Font24;
	delete Font26;
	delete Font28;
	delete Font30;
	delete Font10Bold;
	delete Font12Bold;
	delete Font14Bold;
	delete Font16Bold;
	delete Font18Bold;
	delete Font20Bold;
	delete Font22Bold;
	delete Font24Bold;
	delete Font26Bold;
	delete Font28Bold;
	delete Font30Bold;
}

void ExampleBase::HandleBaseInput()
{
	if (Input::GetKeyPressed(Scancode::X))
	{
		Wireframe = !Wireframe;
	}

	if (Input::GetKeyPressed(Scancode::T))
	{
		RenderStates->TextureFilteringEnable = !RenderStates->TextureFilteringEnable;
	}

	if (Input::GetKeyPressed(Scancode::R))
	{
		FPSCounter->ResetMinFrameTime();
	}
}

void ExampleBase::DrawPerformanceBox(int32 x, int32 y) const
{
	DrawPerformanceBox(x, y, vfloat3());
}
void ExampleBase::DrawPerformanceBox(int32 x, int32 y, vfloat3 cameraPosition) const
{
	bool hasCameraPosition = cameraPosition.X != 0 || cameraPosition.Y != 0 || cameraPosition.Z != 0;
	int32 width = 250;
	int32 height = 115 + (hasCameraPosition ? 18 : 0);

	if (x < 0)
	{
		x = Window->Width - width + x;
	}

	if (y < 0)
	{
		y = Window->Height - height + y;
	}

	Graphics g = Graphics(*Window);
	char str[100];

	char renderedTriangleCountString[20];
	lstrcpyA(renderedTriangleCountString, "(");
	lstrcatA(renderedTriangleCountString, FormatNumber(RenderUnit->Statistics.RenderedTriangleCount, str, true));
	lstrcatA(renderedTriangleCountString, ")");

	g.FillRectangle(x, y, width, height, Color(), .75f, 8);
	g.DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color(255, 255, 255), .3f, 8);
	g.DrawString(x + 10, y + 10, *Font14Bold, "Performance");

	g.DrawString(x + 10, y + 30, *Font30Bold, FormatNumber(FPSCounter->FPS, str), Color(127, 255, 127));
	g.DrawString(x + 18 + g.MeasureString(*Font30Bold, str), y + 45, *Font14, "FPS");

	g.DrawString(x + 10, y + 70, *Font12, "Best Frame Time", Color(230, 230, 230));
	g.DrawString(x + 130, y + 70, *Font12, FormatFixed3Number(FPSCounter->MinFrameTime, str), Color(127, 255, 127));
	g.DrawString(x + 135 + g.MeasureString(*Font12, str), y + 70, *Font12, "ms", Color(230, 230, 230));

	g.DrawString(x + 10, y + 90, *Font12, "Triangles", Color(230, 230, 230));
	g.DrawString(x + 130, y + 90, *Font12, FormatNumber(RenderUnit->Statistics.TotalTriangleCount, str, true), Color(127, 255, 127));
	g.DrawString(x + 135 + g.MeasureString(*Font12, str), y + 90, *Font12, renderedTriangleCountString);

	if (hasCameraPosition)
	{
		char cameraPositionString[20];
		FormatNumber((int32)cameraPosition.X, cameraPositionString);
		lstrcatA(cameraPositionString, ", ");
		lstrcatA(cameraPositionString, FormatNumber((int32)cameraPosition.Y, str));
		lstrcatA(cameraPositionString, ", ");
		lstrcatA(cameraPositionString, FormatNumber((int32)cameraPosition.Z, str));

		g.DrawString(x + 10, y + 110, *Font12, "Camera", Color(230, 230, 230));
		g.DrawString(x + 130, y + 110, *Font12, cameraPositionString, Color(127, 255, 127));
	}
}
void ExampleBase::DrawControlsBox(const char *title, int32 x, int32 y, ...) const
{
	va_list args;
	va_start(args, y);

	Graphics g = Graphics(*Window);

	int32 rows;
	int32 labelWidth = 0;
	int32 textWidth = 0;
	bool hasCheckBoxes = 0;

	const char *labels[10];
	const char *texts[10];
	int32 checkBoxes[10];

	for (rows = 0; rows < 10; rows++)
	{
		labels[rows] = va_arg(args, const char*);
		texts[rows] = va_arg(args, const char*);
		checkBoxes[rows] = va_arg(args, int32);

		if (!labels[rows] || !texts[rows])
		{
			break;
		}

		labelWidth = Math::Max(labelWidth, g.MeasureString(*Font12, labels[rows]));
		textWidth = Math::Max(textWidth, g.MeasureString(*Font12, texts[rows]));
		hasCheckBoxes |= checkBoxes[rows] == 0 || checkBoxes[rows] == 1;
	}

	va_end(args);

	int32 width = 50 + labelWidth + textWidth + (hasCheckBoxes ? 20 : 0);
	int32 height = 45 + rows * 25;

	if (x < 0)
	{
		x = Window->Width - width + x;
	}

	if (y < 0)
	{
		y = Window->Height - height + y;
	}

	g.FillRectangle(x, y, width, height, Color(), .75f, 8);
	g.DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color(255, 255, 255), .3f, 8);
	g.DrawString(x + 10, y + 10, *Font14Bold, title);

	for (int32 i = 0; i < rows; i++)
	{
		g.FillRectangle(x + 10, y + 40 + i * 25, g.MeasureString(*Font12, labels[i]) + 10, 17, Color(255, 255, 255), .2f, 2);
		g.DrawString(x + 15, y + 40 + i * 25, *Font12, labels[i], Color(127, 255, 127));
		g.DrawString(x + 35 + labelWidth, y + 40 + i * 25, *Font12, texts[i]);

		if (checkBoxes[i] == 0 || checkBoxes[i] == 1)
		{
			g.DrawRectangle(x + width - 22, y + 43 + i * 25, 12, 12, Color(255, 255, 255));

			if (checkBoxes[i] == 1)
			{
				g.FillRectangle(x + width - 20, y + 45 + i * 25, 8, 8, Color(127, 255, 127));
			}
		}
	}
}
void ExampleBase::DrawShadowMapImage(int32 x, int32 y, int32 width, int32 height, float zFrom, float zTo) const
{
	width = Math::Min(width, RenderStates->FrameBuffer.Width - 1);
	height = Math::Min(height, RenderStates->FrameBuffer.Height - 1);
	zFrom = RenderStates->ClipNear / zFrom;
	zTo = RenderStates->ClipNear / zTo;

	int32 scale = 0;
	while (RenderStates->ShadowMap.Width >> scale > width || RenderStates->ShadowMap.Height >> scale > height)
	{
		scale++;
	}

	int32 renderWidth = RenderStates->ShadowMap.Width >> scale;
	int32 renderHeight = RenderStates->ShadowMap.Height >> scale;

	Color *frameBuffer = RenderStates->FrameBuffer.GetBuffer<Color>(x + y * RenderStates->FrameBuffer.Width);
	float *shadowMap = RenderStates->ShadowMap.GetBuffer<float>();

	int32 frameBufferStrideY = RenderStates->FrameBuffer.Width - renderWidth;
	int32 shadowMapStrideX = 1 << scale;
	int32 shadowMapStrideY = RenderStates->ShadowMap.Width * (shadowMapStrideX - 1);

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
	char str[100];
	lstrcpyA(title, FormatNumber(RenderStates->ShadowMap.Width, str));
	lstrcatA(title, "x");
	lstrcatA(title, FormatNumber(RenderStates->ShadowMap.Height, str));

	Graphics g = Graphics(*Window);
	g.DrawString(x + 10, y + 10, *Font14Bold, "Shadow Map");
	g.DrawString(x + renderWidth - 10 - g.MeasureString(*Font14, title), y + 10, *Font14, title, Color(127, 255, 127));
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

char* ExampleBase::FormatNumber(int32 number, char *buffer) const
{
	return FormatNumber(number, buffer, false);
}
char* ExampleBase::FormatNumber(int32 number, char *buffer, bool thousandsSeparator) const
{
	if (thousandsSeparator && number >= 1000)
	{
		FormatFixed3Number(number, buffer, "'");
	}
	else
	{
		_itoa(number, buffer, 10);
	}

	return buffer;
}
char* ExampleBase::FormatFixed3Number(int32 number, char *buffer) const
{
	FormatFixed3Number(number, buffer, ".");
	return buffer;
}
vfloat3 ExampleBase::GetCubemapDirection(int32 face) const
{
	switch (face)
	{
		case 0:
			return vfloat3(90, 0, 0);
		case 1:
			return vfloat3(-90, 0, 0);
		case 2:
			return vfloat3(0, -90, 0);
		case 3:
			return vfloat3(0, 90, 0);
		case 4:
			return vfloat3();
		case 5:
			return vfloat3(180, 0, 0);
		default:
			throw std::out_of_range("Cubemap face must be between 0 and 5.");
	}
}

void ExampleBase::FormatFixed3Number(int32 number, char *buffer, const char *separator) const
{
	if (number >= 0)
	{
		buffer[0] = '\0';
	}
	else
	{
		number = -number;
		buffer[0] = '-';
		buffer[1] = '\0';
	}

	_itoa(number / 1000, &buffer[lstrlenA(buffer)], 10);
	lstrcatA(buffer, separator);

	int32 fraction = number % 1000;

	if (fraction < 10)
	{
		lstrcatA(buffer, "00");
	}
	else if (fraction < 100)
	{
		lstrcatA(buffer, "0");
	}

	_itoa(fraction, &buffer[lstrlenA(buffer)], 10);
}