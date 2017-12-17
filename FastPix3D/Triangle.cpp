#include "FastPix3D.h"

Triangle::Triangle()
{
	Vertex1 = NULL;
	Vertex2 = NULL;
	Vertex3 = NULL;
}
Triangle::Triangle(Vertex *vertex1, Vertex *vertex2, Vertex *vertex3)
{
	Vertex1 = vertex1;
	Vertex2 = vertex2;
	Vertex3 = vertex3;
}