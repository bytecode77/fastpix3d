#pragma once
#include "../FastPix3D.h"
#include "../RenderStates.h"
#include "../Texture.h"
#include "Triangle.h"
#include "Vertex.h"

class FASTPIX3D_API Surface
{
private:
	const int32 _VertexCount;
	const int32 _TriangleCount;
	Vertex *_Vertices;
	Triangle *_Triangles;
	CullMode _CullMode;
	const Texture *_Texture;
	vfloat2 _TextureSize;
	BlendMode _BlendMode;
	float _Alpha;
	float _SpecularExponent;
	float _SpecularIntensity;

public:
	readonly_property(int32, VertexCount)
	{
		return _VertexCount;
	}
	readonly_property(int32, TriangleCount)
	{
		return _TriangleCount;
	}
	property_get(::CullMode, CullMode)
	{
		return _CullMode;
	}
	property_set(::CullMode, CullMode)
	{
		_CullMode = value;
	}
	property_get(const ::Texture*, Texture)
	{
		return _Texture;
	}
	property_set(const ::Texture*, Texture)
	{
		_Texture = value;
	}
	property_get(vfloat2, TextureSize)
	{
		return _TextureSize;
	}
	property_set(const vfloat2&, TextureSize)
	{
		_TextureSize = value;
	}
	property_get(::BlendMode, BlendMode)
	{
		return _BlendMode;
	}
	property_set(::BlendMode, BlendMode)
	{
		_BlendMode = value;
	}
	property_get(float, Alpha)
	{
		return _Alpha;
	}
	property_set(float, Alpha)
	{
		_Alpha = value;
	}
	property_get(float, SpecularExponent)
	{
		return _SpecularExponent;
	}
	property_set(float, SpecularExponent)
	{
		_SpecularExponent = value;
	}
	property_get(float, SpecularIntensity)
	{
		return _SpecularIntensity;
	}
	property_set(float, SpecularIntensity)
	{
		_SpecularIntensity = value;
	}

	explicit Surface(int32 vertexCount, int32 triangleCount);
	~Surface();

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
			case 0: return &_Vertices[_Triangles[triangleIndex].Vertex1Index];
			case 1: return &_Vertices[_Triangles[triangleIndex].Vertex2Index];
			case 2: return &_Vertices[_Triangles[triangleIndex].Vertex3Index];
			default: throw;
		}
	}
	void SetVertex(int32 index, const vfloat3 &position)
	{
		SetVertex(index, position, vfloat3());
	}
	void SetVertex(int32 index, const vfloat3 &position, const vfloat3 &normals)
	{
		SetVertex(index, position, normals, vfloat2());
	}
	void SetVertex(int32 index, const vfloat3 &position, const vfloat3 &normals, const vfloat2 &textureCoordinates)
	{
		SetVertex(index, position, normals, textureCoordinates, Color(255, 255, 255));
	}
	void SetVertex(int32 index, const vfloat3 &position, const vfloat3 &normals, const vfloat2 &textureCoordinates, const Color &color)
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

		_Triangles[triangleIndex].Vertex1Index = vertex1Index;
		_Triangles[triangleIndex].Vertex2Index = vertex2Index;
		_Triangles[triangleIndex].Vertex3Index = vertex3Index;
	}

	void NormalizeNormals();
	void FlipNormals();
	void FlipTriangles();
};