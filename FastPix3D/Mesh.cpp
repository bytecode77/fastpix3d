#include "Mesh.h"
#include "Math_.h"
#include "ObjLoader.h"

Mesh::~Mesh()
{
	for (Surface *surface : Surfaces)
	{
		delete surface;
	}
}

Mesh* Mesh::Load(const char *path)
{
	Mesh *mesh = new Mesh();

	objl::Loader loader;
	if (!loader.LoadFile(path)) throw;

	for (objl::Mesh objMesh : loader.LoadedMeshes)
	{
		Surface *surface = mesh->AddSurface((int32)objMesh.Vertices.size(), (int32)objMesh.Indices.size() / 3);
		surface->Opacity = objMesh.MeshMaterial.d;

		for (int32 v = 0; v < surface->VertexCount; v++)
		{
			surface->SetVertex(
				v,
				Vector3f(objMesh.Vertices[v].Position.X, objMesh.Vertices[v].Position.Y, objMesh.Vertices[v].Position.Z),
				Vector3f(objMesh.Vertices[v].Normal.X, objMesh.Vertices[v].Normal.Y, objMesh.Vertices[v].Normal.Z),
				Vector2f(objMesh.Vertices[v].TextureCoordinate.X, -objMesh.Vertices[v].TextureCoordinate.Y)
			);
		}

		for (int32 t = 0; t < surface->TriangleCount; t++)
		{
			surface->SetTriangle(t, objMesh.Indices[t * 3], objMesh.Indices[t * 3 + 1], objMesh.Indices[t * 3 + 2]);
		}

		// Texture path = directory of mesh + texture filename.
		const char *textureName = objMesh.MeshMaterial.map_Kd.empty() ? nullptr : objMesh.MeshMaterial.map_Kd.c_str();

		if (textureName)
		{
			char texturePath[MAX_PATH];
			lstrcpyA(texturePath, path);
			StrRChrA(texturePath, &texturePath[lstrlenA(texturePath)], '\\')[1] = '\0';
			lstrcatA(texturePath, textureName);

			if (GetFileAttributesA(texturePath) != INVALID_FILE_ATTRIBUTES)
			{
				surface->Texture = Texture::Load(texturePath);
			}
		}
	}

	return mesh;
}

Surface* Mesh::AddSurface(int32 vertexCount, int32 triangleCount)
{
	Surface *surface = new Surface(vertexCount, triangleCount);
	Surfaces.push_back(surface);
	return surface;
}

void Mesh::SetCullMode(CullMode cullMode)
{
	for (Surface *surface : Surfaces)
	{
		surface->CullMode = cullMode;
	}
}
void Mesh::SetTexture(const Texture *texture)
{
	for (Surface *surface : Surfaces)
	{
		surface->Texture = texture;
	}
}
void Mesh::SetTextureSize(float x, float y)
{
	SetTextureSize(Vector2f(x, y));
}
void Mesh::SetTextureSize(const Vector2f &size)
{
	for (Surface *surface : Surfaces)
	{
		surface->TextureSize = size;
	}
}
void Mesh::SetOpacity(float opacity)
{
	for (Surface *surface : Surfaces)
	{
		surface->Opacity = opacity;
	}
}

void Mesh::SetVertexColors(byte r, byte g, byte b)
{
	SetVertexColors(Color(r, g, b));
}
void Mesh::SetVertexColors(const Color &color)
{
	for (Surface *surface : Surfaces)
	{
		for (int32 i = 0; i < surface->VertexCount; i++)
		{
			surface->GetVertex(i)->Color = color;
		}
	}
}
void Mesh::SetRandomVertexColors()
{
	for (Surface *surface : Surfaces)
	{
		for (int32 i = 0; i < surface->VertexCount; i++)
		{
			surface->GetVertex(i)->Color = Color(Math::Random(0, 256), Math::Random(0, 256), Math::Random(0, 256));
		}
	}
}

Box3f Mesh::GetBoundingBox() const
{
	Box3f boundingBox = Box3f(Vector3f(NAN), Vector3f(NAN));

	for (Surface *surface : Surfaces)
	{
		for (int32 i = 0; i < surface->TriangleCount; i++)
		{
			for (int32 j = 0; j < 3; j++)
			{
				Vertex *vertex = surface->GetTriangleVertex(i, j);

				if (isnan(boundingBox.Min.X) || boundingBox.Min.X > vertex->Position.X) boundingBox.Min.X = vertex->Position.X;
				if (isnan(boundingBox.Min.Y) || boundingBox.Min.Y > vertex->Position.Y) boundingBox.Min.Y = vertex->Position.Y;
				if (isnan(boundingBox.Min.Z) || boundingBox.Min.Z > vertex->Position.Z) boundingBox.Min.Z = vertex->Position.Z;

				if (isnan(boundingBox.Max.X) || boundingBox.Max.X < vertex->Position.X) boundingBox.Max.X = vertex->Position.X;
				if (isnan(boundingBox.Max.Y) || boundingBox.Max.Y < vertex->Position.Y) boundingBox.Max.Y = vertex->Position.Y;
				if (isnan(boundingBox.Max.Z) || boundingBox.Max.Z < vertex->Position.Z) boundingBox.Max.Z = vertex->Position.Z;
			}
		}
	}

	return boundingBox;
}
void Mesh::FitToBoundingBox(const Box3f &boundingBox, bool uniform)
{
	Box3f originalBoundingBox = GetBoundingBox();
	Box3f newBoundingBox = boundingBox;

	if (uniform)
	{
		// Find longest side and scale other sides.
		Vector3f size = originalBoundingBox.Size;

		if (size.X > size.Y && size.X > size.Z)
		{
			float resizeY = size.Y / size.X;
			float resizeZ = size.Z / size.X;

			newBoundingBox.Min.Y *= resizeY;
			newBoundingBox.Min.Z *= resizeZ;
			newBoundingBox.Max.Y *= resizeY;
			newBoundingBox.Max.Z *= resizeZ;
		}
		else if (size.Y > size.X && size.Y > size.Z)
		{
			float resizeX = size.X / size.Y;
			float resizeZ = size.Z / size.Y;

			newBoundingBox.Min.X *= resizeX;
			newBoundingBox.Min.Z *= resizeZ;
			newBoundingBox.Max.X *= resizeX;
			newBoundingBox.Max.Z *= resizeZ;
		}
		else
		{
			float resizeX = size.X / size.Z;
			float resizeY = size.Y / size.Z;

			newBoundingBox.Min.X *= resizeX;
			newBoundingBox.Min.Y *= resizeY;
			newBoundingBox.Max.X *= resizeX;
			newBoundingBox.Max.Y *= resizeY;
		}
	}

	for (Surface *surface : Surfaces)
	{
		for (int32 i = 0; i < surface->VertexCount; i++)
		{
			Vertex *vertex = surface->GetVertex(i);
			vertex->Position = Math::Interpolate(vertex->Position, originalBoundingBox.Min, originalBoundingBox.Max, newBoundingBox.Min, newBoundingBox.Max);
		}
	}
}
void Mesh::TransformVertices(const Matrix4f &matrix)
{
	TransformVertices(matrix, true, true);
}
void Mesh::TransformVertices(const Matrix4f &matrix, bool positions, bool normals)
{
	for (Surface *surface : Surfaces)
	{
		for (int32 i = 0; i < surface->VertexCount; i++)
		{
			if (positions)
			{
				surface->GetVertex(i)->Position = matrix * surface->GetVertex(i)->Position;
			}

			if (normals)
			{
				surface->GetVertex(i)->Normals = matrix * surface->GetVertex(i)->Normals;
			}
		}
	}
}
void Mesh::NormalizeNormals()
{
	for (Surface *surface : Surfaces)
	{
		surface->NormalizeNormals();
	}
}
void Mesh::FlipTriangles()
{
	for (Surface *surface : Surfaces)
	{
		surface->FlipTriangles();
	}
}