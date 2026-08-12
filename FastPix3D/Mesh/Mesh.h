#pragma once
#include "../FastPix3D.h"
#include "../Math/Box3f.h"
#include "../Math/Color.h"
#include "../Math/Matrix4.h"
#include "../Math/VectorMath.h"
#include "Surface.h"
#include "Texture.h"

class FASTPIX3D_API Mesh
{
private:
	std::vector<Surface*> _Surfaces;

public:
	readonly_property(int32, SurfaceCount)
	{
		return (int32)_Surfaces.size();
	}
	readonly_property(int32, VertexCount)
	{
		int32 count = 0;

		for (const Surface *surface : _Surfaces)
		{
			count += surface->VertexCount;
		}

		return count;
	}
	readonly_property(int32, TriangleCount)
	{
		int32 count = 0;

		for (const Surface *surface : _Surfaces)
		{
			count += surface->TriangleCount;
		}

		return count;
	}
	readonly_indexed_property(Surface*, Surfaces, int32 index)
	{
		return _Surfaces[index];
	}

	Mesh() = default;
	Mesh(const Mesh&) = delete;
	~Mesh();

	static Mesh* Load(const char *path);

	Surface* FindSurface(const char *textureFileName) const;
	Surface* AddSurface(int32 vertexCount, int32 triangleCount);
	void RemoveSurface(int32 index);
	void RemoveSurface(const Surface *surface);

	void SetCullMode(CullMode cullMode);
	void SetTexture(const Texture *texture);
	void SetTextureSize(float x, float y);
	void SetTextureSize(const vfloat2 &size);
	void SetBlendMode(BlendMode blendMode);
	void SetAlpha(float alpha);
	void SetSpecular(float specularExponent, float specularIntensity);
	void SetSpecularExponent(float specularExponent);
	void SetSpecularIntensity(float specularIntensity);

	void SetVertexColors(byte r, byte g, byte b);
	void SetVertexColors(const Color &color);
	void SetRandomVertexColors();

	Box3f GetBoundingBox() const;
	void FitToBoundingBox(const Box3f &boundingBox, bool uniform);
	void TransformVertices(const Matrix4 &matrix);
	void TransformVertices(const Matrix4 &matrix, bool positions, bool normals);
	void AutoNormals();
	void NormalizeNormals();
	void FlipNormals();
	void FlipTriangles();

private:
	static Mesh* LoadGltf(const char *path);
	static Mesh* LoadObj(const char *path);
	static Texture* LoadTexture(const char *meshPath, const char *textureFileName);

public:
	Mesh& operator =(const Mesh&) = delete;
};