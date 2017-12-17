#include "FastPix3D.h"

bool Drawer::IsStencilTriangle;
bool Drawer::IsLastTriangleCulled;
float Drawer::Dz;
float Drawer::Du;
float Drawer::Dv;
float Drawer::Dr;
float Drawer::Dg;
float Drawer::Db;
float Drawer::Dz16;
float Drawer::Du16;
float Drawer::Dv16;
float Drawer::Dr16;
float Drawer::Dg16;
float Drawer::Db16;

void Drawer::Initialize()
{
	IsStencilTriangle = false;
}

void Drawer::DrawTriangle(Matrix modelSpace, Vertex vertex1, Vertex vertex2, Vertex vertex3)
{
	Matrix worldSpace = modelSpace * RenderStates::CameraSpace;
	ProcessedVertex v1 = (ProcessedVertex)vertex1;
	ProcessedVertex v2 = (ProcessedVertex)vertex2;
	ProcessedVertex v3 = (ProcessedVertex)vertex3;

	v1.Position = worldSpace * v1.Position;
	v2.Position = worldSpace * v2.Position;
	v3.Position = worldSpace * v3.Position;

	Vector3f normal = v1.Position * (v3.Position - v2.Position).CrossProduct(v2.Position - v1.Position);
	IsLastTriangleCulled = normal.X + normal.Y + normal.Z < 0;
	if (RenderStates::CullMode == CullMode::Back && IsLastTriangleCulled || RenderStates::CullMode == CullMode::Front && !IsLastTriangleCulled)
	{
		return;
	}

	if (!IsStencilTriangle)
	{
		Vector3f vertex1Normals = worldSpace.MultiplyRotationOnly(v1.Normals);
		Vector3f vertex2Normals = worldSpace.MultiplyRotationOnly(v2.Normals);
		Vector3f vertex3Normals = worldSpace.MultiplyRotationOnly(v3.Normals);

		float r1 = float(Light::AmbientColor.R);
		float g1 = float(Light::AmbientColor.G);
		float b1 = float(Light::AmbientColor.B);
		float r2 = float(Light::AmbientColor.R);
		float g2 = float(Light::AmbientColor.G);
		float b2 = float(Light::AmbientColor.B);
		float r3 = float(Light::AmbientColor.R);
		float g3 = float(Light::AmbientColor.G);
		float b3 = float(Light::AmbientColor.B);

		if (RenderStates::EnableLights)
		{
			for (int32 i = 0; i < 8; i++)
			{
				Light light = *RenderStates::Lights[i];

				if (light.Enabled)
				{
					Vector3f lightVector, position, distance;
					float intensity, diffuse;

					switch (light.Type)
					{
						case LightType::Directional:
							lightVector = RenderStates::CameraSpace.MultiplyRotationOnly(Matrix::RotateXYZ(light.Rotation) * Vector3f(0, 0, -1));
							intensity = light.Intensity * lightVector.DotProduct(lightVector);

							if ((diffuse = lightVector.DotProduct(vertex1Normals) * intensity) > 0)
							{
								r1 += light.DiffuseColor.R * diffuse;
								g1 += light.DiffuseColor.G * diffuse;
								b1 += light.DiffuseColor.B * diffuse;
							}
							if ((diffuse = lightVector.DotProduct(vertex2Normals) * intensity) > 0)
							{
								r2 += light.DiffuseColor.R * diffuse;
								g2 += light.DiffuseColor.G * diffuse;
								b2 += light.DiffuseColor.B * diffuse;
							}
							if ((diffuse = lightVector.DotProduct(vertex3Normals) * intensity) > 0)
							{
								r3 += light.DiffuseColor.R * diffuse;
								g3 += light.DiffuseColor.G * diffuse;
								b3 += light.DiffuseColor.B * diffuse;
							}
							break;
						case LightType::Point:
							position = RenderStates::CameraSpace * light.Position;

							distance = position - v1.Position;
							if ((diffuse = distance.DotProduct(vertex1Normals) * light.Intensity / distance.DotProduct(distance)) > 0)
							{
								r1 += light.DiffuseColor.R * diffuse;
								g1 += light.DiffuseColor.G * diffuse;
								b1 += light.DiffuseColor.B * diffuse;
							}
							distance = position - v2.Position;
							if ((diffuse = distance.DotProduct(vertex2Normals) * light.Intensity / distance.DotProduct(distance)) > 0)
							{
								r2 += light.DiffuseColor.R * diffuse;
								g2 += light.DiffuseColor.G * diffuse;
								b2 += light.DiffuseColor.B * diffuse;
							}
							distance = position - v3.Position;
							if ((diffuse = distance.DotProduct(vertex3Normals) * light.Intensity / distance.DotProduct(distance)) > 0)
							{
								r3 += light.DiffuseColor.R * diffuse;
								g3 += light.DiffuseColor.G * diffuse;
								b3 += light.DiffuseColor.B * diffuse;
							}
							break;
						case LightType::Spot:
							lightVector = RenderStates::CameraSpace.MultiplyRotationOnly(Matrix::RotateXYZ(light.Rotation) * Vector3f(0, 0, -1));
							position = RenderStates::CameraSpace * light.Position;

							distance = position - v1.Position;
							if ((diffuse = distance.DotProduct(vertex1Normals) * light.Intensity / distance.DotProduct(distance) * MathUtility::Interpolate(acos(distance.Normalize().DotProduct(lightVector)), -1, 1, 2, 0)) > 0)
							{
								r1 += light.DiffuseColor.R * diffuse;
								g1 += light.DiffuseColor.G * diffuse;
								b1 += light.DiffuseColor.B * diffuse;
							}
							distance = position - v2.Position;
							if ((diffuse = distance.DotProduct(vertex2Normals) * light.Intensity / distance.DotProduct(distance) * MathUtility::Interpolate(acos(distance.Normalize().DotProduct(lightVector)), -1, 1, 2, 0)) > 0)
							{
								r2 += light.DiffuseColor.R * diffuse;
								g2 += light.DiffuseColor.G * diffuse;
								b2 += light.DiffuseColor.B * diffuse;
							}
							distance = position - v3.Position;
							if ((diffuse = distance.DotProduct(vertex3Normals) * light.Intensity / distance.DotProduct(distance) * MathUtility::Interpolate(acos(distance.Normalize().DotProduct(lightVector)), -1, 1, 2, 0)) > 0)
							{
								r3 += light.DiffuseColor.R * diffuse;
								g3 += light.DiffuseColor.G * diffuse;
								b3 += light.DiffuseColor.B * diffuse;
							}
							break;
					}
				}
			}
		}
		else
		{
			r1 = g1 = b1 = 255;
			r2 = g2 = b2 = 255;
			r3 = g3 = b3 = 255;
		}

		v1.R *= r1 > 256 ? 1 : r1 / 256;
		v1.G *= g1 > 256 ? 1 : g1 / 256;
		v1.B *= b1 > 256 ? 1 : b1 / 256;
		v2.R *= r2 > 256 ? 1 : r2 / 256;
		v2.G *= g2 > 256 ? 1 : g2 / 256;
		v2.B *= b2 > 256 ? 1 : b2 / 256;
		v3.R *= r3 > 256 ? 1 : r3 / 256;
		v3.G *= g3 > 256 ? 1 : g3 / 256;
		v3.B *= b3 > 256 ? 1 : b3 / 256;
	}

	float d = 1 / RenderStates::ClipNear;
	v1.Position *= d;
	v2.Position *= d;
	v3.Position *= d;

	bool vertex1Visible = v1.Position.Z > 1;
	bool vertex2Visible = v2.Position.Z > 1;
	bool vertex3Visible = v3.Position.Z > 1;

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		DrawClippedTriangle(v1, v2, v3);
	}
	else if (vertex1Visible || vertex2Visible || vertex3Visible)
	{
		ProcessedVertex v12, v23, v31;

		v12.Position.X = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Position.X, v2.Position.X);
		v12.Position.Y = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Position.Y, v2.Position.Y);
		v12.Position.Z = 1;
		v23.Position.X = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Position.X, v3.Position.X);
		v23.Position.Y = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Position.Y, v3.Position.Y);
		v23.Position.Z = 1;
		v31.Position.X = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Position.X, v1.Position.X);
		v31.Position.Y = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Position.Y, v1.Position.Y);
		v31.Position.Z = 1;

		if (!IsStencilTriangle)
		{
			v12.TextureCoordinates.X = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.TextureCoordinates.X, v2.TextureCoordinates.X);
			v12.TextureCoordinates.Y = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.TextureCoordinates.Y, v2.TextureCoordinates.Y);
			v12.R = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.R, v2.R);
			v12.G = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.G, v2.G);
			v12.B = MathUtility::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.B, v2.B);
			v23.TextureCoordinates.X = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.TextureCoordinates.X, v3.TextureCoordinates.X);
			v23.TextureCoordinates.Y = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.TextureCoordinates.Y, v3.TextureCoordinates.Y);
			v23.R = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.R, v3.R);
			v23.G = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.G, v3.G);
			v23.B = MathUtility::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.B, v3.B);
			v31.TextureCoordinates.X = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.TextureCoordinates.X, v1.TextureCoordinates.X);
			v31.TextureCoordinates.Y = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.TextureCoordinates.Y, v1.TextureCoordinates.Y);
			v31.R = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.R, v1.R);
			v31.G = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.G, v1.G);
			v31.B = MathUtility::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.B, v1.B);
		}

		if (vertex1Visible && vertex2Visible)
		{
			DrawClippedTriangle(v31, v1, v23);
			DrawClippedTriangle(v1, v2, v23);
		}
		else if (vertex2Visible && vertex3Visible)
		{
			DrawClippedTriangle(v3, v31, v2);
			DrawClippedTriangle(v12, v2, v31);
		}
		else if (vertex1Visible && vertex3Visible)
		{
			DrawClippedTriangle(v1, v12, v23);
			DrawClippedTriangle(v3, v1, v23);
		}
		else if (vertex1Visible)
		{
			DrawClippedTriangle(v1, v12, v31);
		}
		else if (vertex2Visible)
		{
			DrawClippedTriangle(v2, v23, v12);
		}
		else if (vertex3Visible)
		{
			DrawClippedTriangle(v3, v31, v23);
		}
	}
}
void Drawer::DrawStencilTriangle(Matrix modelSpace, Vector3f vertex1Position, Vector3f vertex2Position, Vector3f vertex3Position)
{
	IsStencilTriangle = true;
	DrawTriangle(modelSpace, Vertex(vertex1Position), Vertex(vertex2Position), Vertex(vertex3Position));
	IsStencilTriangle = false;
}
void Drawer::DrawClippedTriangle(ProcessedVertex vertex1, ProcessedVertex vertex2, ProcessedVertex vertex3)
{
	int32 width2 = Device::Width >> 1, height2 = Device::Height >> 1;
	float d = width2 * RenderStates::Zoom;
	vertex1.ProjectedPosition.X = width2 + int32(vertex1.Position.X * d / vertex1.Position.Z);
	vertex1.ProjectedPosition.Y = height2 - int32(vertex1.Position.Y * d / vertex1.Position.Z);
	vertex2.ProjectedPosition.X = width2 + int32(vertex2.Position.X * d / vertex2.Position.Z);
	vertex2.ProjectedPosition.Y = height2 - int32(vertex2.Position.Y * d / vertex2.Position.Z);
	vertex3.ProjectedPosition.X = width2 + int32(vertex3.Position.X * d / vertex3.Position.Z);
	vertex3.ProjectedPosition.Y = height2 - int32(vertex3.Position.Y * d / vertex3.Position.Z);

	if (vertex1.ProjectedPosition.X < 0 && vertex2.ProjectedPosition.X < 0 && vertex3.ProjectedPosition.X < 0 ||
		vertex1.ProjectedPosition.Y < 0 && vertex2.ProjectedPosition.Y < 0 && vertex3.ProjectedPosition.Y < 0 ||
		vertex1.ProjectedPosition.X >= Device::Width && vertex2.ProjectedPosition.X >= Device::Width && vertex3.ProjectedPosition.X >= Device::Width ||
		vertex1.ProjectedPosition.Y >= Device::Height && vertex2.ProjectedPosition.Y >= Device::Height && vertex3.ProjectedPosition.Y >= Device::Height) return;

	if (vertex1.ProjectedPosition.Y > vertex2.ProjectedPosition.Y)
	{
		swap(vertex1, vertex2);
	}
	if (vertex2.ProjectedPosition.Y > vertex3.ProjectedPosition.Y)
	{
		swap(vertex2, vertex3);
		if (vertex1.ProjectedPosition.Y > vertex2.ProjectedPosition.Y)
		{
			swap(vertex1, vertex2);
		}
	}

	vertex1.Position.Z = 1 / vertex1.Position.Z;
	vertex2.Position.Z = 1 / vertex2.Position.Z;
	vertex3.Position.Z = 1 / vertex3.Position.Z;

	if (!IsStencilTriangle)
	{
		if (vertex1.TextureCoordinates.X < 0 || vertex2.TextureCoordinates.X < 0 || vertex3.TextureCoordinates.X < 0)
		{
			float dif = floor(1 - min(vertex1.TextureCoordinates.X, min(vertex2.TextureCoordinates.X, vertex3.TextureCoordinates.X)));
			vertex1.TextureCoordinates.X += dif;
			vertex2.TextureCoordinates.X += dif;
			vertex3.TextureCoordinates.X += dif;
		}
		if (vertex1.TextureCoordinates.Y < 0 || vertex2.TextureCoordinates.Y < 0 || vertex3.TextureCoordinates.Y < 0)
		{
			float dif = floor(1 - min(vertex1.TextureCoordinates.Y, min(vertex2.TextureCoordinates.Y, vertex3.TextureCoordinates.Y)));
			vertex1.TextureCoordinates.Y += dif;
			vertex2.TextureCoordinates.Y += dif;
			vertex3.TextureCoordinates.Y += dif;
		}

		vertex1.TextureCoordinates.X *= RenderStates::CurrentTexture->Width * vertex1.Position.Z;
		vertex1.TextureCoordinates.Y *= RenderStates::CurrentTexture->Height * vertex1.Position.Z;
		vertex1.R *= vertex1.Position.Z;
		vertex1.G *= vertex1.Position.Z;
		vertex1.B *= vertex1.Position.Z;
		vertex2.TextureCoordinates.X *= RenderStates::CurrentTexture->Width * vertex2.Position.Z;
		vertex2.TextureCoordinates.Y *= RenderStates::CurrentTexture->Height * vertex2.Position.Z;
		vertex2.R *= vertex2.Position.Z;
		vertex2.G *= vertex2.Position.Z;
		vertex2.B *= vertex2.Position.Z;
		vertex3.TextureCoordinates.X *= RenderStates::CurrentTexture->Width * vertex3.Position.Z;
		vertex3.TextureCoordinates.Y *= RenderStates::CurrentTexture->Height * vertex3.Position.Z;
		vertex3.R *= vertex3.Position.Z;
		vertex3.G *= vertex3.Position.Z;
		vertex3.B *= vertex3.Position.Z;
	}

	int32 v1y = min(max(vertex1.ProjectedPosition.Y, 0), Device::Height - 1);
	int32 v2y = min(max(vertex2.ProjectedPosition.Y, 0), Device::Height - 1);
	int32 v3y = min(max(vertex3.ProjectedPosition.Y, 0), Device::Height - 1);

	if (vertex1.ProjectedPosition.Y < Device::Height && vertex2.ProjectedPosition.Y >= 0 || vertex2.ProjectedPosition.Y < Device::Height && vertex3.ProjectedPosition.Y >= 0)
	{
		float v13u, v13v, v13r, v13g, v13b;
		float d = (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y) * 1.f / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y);
		float v13x = vertex1.ProjectedPosition.X + (vertex3.ProjectedPosition.X - vertex1.ProjectedPosition.X) * d;
		float v13z = vertex1.Position.Z + (vertex3.Position.Z - vertex1.Position.Z) * d;
		if (!IsStencilTriangle)
		{
			v13u = vertex1.TextureCoordinates.X + (vertex3.TextureCoordinates.X - vertex1.TextureCoordinates.X) * d;
			v13v = vertex1.TextureCoordinates.Y + (vertex3.TextureCoordinates.Y - vertex1.TextureCoordinates.Y) * d;
			v13r = vertex1.R + (vertex3.R - vertex1.R) * d;
			v13g = vertex1.G + (vertex3.G - vertex1.G) * d;
			v13b = vertex1.B + (vertex3.B - vertex1.B) * d;
		}

		d = 1.f / (v13x - vertex2.ProjectedPosition.X);
		Dz = (v13z - vertex2.Position.Z) * d;
		Dz16 = Dz * Subdiv1;

		if (IsStencilTriangle)
		{
			if (vertex1.ProjectedPosition.Y < Device::Height && vertex2.ProjectedPosition.Y >= 0)
			{
				for (int32 y = v1y; y < v2y; y++)
				{
					DrawStencilScanline(
						y,
						vertex1.ProjectedPosition.X + (y - vertex1.ProjectedPosition.Y) * (vertex2.ProjectedPosition.X - vertex1.ProjectedPosition.X) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.Position.Z + (y - vertex1.ProjectedPosition.Y) * (vertex2.Position.Z - vertex1.Position.Z) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.ProjectedPosition.X + (y - vertex1.ProjectedPosition.Y) * (vertex3.ProjectedPosition.X - vertex1.ProjectedPosition.X) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.Position.Z + (y - vertex1.ProjectedPosition.Y) * (vertex3.Position.Z - vertex1.Position.Z) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y)
						);
				}
			}
			if (vertex2.ProjectedPosition.Y < Device::Height && vertex3.ProjectedPosition.Y >= 0)
			{
				for (int32 y = v2y; y < v3y; y++)
				{
					DrawStencilScanline(
						y,
						vertex2.ProjectedPosition.X + (y - vertex2.ProjectedPosition.Y) * (vertex3.ProjectedPosition.X - vertex2.ProjectedPosition.X) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.Position.Z + (y - vertex2.ProjectedPosition.Y) * (vertex3.Position.Z - vertex2.Position.Z) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex1.ProjectedPosition.X + (y - vertex1.ProjectedPosition.Y) * (vertex3.ProjectedPosition.X - vertex1.ProjectedPosition.X) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.Position.Z + (y - vertex1.ProjectedPosition.Y) * (vertex3.Position.Z - vertex1.Position.Z) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y)
						);
				}
			}
		}
		else
		{
			Du = (v13u - vertex2.TextureCoordinates.X) * d;
			Dv = (v13v - vertex2.TextureCoordinates.Y) * d;
			Dr = (v13r - vertex2.R) * d;
			Dg = (v13g - vertex2.G) * d;
			Db = (v13b - vertex2.B) * d;
			Du16 = Du * Subdiv1;
			Dv16 = Dv * Subdiv1;
			Dr16 = Dr * Subdiv1;
			Dg16 = Dg * Subdiv1;
			Db16 = Db * Subdiv1;

			if (vertex1.ProjectedPosition.Y < Device::Height && vertex2.ProjectedPosition.Y >= 0)
			{
				for (int32 y = v1y; y < v2y; y++)
				{
					DrawScanline(
						y,
						vertex1.ProjectedPosition.X + (y - vertex1.ProjectedPosition.Y) * (vertex2.ProjectedPosition.X - vertex1.ProjectedPosition.X) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.Position.Z + (y - vertex1.ProjectedPosition.Y) * (vertex2.Position.Z - vertex1.Position.Z) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.TextureCoordinates.X + (y - vertex1.ProjectedPosition.Y) * (vertex2.TextureCoordinates.X - vertex1.TextureCoordinates.X) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.TextureCoordinates.Y + (y - vertex1.ProjectedPosition.Y) * (vertex2.TextureCoordinates.Y - vertex1.TextureCoordinates.Y) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.R + (y - vertex1.ProjectedPosition.Y) * (vertex2.R - vertex1.R) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.G + (y - vertex1.ProjectedPosition.Y) * (vertex2.G - vertex1.G) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.B + (y - vertex1.ProjectedPosition.Y) * (vertex2.B - vertex1.B) / (vertex2.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.ProjectedPosition.X + (y - vertex1.ProjectedPosition.Y) * (vertex3.ProjectedPosition.X - vertex1.ProjectedPosition.X) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.Position.Z + (y - vertex1.ProjectedPosition.Y) * (vertex3.Position.Z - vertex1.Position.Z) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.TextureCoordinates.X + (y - vertex1.ProjectedPosition.Y) * (vertex3.TextureCoordinates.X - vertex1.TextureCoordinates.X) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.TextureCoordinates.Y + (y - vertex1.ProjectedPosition.Y) * (vertex3.TextureCoordinates.Y - vertex1.TextureCoordinates.Y) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.R + (y - vertex1.ProjectedPosition.Y) * (vertex3.R - vertex1.R) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.G + (y - vertex1.ProjectedPosition.Y) * (vertex3.G - vertex1.G) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.B + (y - vertex1.ProjectedPosition.Y) * (vertex3.B - vertex1.B) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y)
						);
				}
			}
			if (vertex2.ProjectedPosition.Y < Device::Height && vertex3.ProjectedPosition.Y >= 0)
			{
				for (int32 y = v2y; y < v3y; y++)
				{
					DrawScanline(
						y,
						vertex2.ProjectedPosition.X + (y - vertex2.ProjectedPosition.Y) * (vertex3.ProjectedPosition.X - vertex2.ProjectedPosition.X) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.Position.Z + (y - vertex2.ProjectedPosition.Y) * (vertex3.Position.Z - vertex2.Position.Z) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.TextureCoordinates.X + (y - vertex2.ProjectedPosition.Y) * (vertex3.TextureCoordinates.X - vertex2.TextureCoordinates.X) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.TextureCoordinates.Y + (y - vertex2.ProjectedPosition.Y) * (vertex3.TextureCoordinates.Y - vertex2.TextureCoordinates.Y) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.R + (y - vertex2.ProjectedPosition.Y) * (vertex3.R - vertex2.R) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.G + (y - vertex2.ProjectedPosition.Y) * (vertex3.G - vertex2.G) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex2.B + (y - vertex2.ProjectedPosition.Y) * (vertex3.B - vertex2.B) / (vertex3.ProjectedPosition.Y - vertex2.ProjectedPosition.Y),
						vertex1.ProjectedPosition.X + (y - vertex1.ProjectedPosition.Y) * (vertex3.ProjectedPosition.X - vertex1.ProjectedPosition.X) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.Position.Z + (y - vertex1.ProjectedPosition.Y) * (vertex3.Position.Z - vertex1.Position.Z) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.TextureCoordinates.X + (y - vertex1.ProjectedPosition.Y) * (vertex3.TextureCoordinates.X - vertex1.TextureCoordinates.X) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.TextureCoordinates.Y + (y - vertex1.ProjectedPosition.Y) * (vertex3.TextureCoordinates.Y - vertex1.TextureCoordinates.Y) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.R + (y - vertex1.ProjectedPosition.Y) * (vertex3.R - vertex1.R) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.G + (y - vertex1.ProjectedPosition.Y) * (vertex3.G - vertex1.G) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y),
						vertex1.B + (y - vertex1.ProjectedPosition.Y) * (vertex3.B - vertex1.B) / (vertex3.ProjectedPosition.Y - vertex1.ProjectedPosition.Y)
						);
				}
			}
		}
	}
}
void Drawer::DrawScanline(int32 y, int32 v1x, float v1z, float v1u, float v1v, float v1r, float v1g, float v1b, int32 v2x, float v2z, float v2u, float v2v, float v2r, float v2g, float v2b)
{
	if (v1x > v2x)
	{
		MathUtility::Swap(v1x, v2x);
		MathUtility::Swap(v1z, v2z);
		MathUtility::Swap(v1u, v2u);
		MathUtility::Swap(v1v, v2v);
		MathUtility::Swap(v1r, v2r);
		MathUtility::Swap(v1g, v2g);
		MathUtility::Swap(v1b, v2b);
	}
	if (v2x < 0 || v1x >= Device::Width) return;
	if (v1x < 0)
	{
		float d = v1x * 1.f / (v2x - v1x);
		v1z -= (v2z - v1z) * d;
		v1u -= (v2u - v1u) * d;
		v1v -= (v2v - v1v) * d;
		v1r -= (v2r - v1r) * d;
		v1g -= (v2g - v1g) * d;
		v1b -= (v2b - v1b) * d;
		v1x = 0;
	}
	if (v2x >= Device::Width) v2x = Device::Width - 1;

	int32 *backBuffer = &Device::BackBuffer[v1x + y * Device::Width];
	int16 *depthBuffer = &Device::DepthBuffer[v1x + y * Device::Width];
	sbyte *stencilBuffer = &Device::StencilBuffer[v1x + y * Device::Width];
	int32 *textureBuffer = RenderStates::CurrentTexture->Buffer;
	int32 textureWidthExponent = RenderStates::CurrentTexture->WidthExponent;

	float z = v1z, u = v1u, v = v1v, r = v1r, g = v1g, b = v1b;
	int32 tw = RenderStates::CurrentTexture->Width - 1;
	int32 th = RenderStates::CurrentTexture->Height - 1;
	int32 twe = RenderStates::CurrentTexture->WidthExponent;

	int32 subdivs = ((v2x - v1x) >> SubdivExponent) + 1;
	for (int32 i = 0; i < subdivs; i++)
	{
		int32 pixels = i < subdivs - 1 ? Subdiv1 : (v2x - v1x) & SubdivModulo;
		float z1 = 1 / z, z2 = 1 / (z + Dz16);
		float su = u * z1;
		float sv = v * z1;
		float sr = r * z1;
		float sg = g * z1;
		float sb = b * z1;
		float du = ((u + Du16) * z2 - su) * InvertedSubdiv1;
		float dv = ((v + Dv16) * z2 - sv) * InvertedSubdiv1;
		float dr = ((r + Dr16) * z2 - sr) * InvertedSubdiv1;
		float dg = ((g + Dg16) * z2 - sg) * InvertedSubdiv1;
		float db = ((b + Db16) * z2 - sb) * InvertedSubdiv1;
		for (int32 i = 0; i < pixels; i++)
		{
			int16 depth = int16(z * 65536);
			if (*depthBuffer <= depth && (!RenderStates::EnableStencilMask || *stencilBuffer))
			{
				byte *color = (byte*)&textureBuffer[(int32(su) & tw) | ((int32(sv) & th) << twe)];
				if (*(int32*)color != TextureTransparencyKey)
				{
					*backBuffer = ((color[0] * int32(sr)) >> 8) << 16 | ((color[1] * int32(sg)) >> 8) << 8 | (color[2] * int32(sb)) >> 8;
					if (RenderStates::EnableZWrites) *depthBuffer = depth;
				}
			}
			backBuffer++;
			depthBuffer++;
			stencilBuffer++;
			z += Dz;
			su += du;
			sv += dv;
			sr += dr;
			sg += dg;
			sb += db;
		}
		u += Du16;
		v += Dv16;
		r += Dr16;
		g += Dg16;
		b += Db16;
	}
}
void Drawer::DrawStencilScanline(int32 y, int32 v1x, float v1z, int32 v2x, float v2z)
{
	if (v1x > v2x)
	{
		MathUtility::Swap(v1x, v2x);
		MathUtility::Swap(v1z, v2z);
	}
	if (v2x < 0 || v1x >= Device::Width) return;
	if (v1x < 0)
	{
		v1z -= (v2z - v1z) * v1x / (v2x - v1x);
		v1x = 0;
	}
	if (v2x >= Device::Width) v2x = Device::Width - 1;

	int16 *depthBuffer = &Device::DepthBuffer[v1x + y * Device::Width];
	sbyte *stencilBuffer = &Device::StencilBuffer[v1x + y * Device::Width];

	float z = v1z;

	int32 subdivs = ((v2x - v1x) >> SubdivExponent) + 1;
	for (int32 i = 0; i < subdivs; i++)
	{
		int32 pixels = i < subdivs - 1 ? Subdiv1 : (v2x - v1x) & SubdivModulo;
		for (int32 i = 0; i < pixels; i++)
		{
			if (*depthBuffer > z * 65536)
			{
				if (IsLastTriangleCulled)
				{
					(*stencilBuffer)++;
				}
				else
				{
					(*stencilBuffer)--;
				}
			}
			stencilBuffer++;
			depthBuffer++;
			z += Dz;
		}
	}
}

bool Drawer::getIsLastTriangleCulled()
{
	return IsLastTriangleCulled;
}