#include "FastPix3D.h"

ShadowVolume::ShadowVolume(Mesh *mesh)
{
	TriangleCount = mesh->getTriangleCount();
	Triangles = new ShadowTriangle[TriangleCount];

	for (int32 i = 0, count = 0; i < mesh->getSubsetCount(); i++)
	{
		Subset *subset = mesh->getSubset(i);
		Triangle *triangles = subset->getTriangleBuffer();
		for (int32 v = 0; v < subset->getTriangleCount(); v++)
		{
			ShadowTriangle *triangle = &Triangles[count++];
			triangle->Vertex1Position = triangles[v].Vertex1->Position;
			triangle->Vertex2Position = triangles[v].Vertex2->Position;
			triangle->Vertex3Position = triangles[v].Vertex3->Position;
			triangle->Edge1 = triangle->Edge2 = triangle->Edge3 = NULL;
		}
	}

	for (int32 a = 0; a < TriangleCount; a++)
	{
		ShadowTriangle *triangle1 = &Triangles[a];
		for (int32 b = a + 1; b < TriangleCount; b++)
		{
			ShadowTriangle *triangle2 = &Triangles[b];
			bool t1a_t2a = triangle1->Vertex1Position == triangle2->Vertex1Position;
			bool t1a_t2b = triangle1->Vertex1Position == triangle2->Vertex2Position;
			bool t1a_t2c = triangle1->Vertex1Position == triangle2->Vertex3Position;
			bool t1b_t2a = triangle1->Vertex2Position == triangle2->Vertex1Position;
			bool t1b_t2b = triangle1->Vertex2Position == triangle2->Vertex2Position;
			bool t1b_t2c = triangle1->Vertex2Position == triangle2->Vertex3Position;
			bool t1c_t2a = triangle1->Vertex3Position == triangle2->Vertex1Position;
			bool t1c_t2b = triangle1->Vertex3Position == triangle2->Vertex2Position;
			bool t1c_t2c = triangle1->Vertex3Position == triangle2->Vertex3Position;

			if (t1a_t2b && t1b_t2a)
			{
				triangle1->Edge1 = triangle2;
				triangle2->Edge1 = triangle1;
			}
			else if (t1b_t2b && t1c_t2a)
			{
				triangle1->Edge2 = triangle2;
				triangle2->Edge1 = triangle1;
			}
			else if (t1c_t2b && t1a_t2a)
			{
				triangle1->Edge3 = triangle2;
				triangle2->Edge1 = triangle1;
			}
			else if (t1a_t2c && t1b_t2b)
			{
				triangle1->Edge1 = triangle2;
				triangle2->Edge2 = triangle1;
			}
			else if (t1b_t2c && t1c_t2b)
			{
				triangle1->Edge2 = triangle2;
				triangle2->Edge2 = triangle1;
			}
			else if (t1c_t2c && t1a_t2b)
			{
				triangle1->Edge3 = triangle2;
				triangle2->Edge2 = triangle1;
			}
			else if (t1a_t2a && t1b_t2c)
			{
				triangle1->Edge1 = triangle2;
				triangle2->Edge3 = triangle1;
			}
			else if (t1b_t2a && t1c_t2c)
			{
				triangle1->Edge2 = triangle2;
				triangle2->Edge3 = triangle1;
			}
			else if (t1c_t2a && t1a_t2c)
			{
				triangle1->Edge3 = triangle2;
				triangle2->Edge3 = triangle1;
			}
		}
	}
}

int32 ShadowVolume::Draw(Light *light, Matrix modelSpace, float length)
{
	Matrix worldSpace = modelSpace * RenderStates::CameraSpace;
	int32 triangleCount = 0;

	for (int32 v = 0; v < TriangleCount; v++)
	{
		ShadowTriangle *triangle = &Triangles[v];
		triangle->Vertex1TransformedPosition = modelSpace * triangle->Vertex1Position;
		triangle->Vertex2TransformedPosition = modelSpace * triangle->Vertex2Position;
		triangle->Vertex3TransformedPosition = modelSpace * triangle->Vertex3Position;
		Vector3f normal = (triangle->Vertex1TransformedPosition - light->Position) * (triangle->Vertex3TransformedPosition - triangle->Vertex2TransformedPosition).CrossProduct(triangle->Vertex2TransformedPosition - triangle->Vertex1TransformedPosition);
		triangle->IsCulled = normal.X + normal.Y + normal.Z > 0;
	}

	for (int32 v = 0; v < TriangleCount; v++)
	{
		ShadowTriangle *triangle = &Triangles[v];
		if (!triangle->IsCulled)
		{
			Vector3f ray1 = triangle->Vertex1TransformedPosition + (triangle->Vertex1TransformedPosition - light->Position).Normalize() * length;
			Vector3f ray2 = triangle->Vertex2TransformedPosition + (triangle->Vertex2TransformedPosition - light->Position).Normalize() * length;
			Vector3f ray3 = triangle->Vertex3TransformedPosition + (triangle->Vertex3TransformedPosition - light->Position).Normalize() * length;
			if (triangle->Edge1 == NULL || triangle->Edge1->IsCulled)
			{
				Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex1TransformedPosition, ray2, ray1);
				Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex1TransformedPosition, triangle->Vertex2TransformedPosition, ray2);
				triangleCount += 2;
			}
			if (triangle->Edge2 == NULL || triangle->Edge2->IsCulled)
			{
				Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex2TransformedPosition, ray3, ray2);
				Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex2TransformedPosition, triangle->Vertex3TransformedPosition, ray3);
				triangleCount += 2;
			}
			if (triangle->Edge3 == NULL || triangle->Edge3->IsCulled)
			{
				Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex1TransformedPosition, ray1, ray3);
				Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex1TransformedPosition, ray3, triangle->Vertex3TransformedPosition);
				triangleCount += 2;
			}
			Drawer::DrawStencilTriangle(Matrix::Identity(), ray1, ray2, ray3);
			Drawer::DrawStencilTriangle(Matrix::Identity(), triangle->Vertex2TransformedPosition, triangle->Vertex1TransformedPosition, triangle->Vertex3TransformedPosition);
			triangleCount += 2;
		}
	}

	return triangleCount;
}