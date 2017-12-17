class FASTPIX3D_API Mesh
{
private:
	int32 SubsetCount;
	Subset **Subsets;
public:
	static Mesh* FromFile(string path);

	Mesh(int32 subsetCount);
	~Mesh();

	int32 getSubsetCount();
	int32 getVertexCount();
	int32 getTriangleCount();
	Subset* getSubset(int32 index);

	void Draw(Matrix modelSpace);
	void TransformVertices(Matrix matrix);
};