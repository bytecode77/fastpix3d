class FASTPIX3D_API ProcessedVertex : public Vertex
{
public:
	Point ProjectedPosition;
	float R, G, B;

	ProcessedVertex();
	ProcessedVertex(const Vertex &vertex);
};