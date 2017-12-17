#include "FastPix3D.h"

Mesh* Mesh::FromFile(string path)
{
	File file = File::OpenRead(path);
	if (file.ReadString() != "FastPix3D_Mesh_1.0.2") throw;

	Mesh *mesh = new Mesh(file.ReadUInt16());

	for (int32 i = 0; i < mesh->SubsetCount; i++)
	{
		string texturePath = file.ReadString();
		int32 vertexCount = file.ReadUInt16(), triangleCount = file.ReadUInt16();

		Subset *subset = new Subset(vertexCount, triangleCount);
		if (texturePath != "")
		{
			texturePath = Path::Combine(Path::GetDirectoryName(path), texturePath);
			if (File::Exists(texturePath)) subset->AppliedTexture = new Texture(texturePath);
		}

		for (int32 a = 0; a < vertexCount; a++)
		{
			file.Read(&subset->Vertices[a], 4, 8);
			subset->Vertices[a].VertexColor = Color(file.ReadByte(), file.ReadByte(), file.ReadByte());
		}
		for (int32 a = 0; a < triangleCount; a++)
		{
			subset->Triangles[a].Vertex1 = &subset->Vertices[file.ReadUInt16()];
			subset->Triangles[a].Vertex2 = &subset->Vertices[file.ReadUInt16()];
			subset->Triangles[a].Vertex3 = &subset->Vertices[file.ReadUInt16()];
		}

		mesh->Subsets[i] = subset;
	}

	file.Close();
	return mesh;
}

Mesh::Mesh(int32 subsetCount)
{
	SubsetCount = subsetCount;
	Subsets = new Subset*[SubsetCount];
}
Mesh::~Mesh()
{
	for (int32 i = 0; i < SubsetCount; i++) delete Subsets[i];
	delete[] Subsets;
}

int32 Mesh::getSubsetCount()
{
	return SubsetCount;
}
int32 Mesh::getVertexCount()
{
	int32 count = 0;
	for (int32 i = 0; i < SubsetCount; i++) count += Subsets[i]->VertexCount;
	return count;
}
int32 Mesh::getTriangleCount()
{
	int32 count = 0;
	for (int32 i = 0; i < SubsetCount; i++) count += Subsets[i]->TriangleCount;
	return count;
}
Subset* Mesh::getSubset(int32 index)
{
	return Subsets[index];
}

void Mesh::Draw(Matrix modelSpace)
{
	for (int32 i = 0; i < SubsetCount; i++)
	{
		Subset *subset = Subsets[i];
		RenderStates::CurrentTexture = subset->AppliedTexture;
		for (int32 a = 0; a < subset->TriangleCount; a++)
		{
			Triangle triangle = subset->Triangles[a];
			Drawer::DrawTriangle(modelSpace, *triangle.Vertex1, *triangle.Vertex2, *triangle.Vertex3);
		}
	}
}
void Mesh::TransformVertices(Matrix matrix)
{
	for (int32 i = 0; i < SubsetCount; i++)
	{
		Subset *subset = Subsets[i];
		for (int32 a = 0; a < subset->VertexCount; a++)
		{
			subset->Vertices[a].Position = matrix * subset->Vertices[a].Position;
		}
	}
}