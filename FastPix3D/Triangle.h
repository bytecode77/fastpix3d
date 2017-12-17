struct FASTPIX3D_API Triangle
{
public:
	Vertex *Vertex1, *Vertex2, *Vertex3;

	Triangle();
	Triangle(Vertex *vertex1, Vertex *vertex2, Vertex *vertex3);
};