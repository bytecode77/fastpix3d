#pragma once
#include "Window.h"
#include "Input.h"
#include "Thread.h"
#include "RenderUnit.h"
#include "RenderTarget.h"
#include "FreeLook.h"
#include "Vector3f.h"
#include "FPSCounter.h"
#include "Font.h"
#include "PrimitiveFactory.h"

class Example
{
protected:
	Window *Window;
	RenderUnit *RenderUnit;
	FreeLook *FreeLook;
	FPSCounter *FPSCounter;
	const Font *Font;
	float *ShadowMapBuffer;

public:
	explicit Example(int32 width, int32 height, const char *name);
	~Example();

	virtual void Run() = 0;

	void DrawStatisticsBox(int32 x, int32 y) const;
	void DrawPositionBox(int32 x, int32 y, const Vector3f &position) const;
	void DrawFieldSet(int32 x, int32 y, int32 minWidth, int32 minHeight, int32 labelTextSpace, ...) const;

	void DebugShadowMap(int32 x, int32 y, int32 width, int32 height, float zFrom, float zTo) const;
	Mesh* CreateSkybox(const char *path) const;
};