#include "Surface.h"

Surface::Surface(int32 vertexCount, int32 triangleCount) :
	_VertexCount(vertexCount),
	_TriangleCount(triangleCount),
	_Vertices(new Vertex[vertexCount]),
	_Triangles(new Triangle[triangleCount]),
	_CullMode(CullMode::Back),
	_Texture(nullptr),
	_TextureSize(vfloat2(1)),
	_BlendMode(BlendMode::Alpha),
	_Alpha(1),
	_SpecularExponent(0),
	_SpecularIntensity(0)
{
}
Surface::~Surface()
{
	delete[] _Vertices;
	delete[] _Triangles;
}

void Surface::AutoNormals()
{
	for (int32 i = 0; i < _VertexCount; i++)
	{
		_Vertices[i].Normals = vfloat3();
	}

	for (int32 i = 0; i < TriangleCount; i++)
	{
		Vertex *vertex1 = &_Vertices[_Triangles[i].Vertex1Index];
		Vertex *vertex2 = &_Vertices[_Triangles[i].Vertex2Index];
		Vertex *vertex3 = &_Vertices[_Triangles[i].Vertex3Index];

		vfloat3 normal = (vertex2->Position - vertex1->Position).CrossProduct(vertex3->Position - vertex1->Position);

		vertex1->Normals += normal;
		vertex2->Normals += normal;
		vertex3->Normals += normal;
	}

	NormalizeNormals();
}
void Surface::NormalizeNormals()
{
	for (int32 i = 0; i < VertexCount; i++)
	{
		_Vertices[i].Normals = _Vertices[i].Normals.Normalize();
	}
}
void Surface::FlipNormals()
{
	for (int32 i = 0; i < VertexCount; i++)
	{
		_Vertices[i].Normals = -_Vertices[i].Normals;
	}
}
void Surface::FlipTriangles()
{
	for (int32 i = 0; i < TriangleCount; i++)
	{
		std::swap(_Triangles[i].Vertex1Index, _Triangles[i].Vertex2Index);
	}
}