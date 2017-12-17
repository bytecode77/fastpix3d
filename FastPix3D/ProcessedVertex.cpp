#include "FastPix3D.h"

ProcessedVertex::ProcessedVertex()
{
}
ProcessedVertex::ProcessedVertex(const Vertex &vertex)
{
	Position = vertex.Position;
	Normals = vertex.Normals;
	TextureCoordinates = vertex.TextureCoordinates;
	R = float(VertexColor.R);
	G = float(VertexColor.G);
	B = float(VertexColor.B);
}