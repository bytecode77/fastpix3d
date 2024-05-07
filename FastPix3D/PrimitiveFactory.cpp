#include "PrimitiveFactory.h"
#include "Vector2f.h"
#include "Vector3f.h"

Mesh* PrimitiveFactory::Plane()
{
	return Plane(1);
}
Mesh* PrimitiveFactory::Plane(int32 segments)
{
	return Plane(segments, segments);
}
Mesh* PrimitiveFactory::Plane(int32 xSegments, int32 ySegments)
{
	Mesh *mesh = new Mesh();
	Surface *surface = mesh->AddSurface((xSegments + 1) * (ySegments + 1), xSegments * ySegments * 2);

	for (int32 y = 0, index = 0; y <= ySegments; y++)
	{
		for (int32 x = 0; x <= xSegments; x++)
		{
			float xf = -.5f + (float)x / xSegments;
			float yf = .5f - (float)y / ySegments;
			float u = (float)x / xSegments;
			float v = (float)y / ySegments;

			surface->SetVertex(index++, Vector3f(xf, 0, yf), Vector3f(0, 1, 0), Vector2f(-v, u));
		}
	}

	for (int32 y = 0, index = 0, triangleIndex = 0; y < ySegments; y++)
	{
		for (int32 x = 0; x < xSegments; x++)
		{
			surface->SetTriangle(triangleIndex++, index, index + 1, index + 1 + xSegments);
			surface->SetTriangle(triangleIndex++, index + 1, index + 2 + xSegments, index + 1 + xSegments);
			index++;
		}

		index++;
	}

	return mesh;
}
Mesh* PrimitiveFactory::Cube()
{
	return Cube(1);
}
Mesh* PrimitiveFactory::Cube(int32 segments)
{
	return Cube(segments, segments, segments);
}
Mesh* PrimitiveFactory::Cube(int32 xSegments, int32 ySegments, int32 zSegments)
{
	Mesh *mesh = new Mesh();

	for (int32 side = 0; side < 6; side++)
	{
		int32 sideSegmentsX;
		int32 sideSegmentsY;
		switch (side)
		{
			case 0:
				// Top
				sideSegmentsX = xSegments;
				sideSegmentsY = zSegments;
				break;
			case 1:
				// Bottom
				sideSegmentsX = zSegments;
				sideSegmentsY = xSegments;
				break;
			case 2:
				// Left
				sideSegmentsX = ySegments;
				sideSegmentsY = zSegments;
				break;
			case 3:
				// Right
				sideSegmentsX = zSegments;
				sideSegmentsY = ySegments;
				break;
			case 4:
				// Back
				sideSegmentsX = ySegments;
				sideSegmentsY = xSegments;
				break;
			case 5:
				// Front
				sideSegmentsX = xSegments;
				sideSegmentsY = ySegments;
				break;
		}

		Surface *surface = mesh->AddSurface((sideSegmentsX + 1) * (sideSegmentsY + 1), sideSegmentsX * sideSegmentsY * 2);

		for (int32 y = 0, index = 0; y <= sideSegmentsY; y++)
		{
			for (int32 x = 0; x <= sideSegmentsX; x++)
			{
				float xf = -.5f + (float)x / sideSegmentsX;
				float yf = .5f - (float)y / sideSegmentsY;
				float u = (float)x / sideSegmentsX;
				float v = (float)y / sideSegmentsY;

				switch (side)
				{
					case 0:
						// Top
						surface->SetVertex(index++, Vector3f(xf, .5f, yf), Vector3f(0, 1, 0), Vector2f(-v, u));
						break;
					case 1:
						// Bottom
						surface->SetVertex(index++, Vector3f(yf, -.5f, xf), Vector3f(0, -1, 0), Vector2f(u, v));
						break;
					case 2:
						// Left
						surface->SetVertex(index++, Vector3f(-.5f, xf, yf), Vector3f(-1, 0, 0), Vector2f(v, -u));
						break;
					case 3:
						// Right
						surface->SetVertex(index++, Vector3f(.5f, yf, xf), Vector3f(1, 0, 0), Vector2f(u, v));
						break;
					case 4:
						// Back
						surface->SetVertex(index++, Vector3f(xf, yf, -.5f), Vector3f(0, 0, -1), Vector2f(u, v));
						break;
					case 5:
						// Front
						surface->SetVertex(index++, Vector3f(yf, xf, .5f), Vector3f(0, 0, 1), Vector2f(v, -u));
						break;
				}
			}
		}

		for (int32 y = 0, index = 0, triangleIndex = 0; y < sideSegmentsY; y++)
		{
			for (int32 x = 0; x < sideSegmentsX; x++)
			{
				surface->SetTriangle(triangleIndex++, index, index + 1, index + 1 + sideSegmentsX);
				surface->SetTriangle(triangleIndex++, index + 1, index + 2 + sideSegmentsX, index + 1 + sideSegmentsX);
				index++;
			}

			index++;
		}
	}

	return mesh;
}
Mesh* PrimitiveFactory::Cylinder(int32 segments)
{
	return Cylinder(segments, 1, 1);
}
Mesh* PrimitiveFactory::Cylinder(int32 segments, float topRadius, float bottomRadius)
{
	Mesh *mesh = new Mesh();

	Surface *top = mesh->AddSurface(segments + 1, segments);
	Surface *bottom = mesh->AddSurface(segments + 1, segments);

	top->SetVertex(0, Vector3f(0, .5f, 0), Vector3f(0, 1, 0), Vector2f(.5f, .5f));
	bottom->SetVertex(0, Vector3f(0, -.5f, 0), Vector3f(0, -1, 0), Vector2f(.5f, .5f));

	for (int32 i = 0; i < segments; i++)
	{
		float angle = i * 360.0f / segments;
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		top->SetVertex(i + 1, Vector3f(c * .5f * topRadius, .5f, s * .5f * topRadius), Vector3f(0, 1, 0), Vector2f(s * .5f + .5f, c * .5f + .5f));
		bottom->SetVertex(i + 1, Vector3f(c * .5f * bottomRadius, -.5f, s * .5f * bottomRadius), Vector3f(0, -1, 0), Vector2f(s * .5f + .5f, .5f - c * .5f));

		top->SetTriangle(i, i + 1, 0, (i + 1) % segments + 1);
		bottom->SetTriangle(i, 0, i + 1, (i + 1) % segments + 1);
	}

	Surface *side = mesh->AddSurface((segments + 1) * 2, segments * 2);

	for (int32 i = 0; i <= segments; i++)
	{
		float angle = i * 360.0f / segments;
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		side->SetVertex(i, Vector3f(c * .5f * topRadius, .5f, s * .5f * topRadius), Vector3f(c, 0, s), Vector2f((float)i / segments + .5f, 0));
		side->SetVertex(i + segments + 1, Vector3f(c * .5f * bottomRadius, -.5f, s * .5f * bottomRadius), Vector3f(c, 0, s), Vector2f((float)i / segments + .5f, 1));
	}

	for (int32 i = 0; i < segments; i++)
	{
		side->SetTriangle(i, i, (i + 1) % (segments + 1), i + segments + 1);
		side->SetTriangle(i + segments, i + segments + 1, (i + 1) % (segments + 1), (i + 1) % (segments + 1) + segments + 1);
	}

	return mesh;
}
Mesh* PrimitiveFactory::Tube(int32 segments, float innerRadius)
{
	return Tube(segments, 1, innerRadius, 1, innerRadius);
}
Mesh* PrimitiveFactory::Tube(int32 segments, float topRadius, float topInnerRadius, float bottomRadius, float bottomInnerRadius)
{
	Mesh *mesh = new Mesh();

	Surface *top = mesh->AddSurface(segments * 2, segments * 2);
	Surface *bottom = mesh->AddSurface(segments * 2, segments * 2);

	for (int32 i = 0; i < segments; i++)
	{
		float angle = i * 360.0f / segments;
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		top->SetVertex(i, Vector3f(c * .5f * topRadius, .5f, s * .5f * topRadius), Vector3f(0, 1, 0), Vector2f(s * .5f + .5f, c * .5f + .5f));
		top->SetVertex(i + segments, Vector3f(c * .5f * topInnerRadius, .5f, s * .5f * topInnerRadius), Vector3f(0, 1, 0), Vector2f(s * .5f * topInnerRadius / topRadius + .5f, c * .5f * topInnerRadius / topRadius + .5f));

		bottom->SetVertex(i, Vector3f(c * .5f * bottomRadius, -.5f, s * .5f * bottomRadius), Vector3f(0, -1, 0), Vector2f(s * .5f + .5f, .5f - c * .5f));
		bottom->SetVertex(i + segments, Vector3f(c * .5f * bottomInnerRadius, -.5f, s * .5f * bottomInnerRadius), Vector3f(0, -1, 0), Vector2f(s * .5f * bottomInnerRadius / bottomRadius + .5f, .5f - c * .5f * bottomInnerRadius / bottomRadius));

		top->SetTriangle(i, i, i + segments, (i + 1) % segments);
		top->SetTriangle(i + segments, (i + 1) % segments, i + segments, (i + 1) % segments + segments);

		bottom->SetTriangle(i, i, (i + 1) % segments, i + segments);
		bottom->SetTriangle(i + segments, (i + 1) % segments, (i + 1) % segments + segments, i + segments);
	}

	Surface *side = mesh->AddSurface((segments + 1) * 2, segments * 2);
	Surface *inside = mesh->AddSurface((segments + 1) * 2, segments * 2);

	for (int32 i = 0; i <= segments; i++)
	{
		float angle = i * 360.0f / segments;
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		side->SetVertex(i, Vector3f(c * .5f * topRadius, .5f, s * .5f * topRadius), Vector3f(c, 0, s), Vector2f((float)i / segments + .5f, 0));
		side->SetVertex(i + segments + 1, Vector3f(c * .5f * bottomRadius, -.5f, s * .5f * bottomRadius), Vector3f(c, 0, s), Vector2f((float)i / segments + .5f, 1));

		inside->SetVertex(i, Vector3f(c * .5f * topInnerRadius, .5f, s * .5f * topInnerRadius), Vector3f(-c, 0, -s), Vector2f((float)i / segments + .5f, 0));
		inside->SetVertex(i + segments + 1, Vector3f(c * .5f * bottomInnerRadius, -.5f, s * .5f * bottomInnerRadius), Vector3f(-c, 0, -s), Vector2f((float)i / segments + .5f, 1));
	}

	for (int32 i = 0; i < segments; i++)
	{
		side->SetTriangle(i, i, (i + 1) % (segments + 1), i + segments + 1);
		side->SetTriangle(i + segments, i + segments + 1, (i + 1) % (segments + 1), (i + 1) % (segments + 1) + segments + 1);

		inside->SetTriangle(i, i, i + segments + 1, (i + 1) % (segments + 1));
		inside->SetTriangle(i + segments, i + segments + 1, (i + 1) % (segments + 1) + segments + 1, (i + 1) % (segments + 1));
	}

	return mesh;
}
Mesh* PrimitiveFactory::Cone(int32 segments)
{
	Mesh *mesh = new Mesh();

	Surface *bottom = mesh->AddSurface(segments + 1, segments);

	bottom->SetVertex(0, Vector3f(0, -.5f, 0), Vector3f(0, -1, 0), Vector2f(.5f, .5f));

	for (int32 i = 0; i < segments; i++)
	{
		float angle = i * 360.0f / segments;
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		bottom->SetVertex(i + 1, Vector3f(c * .5f, -.5f, s * .5f), Vector3f(0, -1, 0), Vector2f(s * .5f + .5f, .5f - c * .5f));
		bottom->SetTriangle(i, 0, i + 1, (i + 1) % segments + 1);
	}

	Surface *side = mesh->AddSurface((segments + 1) * 2, segments);

	for (int32 i = 0; i <= segments; i++)
	{
		float angle = i * 360.0f / segments;
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);

		side->SetVertex(i, Vector3f(0, .5f, 0), Vector3f(c, .5f, s).Normalize(), Vector2f((i + .5f) / segments + .5f, 0));
		side->SetVertex(i + segments + 1, Vector3f(c * .5f, -.5f, s * .5f), Vector3f(c, .5f, s).Normalize(), Vector2f((float)i / segments + .5f, 1));
	}

	for (int32 i = 0; i < segments; i++)
	{
		side->SetTriangle(i, i, i + 1 + segments + 1, i + segments + 1);
	}

	return mesh;
}
Mesh* PrimitiveFactory::Sphere(int32 xSegments, int32 ySegments)
{
	Mesh *mesh = new Mesh();

	Surface *surface = mesh->AddSurface((xSegments + 1) * (ySegments + 1), xSegments * ySegments * 2);

	for (int32 y = 0, index = 0; y <= ySegments; y++)
	{
		float yAngle = y * 180.0f / ySegments + 90;
		float sy = Math::Sin(yAngle);
		float cy = Math::Cos(yAngle);

		for (int32 x = 0; x <= xSegments; x++)
		{
			float xAngle = x * 360.0f / xSegments;
			float sx = Math::Sin(xAngle);
			float cx = Math::Cos(xAngle);

			surface->SetVertex(index++, Vector3f(cx * cy * .5f, sy * .5f, sx * cy * .5f), Vector3f(cx * cy, sy, sx * cy), Vector2f((float)x / xSegments, (float)y / ySegments));
		}
	}

	for (int32 y = 0, index = 0; y < ySegments; y++)
	{
		for (int32 x = 0; x < xSegments; x++)
		{
			int32 v00 = x + y * (xSegments + 1);
			int32 v10 = x + 1 + y * (xSegments + 1);
			int32 v01 = x + (y + 1) * (xSegments + 1);
			int32 v11 = x + 1 + (y + 1) * (xSegments + 1);

			surface->SetTriangle(index++, v00, v10, v01);
			surface->SetTriangle(index++, v01, v10, v11);
		}
	}

	return mesh;
}
Mesh* PrimitiveFactory::Torus(int32 xSegments, int32 ySegments, float innerRadius)
{
	Mesh *mesh = new Mesh();

	Surface *surface = mesh->AddSurface((xSegments + 1) * (ySegments + 1), xSegments * ySegments * 2);

	float outerRadius = .5f - innerRadius * .5f;
	innerRadius /= 2;

	for (int32 y = 0, index = 0; y <= ySegments; y++)
	{
		float yAngle = y * 360.0f / ySegments;
		float sy = Math::Sin(yAngle);
		float cy = Math::Cos(yAngle);

		for (int32 x = 0; x <= xSegments; x++)
		{
			float xAngle = x * 360.0f / xSegments;
			float sx = Math::Sin(xAngle);
			float cx = Math::Cos(xAngle);

			surface->SetVertex(index++, Vector3f(cx * outerRadius + cx * cy * innerRadius, sy * innerRadius, sx * outerRadius + sx * cy * innerRadius), Vector3f(cx * cy, sy, sx * cy), Vector2f((float)x / xSegments + .5f, .25f - (float)y / ySegments));
		}
	}

	for (int32 y = 0, index = 0; y < ySegments; y++)
	{
		for (int32 x = 0; x < xSegments; x++)
		{
			int32 v00 = x + y * (xSegments + 1);
			int32 v10 = x + 1 + y * (xSegments + 1);
			int32 v01 = x + (y + 1) * (xSegments + 1);
			int32 v11 = x + 1 + (y + 1) * (xSegments + 1);

			surface->SetTriangle(index++, v10, v00, v01);
			surface->SetTriangle(index++, v10, v01, v11);
		}
	}

	return mesh;
}