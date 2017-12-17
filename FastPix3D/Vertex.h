struct FASTPIX3D_API Vertex
{
public:
	Vector3f Position;
	Vector3f Normals;
	Vector2f TextureCoordinates;
	Color VertexColor;

	Vertex();
	Vertex(Vector3f position);
	Vertex(Vector3f position, Vector3f normals);
	Vertex(Vector3f position, Vector3f normals, Vector2f textureCoordinates);
	Vertex(Vector3f position, Vector3f normals, Vector2f textureCoordinates, Color vertexColor);
};