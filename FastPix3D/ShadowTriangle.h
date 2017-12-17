struct FASTPIX3D_API ShadowTriangle
{
private:
	Vector3f Vertex1Position, Vertex2Position, Vertex3Position;
	Vector3f Vertex1TransformedPosition, Vertex2TransformedPosition, Vertex3TransformedPosition;
	ShadowTriangle *Edge1, *Edge2, *Edge3;
	bool IsCulled;

	friend struct ShadowVolume;
};