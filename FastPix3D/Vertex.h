#pragma once
#include "FastPix3D.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Color.h"

struct Vertex
{
	Vector3f Position;
	Vector3f Normals;
	Vector2f TextureCoordinates;
	Color Color = ::Color(255, 255, 255);
};