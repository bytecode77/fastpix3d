#pragma once
#include "FastPix3D.h"
#include "RenderStates.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Texture.h"

class Surface
{
private:
	int32 _VertexCount;
	int32 _TriangleCount;
	Vertex *_Vertices;
	Triangle *_Triangles;
	CullMode _CullMode;
	const Texture *_Texture;
	Vector2f _TextureSize;
	float _Opacity;

public:
	property_get(int32, VertexCount)
	{
		return _VertexCount;
	}
	property_get(int32, TriangleCount)
	{
		return _TriangleCount;
	}
	property_getset(::CullMode, CullMode)
	{
		return _CullMode;
	}
	property_set(::CullMode, CullMode)
	{
		_CullMode = value;
	}
	property_getset(const ::Texture*, Texture)
	{
		return _Texture;
	}
	property_set(const ::Texture*, Texture)
	{
		_Texture = value;
	}
	property_getset(Vector2f, TextureSize)
	{
		return _TextureSize;
	}
	property_set(const Vector2f&, TextureSize)
	{
		_TextureSize = value;
	}
	property_getset(float, Opacity)
	{
		return _Opacity;
	}
	property_set(float, Opacity)
	{
		_Opacity = value;
	}

	explicit Surface(int32 vertexCount, int32 triangleCount) :
		_VertexCount(vertexCount),
		_TriangleCount(triangleCount),
		_Vertices(new Vertex[vertexCount]),
		_Triangles(new Triangle[triangleCount]),
		_CullMode(CullMode::Back),
		_Texture(nullptr),
		_TextureSize(Vector2f(1)),
		_Opacity(1)
	{
	}
	~Surface()
	{
		delete[] _Vertices;
		delete[] _Triangles;
	}

	Vertex* GetVertex(int32 index) const
	{
		if (index < 0 || index >= _VertexCount) throw;

		return &_Vertices[index];
	}
	Vertex* GetTriangleVertex(int32 triangleIndex, int32 vertexNumber) const
	{
		if (triangleIndex < 0 || triangleIndex >= _TriangleCount) throw;

		switch (vertexNumber)
		{
			case 0: return _Triangles[triangleIndex].Vertex1;
			case 1: return _Triangles[triangleIndex].Vertex2;
			case 2: return _Triangles[triangleIndex].Vertex3;
			default: throw;
		}
	}
	void SetVertex(int32 index, const Vector3f &position)
	{
		SetVertex(index, position, Vector3f());
	}
	void SetVertex(int32 index, const Vector3f &position, const Vector3f &normals)
	{
		SetVertex(index, position, normals, Vector2f());
	}
	void SetVertex(int32 index, const Vector3f &position, const Vector3f &normals, const Vector2f &textureCoordinates)
	{
		SetVertex(index, position, normals, textureCoordinates, Color(255, 255, 255));
	}
	void SetVertex(int32 index, const Vector3f &position, const Vector3f &normals, const Vector2f &textureCoordinates, const Color &color)
	{
		if (index < 0 || index >= _VertexCount) throw;

		Vertex *vertex = &_Vertices[index];
		vertex->Position = position;
		vertex->Normals = normals;
		vertex->TextureCoordinates = textureCoordinates;
		vertex->Color = color;
	}
	void SetTriangle(int32 triangleIndex, int32 vertex1Index, int32 vertex2Index, int32 vertex3Index)
	{
		if (triangleIndex < 0 || triangleIndex >= _TriangleCount) throw;
		if (vertex1Index < 0 || vertex1Index >= _VertexCount) throw;
		if (vertex2Index < 0 || vertex2Index >= _VertexCount) throw;
		if (vertex3Index < 0 || vertex3Index >= _VertexCount) throw;

		_Triangles[triangleIndex].Vertex1 = &_Vertices[vertex1Index];
		_Triangles[triangleIndex].Vertex2 = &_Vertices[vertex2Index];
		_Triangles[triangleIndex].Vertex3 = &_Vertices[vertex3Index];
	}

	void NormalizeNormals()
	{
		for (int32 i = 0; i < VertexCount; i++)
		{
			_Vertices[i].Normals = _Vertices[i].Normals.Normalize();
		}
	}
	void FlipTriangles()
	{
		for (int32 i = 0; i < TriangleCount; i++)
		{
			std::swap(_Triangles[i].Vertex1, _Triangles[i].Vertex2);
		}
	}
};