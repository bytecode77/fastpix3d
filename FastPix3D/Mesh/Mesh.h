#pragma once
#include "../FastPix3D.h"
#include "../Math/Box3f.h"
#include "../Math/Color.h"
#include "../Math/Matrix4f.h"
#include "../Math/VectorMath.h"
#include "../Texture.h"
#include "Surface.h"

class FASTPIX3D_API Mesh
{
private:
	std::vector<Surface*> Surfaces;

public:
	readonly_property(int32, SurfaceCount)
	{
		return (int32)Surfaces.size();
	}
	readonly_property(int32, VertexCount)
	{
		int32 count = 0;

		for (const Surface *surface : Surfaces)
		{
			count += surface->VertexCount;
		}

		return count;
	}
	readonly_property(int32, TriangleCount)
	{
		int32 count = 0;

		for (const Surface *surface : Surfaces)
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
	Surface* GetSurface(const char *textureFileName) const;
	Surface* AddSurface(int32 vertexCount, int32 triangleCount);

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
	void TransformVertices(const Matrix4f &matrix);
	void TransformVertices(const Matrix4f &matrix, bool positions, bool normals);
	void NormalizeNormals();
	void FlipNormals();
	void FlipTriangles();

private:
	static Mesh* LoadGltf(const char *path);
	static Mesh* LoadObj(const char *path);
	static Texture* LoadTexture(const char *meshPath, const char *textureFileName);
};