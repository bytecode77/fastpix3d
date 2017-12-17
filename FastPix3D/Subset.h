class FASTPIX3D_API Subset
{
private:
	int32 VertexCount, TriangleCount;
	Vertex *Vertices;
	Triangle *Triangles;
	Texture *AppliedTexture;
public:
	Subset(int32 vertexCount, int32 triangleCount);
	~Subset();

	int32 getVertexCount();
	int32 getTriangleCount();
	Vertex* getVertexBuffer();
	Triangle* getTriangleBuffer();
	Texture* getTexture();

	friend class Mesh;
};