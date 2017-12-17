#include "FastPix3D.h"

Vertex::Vertex()
{
	Position = Vector3f();
	Normals = Vector3f();
	TextureCoordinates = Vector2f();
	VertexColor = Color(255, 255, 255);
}
Vertex::Vertex(Vector3f position)
{
	Position = position;
	Normals = Vector3f();
	TextureCoordinates = Vector2f();
	VertexColor = Color(255, 255, 255);
}
Vertex::Vertex(Vector3f position, Vector3f normals)
{
	Position = position;
	Normals = normals;
	TextureCoordinates = Vector2f();
	VertexColor = Color(255, 255, 255);
}
Vertex::Vertex(Vector3f position, Vector3f normals, Vector2f textureCoordinates)
{
	Position = position;
	Normals = normals;
	TextureCoordinates = textureCoordinates;
	VertexColor = Color(255, 255, 255);
}
Vertex::Vertex(Vector3f position, Vector3f normals, Vector2f textureCoordinates, Color vertexColor)
{
	Position = position;
	Normals = normals;
	TextureCoordinates = textureCoordinates;
	VertexColor = vertexColor;
}