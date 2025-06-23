#pragma once
#include "../FastPix3D.h"
#include "../Math/Color.h"
#include "../Math/VectorMath.h"

struct FASTPIX3D_API Vertex
{
	vfloat3 Position;
	vfloat3 Normals;
	vfloat2 TextureCoordinates;
	Color Color = ::Color(255, 255, 255);
};