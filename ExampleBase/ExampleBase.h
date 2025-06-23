#pragma once
#include <2D/Font.h>
#include <2D/Graphics.h>
#include <Helper/FreeLook.h>
#include <Helper/PrimitiveFactory.h>
#include <Interop/FPSCounter.h>
#include <Interop/Input.h>
#include <Interop/Stopwatch.h>
#include <Interop/System.h>
#include <Interop/Thread.h>
#include <Interop/ThreadPool.h>
#include <Interop/Window.h>
#include <Math/Math_.h>
#include <Math/VectorMath.h>
#include <RenderTarget.h>
#include <RenderUnit.h>

class ExampleBase
{
protected:
	Window *Window;
	RenderUnit *RenderUnit;
	FreeLook *FreeLook;
	FPSCounter *FPSCounter;
	const Font *Font;

	bool Wireframe = false;

public:
	explicit ExampleBase(int32 width, int32 height, const char *name);
	~ExampleBase();

	virtual void Run() = 0;

	void HandleBaseInput();

	void DrawStatisticsBox(int32 x, int32 y) const;
	void DrawPositionBox(int32 x, int32 y, const vfloat3 &position) const;
	void DrawFieldSet(int32 x, int32 y, int32 minWidth, int32 minHeight, int32 labelTextSpace, ...) const;

	Mesh* CreateSkybox(const char *path) const;
};