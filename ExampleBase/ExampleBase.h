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
#include <RenderStates.h>

class ExampleBase
{
protected:
	Window *Window;
	RenderUnit *RenderUnit;
	RenderStates *RenderStates;
	FreeLook *FreeLook;
	FPSCounter *FPSCounter;
	const Font *Font10;
	const Font *Font12;
	const Font *Font14;
	const Font *Font16;
	const Font *Font18;
	const Font *Font20;
	const Font *Font22;
	const Font *Font24;
	const Font *Font26;
	const Font *Font28;
	const Font *Font30;
	const Font *Font10Bold;
	const Font *Font12Bold;
	const Font *Font14Bold;
	const Font *Font16Bold;
	const Font *Font18Bold;
	const Font *Font20Bold;
	const Font *Font22Bold;
	const Font *Font24Bold;
	const Font *Font26Bold;
	const Font *Font28Bold;
	const Font *Font30Bold;

	bool Wireframe = false;

public:
	explicit ExampleBase(int32 width, int32 height, const char *name);
	~ExampleBase();

	virtual void Run() = 0;

	void HandleBaseInput();

	void DrawPerformanceBox(int32 x, int32 y) const;
	void DrawPerformanceBox(int32 x, int32 y, vfloat3 cameraPosition) const;
	void DrawControlsBox(const char *title, int32 x, int32 y, ...) const;
	void DrawShadowMapImage(int32 x, int32 y, int32 width, int32 height, float zFrom, float zTo) const;

	Mesh* CreateSkybox(const char *path) const;

	char* FormatNumber(int32 number, char *buffer, bool thousandsSeparator) const;
	vfloat3 GetCubemapDirection(int32 face) const;
};