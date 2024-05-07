#pragma once
#include "Surface.h"
#include "Texture.h"
#include "Matrix4f.h"
#include "Vector2f.h"
#include "Box3f.h"
#include "Color.h"

class Mesh
{
private:
	std::vector<Surface*> Surfaces;

public:
	property_get(int32, SurfaceCount)
	{
		return (int32)Surfaces.size();
	}
	property_get(int32, VertexCount)
	{
		int32 count = 0;

		for (Surface *surface : Surfaces)
		{
			count += surface->VertexCount;
		}

		return count;
	}
	property_get(int32, TriangleCount)
	{
		int32 count = 0;

		for (Surface *surface : Surfaces)
		{
			count += surface->TriangleCount;
		}

		return count;
	}

	~Mesh();

	static Mesh* Load(const char *path);

	Surface* GetSurface(int32 index) const
	{
		return Surfaces[index];
	}
	Surface* AddSurface(int32 vertexCount, int32 triangleCount);

	void SetCullMode(CullMode cullMode);
	void SetTexture(const Texture *texture);
	void SetTextureSize(float x, float y);
	void SetTextureSize(const Vector2f &size);
	void SetOpacity(float opacity);

	void SetVertexColors(byte r, byte g, byte b);
	void SetVertexColors(const Color &color);
	void SetRandomVertexColors();

	Box3f GetBoundingBox() const;
	void TransformVertices(const Matrix4f &matrix);
	void TransformVertices(const Matrix4f &matrix, bool positions, bool normals);
	void NormalizeNormals();
	void FitToBoundingBox(const Box3f &boundingBox, bool uniform);
	void FlipTriangles();
};