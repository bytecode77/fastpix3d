#pragma once
#include "../FastPix3D.h"
#include "../Mesh/Mesh.h"

class FASTPIX3D_API PrimitiveFactory
{
public:
	PrimitiveFactory() = delete;

	static Mesh* Plane();
	static Mesh* Plane(int32 segments);
	static Mesh* Plane(int32 xSegments, int32 ySegments);
	static Mesh* Cube();
	static Mesh* Cube(int32 segments);
	static Mesh* Cube(int32 xSegments, int32 ySegments, int32 zSegments);
	static Mesh* Cylinder(int32 segments);
	static Mesh* Cylinder(int32 segments, float topRadius, float bottomRadius);
	static Mesh* Tube(int32 segments, float innerRadius);
	static Mesh* Tube(int32 segments, float topRadius, float topInnerRadius, float bottomRadius, float bottomInnerRadius);
	static Mesh* Cone(int32 segments);
	static Mesh* Sphere(int32 xSegments, int32 ySegments);
	static Mesh* Torus(int32 xSegments, int32 ySegments, float innerRadius);
};