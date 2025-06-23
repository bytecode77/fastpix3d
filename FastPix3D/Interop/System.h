#pragma once
#include "../FastPix3D.h"

class FASTPIX3D_API System
{
public:
	System() = delete;

	static int64 Milliseconds();
	static int64 Microseconds();
};