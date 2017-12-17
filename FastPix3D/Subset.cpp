#include "FastPix3D.h"

Subset::Subset(int32 vertexCount, int32 triangleCount)
{
	VertexCount = vertexCount;
	TriangleCount = triangleCount;
	Vertices = new Vertex[VertexCount];
	Triangles = new Triangle[TriangleCount];
	AppliedTexture = NULL;
}
Subset::~Subset()
{
	delete[] Vertices;
	delete[] Triangles;
	delete AppliedTexture;
}

int32 Subset::getVertexCount()
{
	return VertexCount;
}
int32 Subset::getTriangleCount()
{
	return TriangleCount;
}
Vertex* Subset::getVertexBuffer()
{
	return Vertices;
}
Triangle* Subset::getTriangleBuffer()
{
	return Triangles;
}
Texture* Subset::getTexture()
{
	return AppliedTexture;
}