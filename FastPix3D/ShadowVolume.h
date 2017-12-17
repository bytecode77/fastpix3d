struct FASTPIX3D_API ShadowVolume
{
private:
	int TriangleCount;
	ShadowTriangle *Triangles;
public:
	ShadowVolume(Mesh *mesh);

	int32 Draw(Light *light, Matrix modelSpace, float length);
};