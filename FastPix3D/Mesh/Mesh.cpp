#define CGLTF_IMPLEMENTATION
#include "Mesh.h"
#include "../Math/Math_.h"
#include <MeshLoaders/cgltf.h>
#include <MeshLoaders/ObjLoader.h>

Mesh::~Mesh()
{
	for (Surface *surface : Surfaces)
	{
		delete surface;
	}
}

Mesh* Mesh::Load(const char *path)
{
	const char *extension = PathFindExtensionA(path);

	if (!lstrcmpiA(extension, ".gltf") ||
		!lstrcmpiA(extension, ".glb"))
	{
		return LoadGltf(path);
	}
	else if (!lstrcmpiA(extension, ".obj"))
	{
		return LoadObj(path);
	}
	else
	{
		throw;
	}
}

Surface* Mesh::GetSurface(const char *textureFileName) const
{
	// textureFileName can be a filename with or without extension.
	// Only the filename part without extension is used to find the surface.

	const char *dot = StrChrA(textureFileName, '.');
	int32 length = dot ? (int32)(dot - textureFileName) : lstrlenA(textureFileName);

	for (Surface *surface : Surfaces)
	{
		if (surface->Texture)
		{
			const char *fileName = surface->Texture->FileName;
			if (fileName && !StrCmpNIA(textureFileName, fileName, length) && fileName[length] == '.')
			{
				return surface;
			}
		}
	}

	return nullptr;
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
	SetTextureSize(vfloat2(x, y));
}
void Mesh::SetTextureSize(const vfloat2 &size)
{
	for (Surface *surface : Surfaces)
	{
		surface->TextureSize = size;
	}
}
void Mesh::SetBlendMode(BlendMode blendMode)
{
	for (Surface *surface : Surfaces)
	{
		surface->BlendMode = blendMode;
	}
}
void Mesh::SetAlpha(float alpha)
{
	for (Surface *surface : Surfaces)
	{
		surface->Alpha = alpha;
	}
}
void Mesh::SetSpecular(float specularExponent, float specularIntensity)
{
	for (Surface *surface : Surfaces)
	{
		surface->SpecularExponent = specularExponent;
		surface->SpecularIntensity = specularIntensity;
	}
}
void Mesh::SetSpecularExponent(float specularExponent)
{
	for (Surface *surface : Surfaces)
	{
		surface->SpecularExponent = specularExponent;
	}
}
void Mesh::SetSpecularIntensity(float specularIntensity)
{
	for (Surface *surface : Surfaces)
	{
		surface->SpecularIntensity = specularIntensity;
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
	Box3f boundingBox = Box3f(vfloat3(NAN), vfloat3(NAN));

	for (const Surface *surface : Surfaces)
	{
		for (int32 i = 0; i < surface->VertexCount; i++)
		{
			Vertex *vertex = surface->GetVertex(i);

			if (isnan(boundingBox.Min.X) || boundingBox.Min.X > vertex->Position.X) boundingBox.Min.X = vertex->Position.X;
			if (isnan(boundingBox.Min.Y) || boundingBox.Min.Y > vertex->Position.Y) boundingBox.Min.Y = vertex->Position.Y;
			if (isnan(boundingBox.Min.Z) || boundingBox.Min.Z > vertex->Position.Z) boundingBox.Min.Z = vertex->Position.Z;

			if (isnan(boundingBox.Max.X) || boundingBox.Max.X < vertex->Position.X) boundingBox.Max.X = vertex->Position.X;
			if (isnan(boundingBox.Max.Y) || boundingBox.Max.Y < vertex->Position.Y) boundingBox.Max.Y = vertex->Position.Y;
			if (isnan(boundingBox.Max.Z) || boundingBox.Max.Z < vertex->Position.Z) boundingBox.Max.Z = vertex->Position.Z;
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
		vfloat3 size = originalBoundingBox.Size;
		vfloat3 reciprocalSize = size.Reciprocal;

		if (size.X > size.Y && size.X > size.Z)
		{
			float resizeY = size.Y * reciprocalSize.X;
			float resizeZ = size.Z * reciprocalSize.X;

			newBoundingBox.Min.Y *= resizeY;
			newBoundingBox.Min.Z *= resizeZ;
			newBoundingBox.Max.Y *= resizeY;
			newBoundingBox.Max.Z *= resizeZ;
		}
		else if (size.Y > size.X && size.Y > size.Z)
		{
			float resizeX = size.X * reciprocalSize.Y;
			float resizeZ = size.Z * reciprocalSize.Y;

			newBoundingBox.Min.X *= resizeX;
			newBoundingBox.Min.Z *= resizeZ;
			newBoundingBox.Max.X *= resizeX;
			newBoundingBox.Max.Z *= resizeZ;
		}
		else
		{
			float resizeX = size.X * reciprocalSize.Z;
			float resizeY = size.Y * reciprocalSize.Z;

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
void Mesh::FlipNormals()
{
	for (Surface *surface : Surfaces)
	{
		surface->FlipNormals();
	}
}
void Mesh::FlipTriangles()
{
	for (Surface *surface : Surfaces)
	{
		surface->FlipTriangles();
	}
}

Mesh* Mesh::LoadGltf(const char *path)
{
	cgltf_options options = { cgltf_file_type::cgltf_file_type_invalid };
	cgltf_data* data = nullptr;

	if (cgltf_parse_file(&options, path, &data) != cgltf_result::cgltf_result_success ||
		cgltf_load_buffers(&options, data, path) != cgltf_result::cgltf_result_success) throw;

	Mesh *mesh = new Mesh();

	for (int32 n = 0; n < data->nodes_count; n++)
	{
		const cgltf_node &node = data->nodes[n];
		if (!node.mesh) continue;

		Matrix4f matrix;
		cgltf_node_transform_world(&node, (float*)&matrix);
		matrix = matrix.Transpose();

		Matrix4f normalMatrix = matrix.ToNormalMatrix();

		for (int32 p = 0; p < node.mesh->primitives_count; p++)
		{
			const cgltf_primitive &primitive = node.mesh->primitives[p];

			if (primitive.type != cgltf_primitive_type::cgltf_primitive_type_triangles) continue;
			if (!primitive.indices || primitive.indices->count == 0 || primitive.indices->count % 3 != 0) throw;

			const cgltf_attribute *positionAttribute = nullptr;
			const cgltf_attribute *normalAttribute = nullptr;
			const cgltf_attribute *textureCoordinateAttribute = nullptr;
			const cgltf_attribute *colorAttribute = nullptr;

			for (int32 i = 0; i < primitive.attributes_count; i++)
			{
				switch (primitive.attributes[i].type)
				{
					case cgltf_attribute_type::cgltf_attribute_type_position:
						positionAttribute = &primitive.attributes[i];
						break;
					case cgltf_attribute_type::cgltf_attribute_type_normal:
						normalAttribute = &primitive.attributes[i];
						break;
					case cgltf_attribute_type::cgltf_attribute_type_texcoord:
						textureCoordinateAttribute = &primitive.attributes[i];
						break;
					case cgltf_attribute_type::cgltf_attribute_type_color:
						colorAttribute = &primitive.attributes[i];
						break;
				}
			}

			if (!positionAttribute) throw;

			Surface *surface = mesh->AddSurface((int32)positionAttribute->data->count, (int32)primitive.indices->count / 3);

			for (int32 i = 0; i < surface->VertexCount; i++)
			{
				vfloat3 position;
				vfloat3 normal;
				vfloat2 textureCoordinates;
				vfloat3 color = vfloat3(1);

				if (!cgltf_accessor_read_float(positionAttribute->data, i, (float*)&position, 3)) throw;
				if (normalAttribute && !cgltf_accessor_read_float(normalAttribute->data, i, (float*)&normal, 3)) throw;
				if (textureCoordinateAttribute && !cgltf_accessor_read_float(textureCoordinateAttribute->data, i, (float*)&textureCoordinates, 2)) throw;
				if (colorAttribute && !cgltf_accessor_read_float(colorAttribute->data, i, (float*)&color, 3)) throw;

				surface->SetVertex(
					i,
					matrix * position,
					(normalMatrix * normal).Normalize(),
					textureCoordinates,
					(Color)VectorMath::Clamp((vint3)(color * 255), 0, 255)
				);
			}

			for (int32 i = 0, j = 0; i < surface->TriangleCount; i++, j += 3)
			{
				surface->SetTriangle(
					i,
					(int32)cgltf_accessor_read_index(primitive.indices, j),
					(int32)cgltf_accessor_read_index(primitive.indices, j + 1),
					(int32)cgltf_accessor_read_index(primitive.indices, j + 2)
				);
			}

			if (primitive.material)
			{
				if (primitive.material->double_sided)
				{
					surface->CullMode = CullMode::None;
				}

				if (primitive.material->alpha_mode == cgltf_alpha_mode::cgltf_alpha_mode_blend)
				{
					surface->BlendMode = BlendMode::Alpha;
				}

				vfloat4 color = vfloat4(primitive.material->pbr_metallic_roughness.base_color_factor);
				surface->Alpha = color[3];

				if (color[0] != 1 || color[1] != 1 || color[2] != 1)
				{
					for (int32 i = 0; i < surface->VertexCount; i++)
					{
						surface->GetVertex(i)->Color *= color;
					}
				}

				float roughness = Math::Clamp(primitive.material->pbr_metallic_roughness.roughness_factor, 0.0f, 1.0f);
				if (roughness > 0)
				{
					roughness *= roughness;
					surface->SpecularExponent = (1 - roughness) * 30 + roughness * 100;
					surface->SpecularIntensity = Math::Clamp(primitive.material->pbr_metallic_roughness.metallic_factor, 0.0f, 1.0f);
					if (surface->SpecularIntensity == 0) surface->SpecularIntensity = .5f;
				}

				if (primitive.material->pbr_metallic_roughness.base_color_texture.texture)
				{
					const cgltf_image *materialTexture = primitive.material->pbr_metallic_roughness.base_color_texture.texture->image;

					if (materialTexture)
					{
						if (materialTexture->uri)
						{
							surface->Texture = LoadTexture(path, materialTexture->uri);
						}
						else if (materialTexture->buffer_view && materialTexture->buffer_view->buffer && materialTexture->buffer_view->buffer->data)
						{
							surface->Texture = Texture::FromMemory((byte*)materialTexture->buffer_view->buffer->data + materialTexture->buffer_view->offset, (int32)materialTexture->buffer_view->size);
						}
					}
				}
			}
		}
	}

	cgltf_free(data);
	return mesh;
}
Mesh* Mesh::LoadObj(const char *path)
{
	objl::Loader loader;
	if (!loader.LoadFile(path)) throw;

	Mesh *mesh = new Mesh();

	for (const objl::Mesh &objMesh : loader.LoadedMeshes)
	{
		Surface *surface = mesh->AddSurface((int32)objMesh.Vertices.size(), (int32)objMesh.Indices.size() / 3);
		surface->Alpha = objMesh.MeshMaterial.d;
		surface->SpecularExponent = objMesh.MeshMaterial.Ns;
		surface->SpecularIntensity = (objMesh.MeshMaterial.Ks.X + objMesh.MeshMaterial.Ks.Y + objMesh.MeshMaterial.Ks.Z) * (1.0f / 3.0f);

		for (int32 i = 0; i < surface->VertexCount; i++)
		{
			surface->SetVertex(
				i,
				vfloat3(objMesh.Vertices[i].Position.X, objMesh.Vertices[i].Position.Y, objMesh.Vertices[i].Position.Z),
				vfloat3(objMesh.Vertices[i].Normal.X, objMesh.Vertices[i].Normal.Y, objMesh.Vertices[i].Normal.Z).Normalize(),
				vfloat2(objMesh.Vertices[i].TextureCoordinate.X, -objMesh.Vertices[i].TextureCoordinate.Y)
			);
		}

		for (int32 i = 0, j = 0; i < surface->TriangleCount; i++, j += 3)
		{
			surface->SetTriangle(i, objMesh.Indices[j], objMesh.Indices[j + 1], objMesh.Indices[j + 2]);
		}

		surface->Texture = LoadTexture(path, objMesh.MeshMaterial.map_Kd.c_str());
	}

	return mesh;
}
Texture* Mesh::LoadTexture(const char *meshPath, const char *textureFileName)
{
	if (!textureFileName || lstrlenA(textureFileName) == 0) return nullptr;

	// Texture path = directory of mesh + texture filename.
	char texturePath[MAX_PATH];
	lstrcpyA(texturePath, meshPath);
	StrRChrA(texturePath, &texturePath[lstrlenA(texturePath)], '\\')[1] = '\0';
	lstrcatA(texturePath, textureFileName);

	if (GetFileAttributesA(texturePath) == INVALID_FILE_ATTRIBUTES) return nullptr;

	return Texture::FromFile(texturePath);
}