#include "Example.h"
#include "Color.h"
#include "Mesh.h"
#include "PrimitiveFactory.h"
#include "Texture.h"
#include "Input.h"
#include "Graphics.h"
#include "Matrix4f.h"
#include "Math_.h"

Example::Example(int32 width, int32 height, const char *name)
{
	char title[100];
	lstrcpyA(title, "FastPix3D: ");
	lstrcatA(title, name);

	Window = new ::Window(width, height, title);

	ShadowMapBuffer = new float[4096 * 4096];

	RenderUnit = new ::RenderUnit();
	RenderUnit->RenderStates.FrameBuffer = RenderTarget(*Window);
	RenderUnit->RenderStates.DepthBuffer = RenderTarget::Create<float>(Window->Width, Window->Height);
	RenderUnit->RenderStates.StencilBuffer = RenderTarget::Create<byte>(Window->Width, Window->Height);
	RenderUnit->RenderStates.ShadowMap = RenderTarget(2048, 2048, ShadowMapBuffer);

	FreeLook = new ::FreeLook();
	FPSCounter = new ::FPSCounter(500);
	Font = new ::Font("Assets\\Fonts\\16x16.png");
}
Example::~Example()
{
	delete Window;

	delete[] RenderUnit->RenderStates.DepthBuffer.Buffer;
	delete[] RenderUnit->RenderStates.StencilBuffer.Buffer;
	delete[] ShadowMapBuffer;

	delete RenderUnit;

	delete FreeLook;
	delete FPSCounter;
	delete Font;
}

void Example::DrawStatisticsBox(int32 x, int32 y) const
{
	char buffer1[100];
	char buffer2[100];

	DrawFieldSet(
		x,
		y,
		170,
		0,
		20,
		"FPS:",
		_itoa(FPSCounter->FPS, buffer1, 10),
		"Frame Time:",
		_itoa(FPSCounter->AverageFrameTime, buffer2, 10),
		nullptr
	);

	DrawFieldSet(
		x + 170,
		y,
		170,
		0,
		20,
		"Triangles:",
		_itoa(RenderUnit->Statistics.TotalTriangleCount, buffer1, 10),
		"Rendered:",
		_itoa(RenderUnit->Statistics.RenderedTriangleCount, buffer2, 10),
		nullptr
	);
}
void Example::DrawPositionBox(int32 x, int32 y, const Vector3f &position) const
{
	char buffer[100];
	sprintf(buffer, "%d, %d, %d", (int32)position.X, (int32)position.Y, (int32)position.Z);

	DrawFieldSet(x, y, 200, 0, 30, "Camera:", buffer, "", "", nullptr);
}
void Example::DrawFieldSet(int32 x, int32 y, int32 minWidth, int32 minHeight, int32 labelTextSpace, ...) const
{
	va_list args;
	va_start(args, labelTextSpace);

	Graphics g = Graphics(*Window);
	g.SetFont(*Font);

	int32 rows;
	int32 labelWidth = 0;
	int32 textWidth = 0;
	const char *labels[10];
	const char *texts[10];

	for (rows = 0; rows < 10; rows++)
	{
		labels[rows] = va_arg(args, const char *);
		texts[rows] = va_arg(args, const char *);

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
		Math::Max(20 + labelWidth + labelTextSpace + textWidth, minWidth),
		Math::Max(18 + rows * 20, minHeight),
		Color(),
		.5f
	);

	for (int32 i = 0; i < rows; i++)
	{
		g.DrawString(x + 10, y + 10 + i * 20, labels[i]);
		g.DrawString(x + 10 + labelWidth + labelTextSpace, y + 10 + i * 20, texts[i]);
	}
}

void Example::DebugShadowMap(int32 x, int32 y, int32 width, int32 height, float zFrom, float zTo) const
{
	width = Math::Min(width, RenderUnit->RenderStates.FrameBuffer.Width - 1);
	height = Math::Min(height, RenderUnit->RenderStates.FrameBuffer.Height - 1);
	zFrom = 1 / zFrom * RenderUnit->RenderStates.ClipNear;
	zTo = 1 / zTo * RenderUnit->RenderStates.ClipNear;

	int32 scale = 0;
	while (RenderUnit->RenderStates.ShadowMap.Width >> scale > width || RenderUnit->RenderStates.ShadowMap.Height >> scale > height)
	{
		scale++;
	}

	for (int32 py = 0; py < RenderUnit->RenderStates.ShadowMap.Height >> scale; py++)
	{
		float *shadowMap = RenderUnit->RenderStates.ShadowMap.GetBuffer<float>((py << scale) * RenderUnit->RenderStates.ShadowMap.Width);
		int32 *frameBuffer = RenderUnit->RenderStates.FrameBuffer.GetBuffer<int32>(x + (py + y) * RenderUnit->RenderStates.FrameBuffer.Width);

		for (int32 px = 0; px < RenderUnit->RenderStates.ShadowMap.Width >> scale; px++)
		{
			if (*shadowMap > 0)
			{
				int32 color = (int32)Math::Interpolate(*shadowMap, zFrom, zTo, 255.0f, 0.0f) & 0xff;
				*frameBuffer = color << 16 | color << 8 | color;
			}
			else
			{
				int32 r = (*frameBuffer >> 16) & 0xff;
				int32 g = (*frameBuffer >> 8) & 0xff;
				int32 b = *frameBuffer & 0xff;
				*frameBuffer = (r / 2) << 16 | (g / 2) << 8 | (b / 2);
			}

			frameBuffer++;
			shadowMap += 1 << scale;
		}
	}

	char title[100];
	char buffer[100];
	lstrcpyA(title, "Shadow Map ");
	lstrcatA(title, _itoa(RenderUnit->RenderStates.ShadowMap.Width, buffer, 10));
	lstrcatA(title, "x");
	lstrcatA(title, _itoa(RenderUnit->RenderStates.ShadowMap.Height, buffer, 10));

	Graphics g = Graphics(*Window);
	g.SetFont(*Font);
	g.DrawString(x + 10, y + 10, title);
}
Mesh* Example::CreateSkybox(const char *path) const
{
	char fileName[MAX_PATH];
	int32 sideIndex = StrStrA(path, "XX") - path; // "XX" will be replaced with UP, DN, LF, RT, FT, BK.

	Mesh *skybox = PrimitiveFactory::Cube();
	skybox->FlipTriangles();

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "UP", 2);
	skybox->GetSurface(0)->Texture = Texture::Load(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "DN", 2);
	skybox->GetSurface(1)->Texture = Texture::Load(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "LF", 2);
	skybox->GetSurface(2)->Texture = Texture::Load(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "RT", 2);
	skybox->GetSurface(3)->Texture = Texture::Load(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "FT", 2);
	skybox->GetSurface(4)->Texture = Texture::Load(fileName);

	lstrcpyA(fileName, path);
	memcpy(&fileName[sideIndex], "BK", 2);
	skybox->GetSurface(5)->Texture = Texture::Load(fileName);

	// Remove 0.5 pixels from each side to fix texture seams.
	skybox->SetTextureSize(Vector2f(-1, 1) * (1 + .5f / 256));

	return skybox;
}