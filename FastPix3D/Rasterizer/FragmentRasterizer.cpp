#include "FragmentRasterizer.h"
#include "../Math/Box2i.h"
#include "../RasterizerMath.h"

void FragmentRasterizer::DrawTriangle(const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	FragmentRasterizerVertex v1 = FragmentRasterizerVertex(_v1);
	FragmentRasterizerVertex v2 = FragmentRasterizerVertex(_v2);
	FragmentRasterizerVertex v3 = FragmentRasterizerVertex(_v3);

	// Transform vertices to view space.
	v1.Position = RenderStates.Precomputed.ModelViewMatrix * v1.Position;
	v2.Position = RenderStates.Precomputed.ModelViewMatrix * v2.Position;
	v3.Position = RenderStates.Precomputed.ModelViewMatrix * v3.Position;

	// Back-face culling.
	bool isFrontFace;
	if (RasterizerMath::IsTriangleCulled(RenderStates.CullMode, v1.Position, v2.Position, v3.Position, isFrontFace))
	{
		return;
	}
	else if (!isFrontFace)
	{
		// When rendering backfaces, reverse winding order.
		std::swap(v1, v3);
	}

	// Clip along near clipping plane.
	bool vertex1Visible = v1.Position.Z > RenderStates.ClipNear;
	bool vertex2Visible = v2.Position.Z > RenderStates.ClipNear;
	bool vertex3Visible = v3.Position.Z > RenderStates.ClipNear;

	if (!vertex1Visible && !vertex2Visible && !vertex3Visible ||
		v1.Position.Z > RenderStates.ClipFar && v2.Position.Z > RenderStates.ClipFar && v3.Position.Z > RenderStates.ClipFar)
	{
		// All vertices are behind the near or far clipping plane.
		return;
	}

	if (RenderStates.ShadowMapFunc != ShadowMapFunc::None)
	{
		if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
		{
			// Transform shadow map coordinates to shadow light space.
			v1.ShadowXyz = RenderStates.Precomputed.ShadowLightModelMatrix * v1.ShadowXyz;
			v2.ShadowXyz = RenderStates.Precomputed.ShadowLightModelMatrix * v2.ShadowXyz;
			v3.ShadowXyz = RenderStates.Precomputed.ShadowLightModelMatrix * v3.ShadowXyz;
		}
		else
		{
			// Store direction vector from light to vertex for cubemap shadow mapping.
			v1.ShadowXyz = RenderStates.ModelMatrix * v1.ShadowXyz - RenderStates.Lights[RenderStates.ShadowLightIndex].Position;
			v2.ShadowXyz = RenderStates.ModelMatrix * v2.ShadowXyz - RenderStates.Lights[RenderStates.ShadowLightIndex].Position;
			v3.ShadowXyz = RenderStates.ModelMatrix * v3.ShadowXyz - RenderStates.Lights[RenderStates.ShadowLightIndex].Position;
		}
	}

	// Transform normals to view space (only required for lighting and sphere mapping).
	if (RenderStates.LightsEnable || RenderStates.TextureEnable && RenderStates.Texture && RenderStates.Texture->Type == TextureType::SphereMap)
	{
		v1.Normals = RenderStates.Precomputed.NormalMatrix * v1.Normals;
		v2.Normals = RenderStates.Precomputed.NormalMatrix * v2.Normals;
		v3.Normals = RenderStates.Precomputed.NormalMatrix * v3.Normals;
	}

	// Compute texture coordinates for sphere mapping.
	if (RenderStates.TextureEnable && RenderStates.Texture && RenderStates.Texture->Type == TextureType::SphereMap)
	{
		v1.TextureCoordinates.X = .5f + v1.Normals.X * .5f;
		v1.TextureCoordinates.Y = .5f - v1.Normals.Y * .5f;
		v2.TextureCoordinates.X = .5f + v2.Normals.X * .5f;
		v2.TextureCoordinates.Y = .5f - v2.Normals.Y * .5f;
		v3.TextureCoordinates.X = .5f + v3.Normals.X * .5f;
		v3.TextureCoordinates.Y = .5f - v3.Normals.Y * .5f;
	}

	bool hasSpecular = RenderStates.LightsEnable && RenderStates.SpecularExponent > 0 && RenderStates.SpecularIntensity > 0;

	// Change vertex colors based on lighting.
	if (RenderStates.LightsEnable)
	{
		vfloat3 vertex1Color = (vfloat3)RenderStates.AmbientLight;
		vfloat3 vertex2Color = (vfloat3)RenderStates.AmbientLight;
		vfloat3 vertex3Color = (vfloat3)RenderStates.AmbientLight;
		vfloat3 vertex1ShadowColor = (vfloat3)RenderStates.AmbientLight;
		vfloat3 vertex2ShadowColor = (vfloat3)RenderStates.AmbientLight;
		vfloat3 vertex3ShadowColor = (vfloat3)RenderStates.AmbientLight;

		for (int32 i = 0; i <= RenderStates.Precomputed.LightsMaxIndex; i++)
		{
			const Light &light = RenderStates.Lights[i];

			if (light.Enabled)
			{
				vfloat3 diffuse;
				vfloat3 specular;

				switch (light.Type)
				{
					case LightType::Directional:
					{
						diffuse = vfloat3(
							light.Precomputed.DirectionViewSpace.DotProduct(v1.Normals),
							light.Precomputed.DirectionViewSpace.DotProduct(v2.Normals),
							light.Precomputed.DirectionViewSpace.DotProduct(v3.Normals)
						) * light.Precomputed.DirectionViewSpaceDotIntensity;

						if (hasSpecular)
						{
							specular = VectorMath::Pow(VectorMath::Max(vfloat3(
								v1.Normals.DotProduct((light.Precomputed.DirectionViewSpace - v1.Position.Normalize()).Normalize()),
								v2.Normals.DotProduct((light.Precomputed.DirectionViewSpace - v2.Position.Normalize()).Normalize()),
								v3.Normals.DotProduct((light.Precomputed.DirectionViewSpace - v3.Position.Normalize()).Normalize())
							), vfloat3()), RenderStates.SpecularExponent) * light.Precomputed.SpecularIntensity;
						}

						break;
					}
					case LightType::Point:
					{
						vfloat3 distance1 = light.Precomputed.PositionViewSpace - v1.Position;
						vfloat3 distance2 = light.Precomputed.PositionViewSpace - v2.Position;
						vfloat3 distance3 = light.Precomputed.PositionViewSpace - v3.Position;

						diffuse = vfloat3(
							distance1.DotProduct(v1.Normals) * distance1.InverseSquaredLength,
							distance2.DotProduct(v2.Normals) * distance2.InverseSquaredLength,
							distance3.DotProduct(v3.Normals) * distance3.InverseSquaredLength
						) * light.Intensity;

						if (hasSpecular)
						{
							specular = VectorMath::Pow(VectorMath::Max(vfloat3(
								v1.Normals.DotProduct((distance1.Normalize() - v1.Position.Normalize()).Normalize()),
								v2.Normals.DotProduct((distance2.Normalize() - v2.Position.Normalize()).Normalize()),
								v3.Normals.DotProduct((distance3.Normalize() - v3.Position.Normalize()).Normalize())
							), vfloat3()), RenderStates.SpecularExponent) * light.Precomputed.SpecularIntensity;
						}

						break;
					}
					case LightType::Spot:
					{
						vfloat3 distance1 = light.Precomputed.PositionViewSpace - v1.Position;
						vfloat3 distance2 = light.Precomputed.PositionViewSpace - v2.Position;
						vfloat3 distance3 = light.Precomputed.PositionViewSpace - v3.Position;

						vfloat3 cone = vfloat3(
							(1 - acosf(distance1.Normalize().DotProduct(light.Precomputed.DirectionViewSpace))),
							(1 - acosf(distance2.Normalize().DotProduct(light.Precomputed.DirectionViewSpace))),
							(1 - acosf(distance3.Normalize().DotProduct(light.Precomputed.DirectionViewSpace)))
						);

						diffuse = vfloat3(
							distance1.DotProduct(v1.Normals) * distance1.InverseSquaredLength * cone.X,
							distance2.DotProduct(v2.Normals) * distance2.InverseSquaredLength * cone.Y,
							distance3.DotProduct(v3.Normals) * distance3.InverseSquaredLength * cone.Z
						) * light.Intensity;

						if (hasSpecular)
						{
							specular = VectorMath::Pow(VectorMath::Max(vfloat3(
								v1.Normals.DotProduct((distance1.Normalize() - v1.Position.Normalize()).Normalize()),
								v2.Normals.DotProduct((distance2.Normalize() - v2.Position.Normalize()).Normalize()),
								v3.Normals.DotProduct((distance3.Normalize() - v3.Position.Normalize()).Normalize())
							), vfloat3()), RenderStates.SpecularExponent) * VectorMath::Max(cone, vfloat3()) * light.Precomputed.SpecularIntensity;
						}

						break;
					}
				}

				diffuse = VectorMath::Max(diffuse, vfloat3());

				vertex1Color += light.Precomputed.ColorF * diffuse.X;
				vertex2Color += light.Precomputed.ColorF * diffuse.Y;
				vertex3Color += light.Precomputed.ColorF * diffuse.Z;

				if (hasSpecular)
				{
					v1.Specular += light.Precomputed.ColorF * specular.X;
					v2.Specular += light.Precomputed.ColorF * specular.Y;
					v3.Specular += light.Precomputed.ColorF * specular.Z;
				}

				if (RenderStates.ShadowMapFunc != ShadowMapFunc::None && i != RenderStates.ShadowLightIndex)
				{
					vertex1ShadowColor += light.Precomputed.ColorF * diffuse.X;
					vertex2ShadowColor += light.Precomputed.ColorF * diffuse.Y;
					vertex3ShadowColor += light.Precomputed.ColorF * diffuse.Z;
				}
			}
		}

		v1.Color = VectorMath::Min(v1.Color * vertex1Color * (1.0f / 256.0f), 255);
		v2.Color = VectorMath::Min(v2.Color * vertex2Color * (1.0f / 256.0f), 255);
		v3.Color = VectorMath::Min(v3.Color * vertex3Color * (1.0f / 256.0f), 255);

		v1.Specular = VectorMath::Min(v1.Specular, 255);
		v2.Specular = VectorMath::Min(v2.Specular, 255);
		v3.Specular = VectorMath::Min(v3.Specular, 255);
		hasSpecular &= !(v1.Specular < 2) || !(v2.Specular < 2) || !(v3.Specular < 2);

		if (RenderStates.ShadowMapFunc != ShadowMapFunc::None)
		{
			v1.ShadowColor = VectorMath::Min(v1.ShadowColor * vertex1ShadowColor * (1.0f / 256.0f), 255);
			v2.ShadowColor = VectorMath::Min(v2.ShadowColor * vertex2ShadowColor * (1.0f / 256.0f), 255);
			v3.ShadowColor = VectorMath::Min(v3.ShadowColor * vertex3ShadowColor * (1.0f / 256.0f), 255);
		}
	}

	bool rendered = false;
	bool hasColor = !RenderStates.TextureEnable || !RenderStates.Texture || hasSpecular || !(v1.Color > 254) || !(v2.Color > 254) || !(v3.Color > 254);

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		// All vertices are in front of the near clipping plane.
		rendered = DrawClippedTriangle(v1, v2, v3, hasColor, hasSpecular);
	}
	else
	{
		// 1 or 2 vertices are in front of the near clipping plane.
		FragmentRasterizerVertex v12;
		FragmentRasterizerVertex v23;
		FragmentRasterizerVertex v31;

		v12.Position = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.Position, v2.Position);
		v23.Position = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.Position, v3.Position);
		v31.Position = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.Position, v1.Position);

		if (RenderStates.TextureEnable && RenderStates.Texture)
		{
			v12.TextureCoordinates = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.TextureCoordinates, v2.TextureCoordinates);
			v23.TextureCoordinates = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.TextureCoordinates, v3.TextureCoordinates);
			v31.TextureCoordinates = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.TextureCoordinates, v1.TextureCoordinates);
		}

		if (hasColor)
		{
			v12.Color = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.Color, v2.Color);
			v23.Color = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.Color, v3.Color);
			v31.Color = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.Color, v1.Color);
		}

		if (hasSpecular)
		{
			v12.Specular = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.Specular, v2.Specular);
			v23.Specular = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.Specular, v3.Specular);
			v31.Specular = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.Specular, v1.Specular);
		}

		if (RenderStates.ShadowMapFunc != ShadowMapFunc::None)
		{
			v12.ShadowXyz = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.ShadowXyz, v2.ShadowXyz);
			v23.ShadowXyz = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.ShadowXyz, v3.ShadowXyz);
			v31.ShadowXyz = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.ShadowXyz, v1.ShadowXyz);

			v12.ShadowColor = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.ShadowColor, v2.ShadowColor);
			v23.ShadowColor = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.ShadowColor, v3.ShadowColor);
			v31.ShadowColor = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.ShadowColor, v1.ShadowColor);
		}

		if (vertex1Visible && vertex2Visible)
		{
			rendered = DrawClippedTriangle(v31, v1, v23, hasColor, hasSpecular);
			rendered |= DrawClippedTriangle(v1, v2, v23, hasColor, hasSpecular);
		}
		else if (vertex2Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v2, hasColor, hasSpecular);
			rendered |= DrawClippedTriangle(v12, v2, v31, hasColor, hasSpecular);
		}
		else if (vertex1Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v23, hasColor, hasSpecular);
			rendered |= DrawClippedTriangle(v3, v1, v23, hasColor, hasSpecular);
		}
		else if (vertex1Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v31, hasColor, hasSpecular);
		}
		else if (vertex2Visible)
		{
			rendered = DrawClippedTriangle(v2, v23, v12, hasColor, hasSpecular);
		}
		else if (vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v23, hasColor, hasSpecular);
		}
	}

	if (rendered && RasterizerMath::GetWorkloadThreadIndex(RenderStates.Workload) == 0)
	{
		Statistics.RenderedTriangleCount++;
	}
}
bool FragmentRasterizer::DrawClippedTriangle(const FragmentRasterizerVertex &v1, const FragmentRasterizerVertex &v2, const FragmentRasterizerVertex &v3, bool hasColor, bool hasSpecular) const
{
	// When using template parameters for render states, the compiler will evaluate branches at compile time.
	// Branches at a per-pixel level are extremely expensive and must be kept to what's essential, such as depth testing.

	if (RenderStates.TextureEnable && RenderStates.Texture)
	{
		if (hasColor)
		{
			if (hasSpecular)
			{
				if (RenderStates.TextureFilteringEnable)
				{
					switch (RenderStates.ShadowMapFunc)
					{
						case ShadowMapFunc::None:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, true, true, true, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, true, true, true, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Point:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, true, true, true, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, true, true, true, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Pcf:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, true, true, true, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, true, true, true, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
					}
				}
				else
				{
					switch (RenderStates.ShadowMapFunc)
					{
						case ShadowMapFunc::None:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, false, true, true, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, false, true, true, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Point:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, false, true, true, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, false, true, true, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Pcf:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, false, true, true, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, false, true, true, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
					}
				}
			}
			else
			{
				if (RenderStates.TextureFilteringEnable)
				{
					switch (RenderStates.ShadowMapFunc)
					{
						case ShadowMapFunc::None:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, true, true, false, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, true, true, false, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Point:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, true, true, false, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, true, true, false, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Pcf:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, true, true, false, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, true, true, false, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
					}
				}
				else
				{
					switch (RenderStates.ShadowMapFunc)
					{
						case ShadowMapFunc::None:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, false, true, false, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, false, true, false, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Point:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, false, true, false, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, false, true, false, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
						case ShadowMapFunc::Pcf:
							if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
							{
								return DrawClippedTriangle<true, false, true, false, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
							}
							else
							{
								return DrawClippedTriangle<true, false, true, false, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
							}
					}
				}
			}
		}
		else
		{
			if (RenderStates.TextureFilteringEnable)
			{
				switch (RenderStates.ShadowMapFunc)
				{
					case ShadowMapFunc::None:
						if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
						{
							return DrawClippedTriangle<true, true, false, false, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
						}
						else
						{
							return DrawClippedTriangle<true, true, false, false, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
						}
					case ShadowMapFunc::Point:
						if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
						{
							return DrawClippedTriangle<true, true, false, false, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
						}
						else
						{
							return DrawClippedTriangle<true, true, false, false, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
						}
					case ShadowMapFunc::Pcf:
						if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
						{
							return DrawClippedTriangle<true, true, false, false, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
						}
						else
						{
							return DrawClippedTriangle<true, true, false, false, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
						}
				}
			}
			else
			{
				switch (RenderStates.ShadowMapFunc)
				{
					case ShadowMapFunc::None:
						if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
						{
							return DrawClippedTriangle<true, false, false, false, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
						}
						else
						{
							return DrawClippedTriangle<true, false, false, false, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
						}
					case ShadowMapFunc::Point:
						if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
						{
							return DrawClippedTriangle<true, false, false, false, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
						}
						else
						{
							return DrawClippedTriangle<true, false, false, false, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
						}
					case ShadowMapFunc::Pcf:
						if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
						{
							return DrawClippedTriangle<true, false, false, false, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
						}
						else
						{
							return DrawClippedTriangle<true, false, false, false, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
						}
				}
			}
		}
	}
	else
	{
		if (hasSpecular)
		{
			switch (RenderStates.ShadowMapFunc)
			{
				case ShadowMapFunc::None:
					if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
					{
						return DrawClippedTriangle<false, false, true, true, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
					}
					else
					{
						return DrawClippedTriangle<false, false, true, true, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
					}
				case ShadowMapFunc::Point:
					if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
					{
						return DrawClippedTriangle<false, false, true, true, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
					}
					else
					{
						return DrawClippedTriangle<false, false, true, true, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
					}
				case ShadowMapFunc::Pcf:
					if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
					{
						return DrawClippedTriangle<false, false, true, true, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
					}
					else
					{
						return DrawClippedTriangle<false, false, true, true, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
					}
			}
		}
		else
		{
			switch (RenderStates.ShadowMapFunc)
			{
				case ShadowMapFunc::None:
					if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
					{
						return DrawClippedTriangle<false, false, true, false, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
					}
					else
					{
						return DrawClippedTriangle<false, false, true, false, ShadowMapFunc::None, ShadowMapProjection::Cubemap>(v1, v2, v3);
					}
				case ShadowMapFunc::Point:
					if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
					{
						return DrawClippedTriangle<false, false, true, false, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
					}
					else
					{
						return DrawClippedTriangle<false, false, true, false, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
					}
				case ShadowMapFunc::Pcf:
					if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
					{
						return DrawClippedTriangle<false, false, true, false, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
					}
					else
					{
						return DrawClippedTriangle<false, false, true, false, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
					}
			}
		}
	}

	return false;
}
template<bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
bool FragmentRasterizer::DrawClippedTriangle(const FragmentRasterizerVertex &v1, const FragmentRasterizerVertex &v2, const FragmentRasterizerVertex &v3) const
{
	if (RenderStates.ZEnable)
	{
		if (RenderStates.ZWriteEnable)
		{
			switch (RenderStates.BlendMode)
			{
				case BlendMode::None: return DrawClippedTriangle<true, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::None, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::TransparencyKey: return DrawClippedTriangle<true, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::TransparencyKey, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Alpha: return DrawClippedTriangle<true, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Alpha, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Multiply: return DrawClippedTriangle<true, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Multiply, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Add: return DrawClippedTriangle<true, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Add, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
			}
		}
		else
		{
			switch (RenderStates.BlendMode)
			{
				case BlendMode::None: return DrawClippedTriangle<true, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::None, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::TransparencyKey: return DrawClippedTriangle<true, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::TransparencyKey, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Alpha: return DrawClippedTriangle<true, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Alpha, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Multiply: return DrawClippedTriangle<true, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Multiply, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Add: return DrawClippedTriangle<true, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Add, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
			}
		}
	}
	else
	{
		if (RenderStates.ZWriteEnable)
		{
			switch (RenderStates.BlendMode)
			{
				case BlendMode::None: return DrawClippedTriangle<false, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::None, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::TransparencyKey: return DrawClippedTriangle<false, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::TransparencyKey, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Alpha: return DrawClippedTriangle<false, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Alpha, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Multiply: return DrawClippedTriangle<false, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Multiply, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Add: return DrawClippedTriangle<false, true, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Add, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
			}
		}
		else
		{
			switch (RenderStates.BlendMode)
			{
				case BlendMode::None: return DrawClippedTriangle<false, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::None, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::TransparencyKey: return DrawClippedTriangle<false, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::TransparencyKey, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Alpha: return DrawClippedTriangle<false, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Alpha, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Multiply: return DrawClippedTriangle<false, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Multiply, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
				case BlendMode::Add: return DrawClippedTriangle<false, false, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Add, shadowMapFunc, shadowMapProjection>(v1, v2, v3);
			}
		}
	}

	return false;
}
template<bool zEnable, bool zWriteEnable, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
bool FragmentRasterizer::DrawClippedTriangle(FragmentRasterizerVertex v1, FragmentRasterizerVertex v2, FragmentRasterizerVertex v3) const
{
	// Transform vertices to clip space.
	v1.Position = RasterizerMath::ToClipSpace(v1.Position, RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, RenderStates.Zoom, RenderStates.ClipNear);
	v2.Position = RasterizerMath::ToClipSpace(v2.Position, RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, RenderStates.Zoom, RenderStates.ClipNear);
	v3.Position = RasterizerMath::ToClipSpace(v3.Position, RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, RenderStates.Zoom, RenderStates.ClipNear);

	if (RasterizerMath::IsTriangleOutsideClipVolume(v1.Position, v2.Position, v3.Position))
	{
		// Triangle is completely outside of the view frustum.
		return false;
	}

	if constexpr (shadowMapFunc != ShadowMapFunc::None)
	{
		if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
		{
			// X/Y = Shadow map UV coordinates
			// Z   = Depth to light source
			v1.ShadowXyz.X = .5f + v1.ShadowXyz.X / v1.ShadowXyz.Z * RenderStates.ShadowLightZoom * .5f;
			v1.ShadowXyz.Y = .5f - v1.ShadowXyz.Y / v1.ShadowXyz.Z * RenderStates.ShadowLightZoom * .5f;
			v2.ShadowXyz.X = .5f + v2.ShadowXyz.X / v2.ShadowXyz.Z * RenderStates.ShadowLightZoom * .5f;
			v2.ShadowXyz.Y = .5f - v2.ShadowXyz.Y / v2.ShadowXyz.Z * RenderStates.ShadowLightZoom * .5f;
			v3.ShadowXyz.X = .5f + v3.ShadowXyz.X / v3.ShadowXyz.Z * RenderStates.ShadowLightZoom * .5f;
			v3.ShadowXyz.Y = .5f - v3.ShadowXyz.Y / v3.ShadowXyz.Z * RenderStates.ShadowLightZoom * .5f;

			// Store UV multiplied by texture resolution.
			v1.ShadowXyz.X *= RenderStates.ShadowMap.Width;
			v1.ShadowXyz.Y *= RenderStates.ShadowMap.Width;
			v2.ShadowXyz.X *= RenderStates.ShadowMap.Width;
			v2.ShadowXyz.Y *= RenderStates.ShadowMap.Width;
			v3.ShadowXyz.X *= RenderStates.ShadowMap.Width;
			v3.ShadowXyz.Y *= RenderStates.ShadowMap.Width;

			if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				// PCF is the equivalent of bilinear texture filtering on the shadow map, so offset texels by half.
				v1.ShadowXyz.X -= .5f;
				v1.ShadowXyz.Y -= .5f;
				v2.ShadowXyz.X -= .5f;
				v2.ShadowXyz.Y -= .5f;
				v3.ShadowXyz.X -= .5f;
				v3.ShadowXyz.Y -= .5f;
			}

			// Shadow maps of point lights need to be perspective corrected by both the camera space and the light space.
			v1.ShadowXyz.X *= v1.ShadowXyz.Z;
			v1.ShadowXyz.Y *= v1.ShadowXyz.Z;
			v2.ShadowXyz.X *= v2.ShadowXyz.Z;
			v2.ShadowXyz.Y *= v2.ShadowXyz.Z;
			v3.ShadowXyz.X *= v3.ShadowXyz.Z;
			v3.ShadowXyz.Y *= v3.ShadowXyz.Z;
		}

		v1.ShadowXyz *= v1.Position.Z;
		v2.ShadowXyz *= v2.Position.Z;
		v3.ShadowXyz *= v3.Position.Z;
	}

	// Project vertices screen space.
	vfloat2 v1ScreenF = RasterizerMath::ToScreenSpaceF(v1.Position, RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height);
	vfloat2 v2ScreenF = RasterizerMath::ToScreenSpaceF(v2.Position, RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height);
	vfloat2 v3ScreenF = RasterizerMath::ToScreenSpaceF(v3.Position, RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height);

	int32 fixExponent = RasterizerMath::GetTriangleFixExponent(v1ScreenF, v2ScreenF, v3ScreenF);
	vfix2 v1Screen = (vfix2)VectorMath::Round(fixf(v1ScreenF, fixExponent));
	vfix2 v2Screen = (vfix2)VectorMath::Round(fixf(v2ScreenF, fixExponent));
	vfix2 v3Screen = (vfix2)VectorMath::Round(fixf(v3ScreenF, fixExponent));

	// Compute bounding box, aligned to 8x8 screen coordinates (max = exclusive upper bound)
	Box2x boundingBoxFix = Box2i(
		vfix2(
			Math::Max(halfspace_lower_fix(Math::Min(v1Screen.X, v2Screen.X, v3Screen.X), fixExponent), 0),
			Math::Max(halfspace_lower_fix(Math::Min(v1Screen.Y, v2Screen.Y, v3Screen.Y), fixExponent), 0)
		),
		vfix2(
			Math::Min(halfspace_upper_fix(Math::Max(v1Screen.X, v2Screen.X, v3Screen.X), fixExponent), fix(RenderStates.FrameBuffer.Width, fixExponent) - 1),
			Math::Min(halfspace_upper_fix(Math::Max(v1Screen.Y, v2Screen.Y, v3Screen.Y), fixExponent), fix(RenderStates.FrameBuffer.Height, fixExponent) - 1)
		)
	);
	Box2i boundingBox = Box2i(unfix(boundingBoxFix.Min, fixExponent), unfix(boundingBoxFix.Max, fixExponent));

	int32 workloadOffset;
	int32 workloadIncrement;
	RasterizerMath::GetWorkloadParameters(RenderStates.Workload, halfspace_div(boundingBox.Min.Y), workloadOffset, workloadIncrement);
	workloadOffset = halfspace_mul(workloadOffset);
	workloadIncrement = halfspace_mul(workloadIncrement);

	fix32 yStart = boundingBoxFix.Min.Y + fix(workloadOffset, fixExponent);

	if constexpr (hasTexture)
	{
		// Scale texture coordinates by texture size.
		v1.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v2.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v3.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;

		// Texture coordinates must be positive.
		RasterizerMath::MakeTextureCoordinatesPositive(v1.TextureCoordinates, v2.TextureCoordinates, v3.TextureCoordinates);

		// Store UV multiplied by texture resolution.
		v1.TextureCoordinates.X *= v1.Position.Z * RenderStates.Texture->Width;
		v1.TextureCoordinates.Y *= v1.Position.Z * RenderStates.Texture->Height;
		v2.TextureCoordinates.X *= v2.Position.Z * RenderStates.Texture->Width;
		v2.TextureCoordinates.Y *= v2.Position.Z * RenderStates.Texture->Height;
		v3.TextureCoordinates.X *= v3.Position.Z * RenderStates.Texture->Width;
		v3.TextureCoordinates.Y *= v3.Position.Z * RenderStates.Texture->Height;
	}

	if constexpr (hasColor)
	{
		v1.Color *= v1.Position.Z;
		v2.Color *= v2.Position.Z;
		v3.Color *= v3.Position.Z;
	}

	if constexpr (hasSpecular)
	{
		v1.Specular *= v1.Position.Z;
		v2.Specular *= v2.Position.Z;
		v3.Specular *= v3.Position.Z;
	}

	if constexpr (shadowMapFunc != ShadowMapFunc::None)
	{
		v1.ShadowColor *= v1.Position.Z;
		v2.ShadowColor *= v2.Position.Z;
		v3.ShadowColor *= v3.Position.Z;
	}

	int32 alpha = blendMode == BlendMode::Alpha || blendMode == BlendMode::Add ? (int32)(RenderStates.Alpha * 255) : 0;

	// Edge equation
	vfix4 edgeDeltaX = vfix4(v1Screen.Y, v2Screen.Y, v3Screen.Y) - vfix4(v2Screen.Y, v3Screen.Y, v1Screen.Y);
	vfix4 edgeDeltaY = vfix4(v2Screen.X, v3Screen.X, v1Screen.X) - vfix4(v1Screen.X, v2Screen.X, v3Screen.X);
	vfix4 blockEdgeDeltaX = halfspace_mul(edgeDeltaX);
	vfix4 blockEdgeDeltaY = edgeDeltaY * workloadIncrement;

	// Top-left fill convention.
	vfix4 edgeStart = vfix4(fix(1, fixExponent)) & (VectorMath::CmpLt(edgeDeltaX, vfix4()) | VectorMath::CmpEq(edgeDeltaX, vfix4()) & VectorMath::CmpGt(edgeDeltaY, vfix4()));

	// Edge equation at top-left corner of the bounding box.
	edgeStart += vfix4(
		(fix32)unfix((fix64)edgeDeltaX[0] * (boundingBoxFix.Min.X - v1Screen.X) + (fix64)edgeDeltaY[0] * (yStart - v1Screen.Y), fixExponent),
		(fix32)unfix((fix64)edgeDeltaX[1] * (boundingBoxFix.Min.X - v2Screen.X) + (fix64)edgeDeltaY[1] * (yStart - v2Screen.Y), fixExponent),
		(fix32)unfix((fix64)edgeDeltaX[2] * (boundingBoxFix.Min.X - v3Screen.X) + (fix64)edgeDeltaY[2] * (yStart - v3Screen.Y), fixExponent)
	);

	// Edge equation offsets for the other 3 corners of the block.
	vfix4 cornerEdgeARow = VectorMath::Shuffle<0, 0, 0, 0>(edgeStart) + halfspace_mul((VectorMath::Shuffle<3, 0, 3, 0>(edgeDeltaX) + VectorMath::Shuffle<3, 3, 0, 0>(edgeDeltaY)));
	vfix4 cornerEdgeBRow = VectorMath::Shuffle<1, 1, 1, 1>(edgeStart) + halfspace_mul((VectorMath::Shuffle<3, 1, 3, 1>(edgeDeltaX) + VectorMath::Shuffle<3, 3, 1, 1>(edgeDeltaY)));
	vfix4 cornerEdgeCRow = VectorMath::Shuffle<2, 2, 2, 2>(edgeStart) + halfspace_mul((VectorMath::Shuffle<3, 2, 3, 2>(edgeDeltaX) + VectorMath::Shuffle<3, 3, 2, 2>(edgeDeltaY)));

	vfix4 cornerEdgeADeltaX = VectorMath::Shuffle<0, 0, 0, 0>(blockEdgeDeltaX);
	vfix4 cornerEdgeBDeltaX = VectorMath::Shuffle<1, 1, 1, 1>(blockEdgeDeltaX);
	vfix4 cornerEdgeCDeltaX = VectorMath::Shuffle<2, 2, 2, 2>(blockEdgeDeltaX);
	vfix4 cornerEdgeADeltaY = VectorMath::Shuffle<0, 0, 0, 0>(blockEdgeDeltaY);
	vfix4 cornerEdgeBDeltaY = VectorMath::Shuffle<1, 1, 1, 1>(blockEdgeDeltaY);
	vfix4 cornerEdgeCDeltaY = VectorMath::Shuffle<2, 2, 2, 2>(blockEdgeDeltaY);

	vfix8 blockEdgeARow = vfix8(edgeStart[0]) + vfix8(edgeDeltaX[0]) * Delta1To8Multiplier;
	vfix8 blockEdgeBRow = vfix8(edgeStart[1]) + vfix8(edgeDeltaX[1]) * Delta1To8Multiplier;
	vfix8 blockEdgeCRow = vfix8(edgeStart[2]) + vfix8(edgeDeltaX[2]) * Delta1To8Multiplier;

	vfix8 blockEdgeADeltaX = vfix8(blockEdgeDeltaX[0]);
	vfix8 blockEdgeBDeltaX = vfix8(blockEdgeDeltaX[1]);
	vfix8 blockEdgeCDeltaX = vfix8(blockEdgeDeltaX[2]);
	vfix8 blockEdgeADeltaY = vfix8(blockEdgeDeltaY[0]);
	vfix8 blockEdgeBDeltaY = vfix8(blockEdgeDeltaY[1]);
	vfix8 blockEdgeCDeltaY = vfix8(blockEdgeDeltaY[2]);

	vfix8 edgeADeltaY = vfix8(edgeDeltaY[0]);
	vfix8 edgeBDeltaY = vfix8(edgeDeltaY[1]);
	vfix8 edgeCDeltaY = vfix8(edgeDeltaY[2]);

	// Barycentric determinants for interpolation of vertex attributes.
	float barycentricDet = 1.0f / ((fix64)edgeDeltaX[1] * edgeDeltaY[2] - (fix64)edgeDeltaY[1] * edgeDeltaX[2]);
	vfloat2 barycentricL1 = (vfloat2)fix(vfix2(edgeDeltaX[1], edgeDeltaY[1]), fixExponent) * barycentricDet;
	vfloat2 barycentricL2 = (vfloat2)fix(vfix2(edgeDeltaX[2], edgeDeltaY[2]), fixExponent) * barycentricDet;
	vfloat2 barycentricL3 = -barycentricL1 - barycentricL2;

	// Barycentric coordinates at top-left corner of the bounding box.
	float barycentricStartL1 = unfixf((boundingBoxFix.Min.X - v3Screen.X) * barycentricL1.X + (yStart - v3Screen.Y) * barycentricL1.Y, fixExponent);
	float barycentricStartL2 = unfixf((boundingBoxFix.Min.X - v3Screen.X) * barycentricL2.X + (yStart - v3Screen.Y) * barycentricL2.Y, fixExponent);
	float barycentricStartL3 = 1 - barycentricStartL1 - barycentricStartL2;

	vfloat8 v1Attributes = vfloat8(v1.Position.Z, hasTexture ? v1.TextureCoordinates.X : 0, hasTexture ? v1.TextureCoordinates.Y : 0, hasColor ? v1.Color.X : 0, hasColor ? v1.Color.Y : 0, hasColor ? v1.Color.Z : 0);
	vfloat8 v2Attributes = vfloat8(v2.Position.Z, hasTexture ? v2.TextureCoordinates.X : 0, hasTexture ? v2.TextureCoordinates.Y : 0, hasColor ? v2.Color.X : 0, hasColor ? v2.Color.Y : 0, hasColor ? v2.Color.Z : 0);
	vfloat8 v3Attributes = vfloat8(v3.Position.Z, hasTexture ? v3.TextureCoordinates.X : 0, hasTexture ? v3.TextureCoordinates.Y : 0, hasColor ? v3.Color.X : 0, hasColor ? v3.Color.Y : 0, hasColor ? v3.Color.Z : 0);
	vfloat8 v1AttributesB;
	vfloat8 v2AttributesB;
	vfloat8 v3AttributesB;

	if constexpr (hasSpecular)
	{
		v1AttributesB = vfloat8(v1.Specular.X, v1.Specular.Y, v1.Specular.Z);
		v2AttributesB = vfloat8(v2.Specular.X, v2.Specular.Y, v2.Specular.Z);
		v3AttributesB = vfloat8(v3.Specular.X, v3.Specular.Y, v3.Specular.Z);
	}

	// Vertex attributes at top-left corner of the bounding box & deltas per pixel for interpolation.
	vfloat8 blockAttributesRow = v1Attributes * barycentricStartL1 + v2Attributes * barycentricStartL2 + v3Attributes * barycentricStartL3;
	vfloat8 attributesDeltaX = v1Attributes * barycentricL1.X + v2Attributes * barycentricL2.X + v3Attributes * barycentricL3.X;
	vfloat8 attributesDeltaY = v1Attributes * barycentricL1.Y + v2Attributes * barycentricL2.Y + v3Attributes * barycentricL3.Y;
	vfloat8 blockAttributesBRow;
	vfloat8 attributesBDeltaX;
	vfloat8 attributesBDeltaY;

	if constexpr (hasSpecular)
	{
		blockAttributesBRow = v1AttributesB * barycentricStartL1 + v2AttributesB * barycentricStartL2 + v3AttributesB * barycentricStartL3;
		attributesBDeltaX = v1AttributesB * barycentricL1.X + v2AttributesB * barycentricL2.X + v3AttributesB * barycentricL3.X;
		attributesBDeltaY = v1AttributesB * barycentricL1.Y + v2AttributesB * barycentricL2.Y + v3AttributesB * barycentricL3.Y;
	}

	// Deltas per block for interpolation of vertex attributes.
	vfloat8 blockAttributesDeltaX = halfspace_mulf(attributesDeltaX);
	vfloat8 blockAttributesDeltaY = attributesDeltaY * (float)workloadIncrement;
	vfloat8 blockAttributesBDeltaX;
	vfloat8 blockAttributesBDeltaY;

	if constexpr (hasSpecular)
	{
		blockAttributesBDeltaX = halfspace_mulf(attributesBDeltaX);
		blockAttributesBDeltaY = attributesBDeltaY * (float)workloadIncrement;
	}

	vfloat8 blockShadowAttributesRow;
	vfloat8 shadowAttributesDeltaX;
	vfloat8 shadowAttributesDeltaY;
	vfloat8 blockShadowAttributesDeltaX;
	vfloat8 blockShadowAttributesDeltaY;

	if constexpr (shadowMapFunc != ShadowMapFunc::None)
	{
		vfloat8 v1ShadowAttributes = vfloat8(v1.ShadowXyz.X, v1.ShadowXyz.Y, v1.ShadowXyz.Z, v1.ShadowColor.X, v1.ShadowColor.Y, v1.ShadowColor.Z);
		vfloat8 v2ShadowAttributes = vfloat8(v2.ShadowXyz.X, v2.ShadowXyz.Y, v2.ShadowXyz.Z, v2.ShadowColor.X, v2.ShadowColor.Y, v2.ShadowColor.Z);
		vfloat8 v3ShadowAttributes = vfloat8(v3.ShadowXyz.X, v3.ShadowXyz.Y, v3.ShadowXyz.Z, v3.ShadowColor.X, v3.ShadowColor.Y, v3.ShadowColor.Z);

		blockShadowAttributesRow = v1ShadowAttributes * barycentricStartL1 + v2ShadowAttributes * barycentricStartL2 + v3ShadowAttributes * barycentricStartL3;
		shadowAttributesDeltaX = v1ShadowAttributes * barycentricL1.X + v2ShadowAttributes * barycentricL2.X + v3ShadowAttributes * barycentricL3.X;
		shadowAttributesDeltaY = v1ShadowAttributes * barycentricL1.Y + v2ShadowAttributes * barycentricL2.Y + v3ShadowAttributes * barycentricL3.Y;

		blockShadowAttributesDeltaX = halfspace_mulf(shadowAttributesDeltaX);
		blockShadowAttributesDeltaY = shadowAttributesDeltaY * (float)workloadIncrement;
	}

	int32 bufferStart = boundingBox.Min.X + unfix(yStart, fixExponent) * RenderStates.FrameBuffer.Width;
	Color *frameBuffer = RenderStates.FrameBuffer.GetBuffer<Color>(bufferStart);
	float *depthBuffer = zEnable || zWriteEnable ? RenderStates.DepthBuffer.GetBuffer<float>(bufferStart) : nullptr;

	float *shadowMap;
	int32 shadowMapSize;
	int32 shadowMapSizeMask;
	int32 shadowMapSizeExponent;

	if constexpr (shadowMapFunc != ShadowMapFunc::None)
	{
		shadowMap = RenderStates.ShadowMap.GetBuffer<float>();
		shadowMapSize = RenderStates.ShadowMap.Width;
		shadowMapSizeMask = shadowMapSize - 1;
		shadowMapSizeExponent = Math::GetExponent(shadowMapSize);
	}

	int32 stride = RenderStates.FrameBuffer.Width; // Advance to next row in 8x8 block.
	int32 strideBlock = -halfspace_mul(RenderStates.FrameBuffer.Width); // Rewind to top-left corner of 8x8 block.
	int32 strideRow = RenderStates.FrameBuffer.Width * workloadIncrement - (boundingBox.Max.X + 1 - boundingBox.Min.X); // Advance to next row of blocks.

	for (int32 blockY = unfix(yStart, fixExponent); blockY < boundingBox.Max.Y; blockY += workloadIncrement)
	{
		bool isDrawing = false;
		vfix4 cornerEdgeA = cornerEdgeARow;
		vfix4 cornerEdgeB = cornerEdgeBRow;
		vfix4 cornerEdgeC = cornerEdgeCRow;
		vfix8 blockEdgeA = blockEdgeARow;
		vfix8 blockEdgeB = blockEdgeBRow;
		vfix8 blockEdgeC = blockEdgeCRow;
		vfloat8 blockAttributes = blockAttributesRow;
		vfloat8 blockShadowAttributes = blockShadowAttributesRow;
		vfloat8 blockAttributesB = blockAttributesBRow;

		for (int32 blockX = boundingBox.Min.X; blockX < boundingBox.Max.X; blockX += halfspace_mul(1))
		{
			// At least one corner of the 8x8 block is inside the triangle.
			if (VectorMath::CmpGtMask(cornerEdgeA, vfix4()) != 0 &&
				VectorMath::CmpGtMask(cornerEdgeB, vfix4()) != 0 &&
				VectorMath::CmpGtMask(cornerEdgeC, vfix4()) != 0)
			{
				isDrawing = true;

				vfloat8 attributesRow = blockAttributes / blockAttributes[ATTRIBUTE_Z];
				attributesRow[ATTRIBUTE_Z] = blockAttributes[ATTRIBUTE_Z];

				// Do perspective correction only once per block.
				vfloat8 attributesPixelDeltaX = (blockAttributes + attributesDeltaX) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaX[ATTRIBUTE_Z]) - attributesRow;
				vfloat8 attributesPixelDeltaY = (blockAttributes + attributesDeltaY) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaY[ATTRIBUTE_Z]) - attributesRow;
				attributesPixelDeltaX[ATTRIBUTE_Z] = attributesDeltaX[ATTRIBUTE_Z];
				attributesPixelDeltaY[ATTRIBUTE_Z] = attributesDeltaY[ATTRIBUTE_Z];

				vfloat8 attributesBRow;
				vfloat8 attributesBPixelDeltaX;
				vfloat8 attributesBPixelDeltaY;

				if constexpr (hasSpecular)
				{
					attributesBRow = blockAttributesB / blockAttributes[ATTRIBUTE_Z];
					attributesBPixelDeltaX = (blockAttributesB + attributesBDeltaX) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaX[ATTRIBUTE_Z]) - attributesBRow;
					attributesBPixelDeltaY = (blockAttributesB + attributesBDeltaY) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaY[ATTRIBUTE_Z]) - attributesBRow;
				}

				vfloat8 shadowAttributesRow;
				vfloat8 shadowAttributesPixelDeltaX;
				vfloat8 shadowAttributesPixelDeltaY;

				if constexpr (shadowMapFunc != ShadowMapFunc::None)
				{
					if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
					{
						vfloat8 d = vfloat8(
							blockShadowAttributes[ATTRIBUTE_SHADOW_Z],
							blockShadowAttributes[ATTRIBUTE_SHADOW_Z],
							blockAttributes[ATTRIBUTE_Z],
							blockAttributes[ATTRIBUTE_Z],
							blockAttributes[ATTRIBUTE_Z],
							blockAttributes[ATTRIBUTE_Z]
						);

						vfloat8 dx = vfloat8(
							shadowAttributesDeltaX[ATTRIBUTE_SHADOW_Z],
							shadowAttributesDeltaX[ATTRIBUTE_SHADOW_Z],
							attributesDeltaX[ATTRIBUTE_Z],
							attributesDeltaX[ATTRIBUTE_Z],
							attributesDeltaX[ATTRIBUTE_Z],
							attributesDeltaX[ATTRIBUTE_Z]
						);

						vfloat8 dy = vfloat8(
							shadowAttributesDeltaY[ATTRIBUTE_SHADOW_Z],
							shadowAttributesDeltaY[ATTRIBUTE_SHADOW_Z],
							attributesDeltaY[ATTRIBUTE_Z],
							attributesDeltaY[ATTRIBUTE_Z],
							attributesDeltaY[ATTRIBUTE_Z],
							attributesDeltaY[ATTRIBUTE_Z]
						);

						shadowAttributesRow = blockShadowAttributes / d;
						shadowAttributesPixelDeltaX = (blockShadowAttributes + shadowAttributesDeltaX) / (d + dx) - shadowAttributesRow;
						shadowAttributesPixelDeltaY = (blockShadowAttributes + shadowAttributesDeltaY) / (d + dy) - shadowAttributesRow;
					}
					else if constexpr (shadowMapProjection == ShadowMapProjection::Cubemap)
					{
						shadowAttributesRow = blockShadowAttributes / blockAttributes[ATTRIBUTE_Z];
						shadowAttributesPixelDeltaX = (blockShadowAttributes + shadowAttributesDeltaX) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaX[ATTRIBUTE_Z]) - shadowAttributesRow;
						shadowAttributesPixelDeltaY = (blockShadowAttributes + shadowAttributesDeltaY) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaY[ATTRIBUTE_Z]) - shadowAttributesRow;
					}
				}

				Color *textureBuffer;
				int32 textureWidthMask;
				int32 textureHeightMask;
				int32 textureWidthExponent;

				if constexpr (hasTexture)
				{
					int32 mipLevelU;
					int32 mipLevelV;
					RasterizerMath::GetMipLevel(attributesPixelDeltaX[ATTRIBUTE_U], attributesPixelDeltaX[ATTRIBUTE_V], attributesPixelDeltaY[ATTRIBUTE_U], attributesPixelDeltaY[ATTRIBUTE_V], RenderStates.Texture->MipLevels, mipLevelU, mipLevelV);

					textureBuffer = RenderStates.Texture->GetMipMap(mipLevelU, mipLevelV);
					textureWidthMask = (RenderStates.Texture->Width >> mipLevelU) - 1;
					textureHeightMask = (RenderStates.Texture->Height >> mipLevelV) - 1;
					textureWidthExponent = Math::GetExponent(RenderStates.Texture->Width) - mipLevelU;

					if (mipLevelU | mipLevelV)
					{
						float du = MipLevels[mipLevelU];
						float dv = MipLevels[mipLevelV];
						attributesRow[ATTRIBUTE_U] *= du;
						attributesRow[ATTRIBUTE_V] *= dv;
						attributesPixelDeltaX[ATTRIBUTE_U] *= du;
						attributesPixelDeltaX[ATTRIBUTE_V] *= dv;
						attributesPixelDeltaY[ATTRIBUTE_U] *= du;
						attributesPixelDeltaY[ATTRIBUTE_V] *= dv;
					}
				}

				vfloat8 attributeZ = vfloat8(attributesRow[ATTRIBUTE_Z]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_Z]) * Delta1To8MultiplierF;
				vfloat8 attributeU;
				vfloat8 attributeV;
				vfloat8 attributeR;
				vfloat8 attributeG;
				vfloat8 attributeB;
				vfloat8 attributeSpecularR;
				vfloat8 attributeSpecularG;
				vfloat8 attributeSpecularB;
				vfloat8 attributeShadowX;
				vfloat8 attributeShadowY;
				vfloat8 attributeShadowZ;
				vfloat8 attributeShadowR;
				vfloat8 attributeShadowG;
				vfloat8 attributeShadowB;
				if constexpr (hasTexture) attributeU = vfloat8(attributesRow[ATTRIBUTE_U]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_U]) * Delta1To8MultiplierF;
				if constexpr (hasTexture) attributeV = vfloat8(attributesRow[ATTRIBUTE_V]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_V]) * Delta1To8MultiplierF;
				if constexpr (hasColor) attributeR = vfloat8(attributesRow[ATTRIBUTE_R]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_R]) * Delta1To8MultiplierF;
				if constexpr (hasColor) attributeG = vfloat8(attributesRow[ATTRIBUTE_G]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_G]) * Delta1To8MultiplierF;
				if constexpr (hasColor) attributeB = vfloat8(attributesRow[ATTRIBUTE_B]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_B]) * Delta1To8MultiplierF;
				if constexpr (hasSpecular) attributeSpecularR = vfloat8(attributesBRow[ATTRIBUTE_SPECULAR_R]) + vfloat8(attributesBPixelDeltaX[ATTRIBUTE_SPECULAR_R]) * Delta1To8MultiplierF;
				if constexpr (hasSpecular) attributeSpecularG = vfloat8(attributesBRow[ATTRIBUTE_SPECULAR_G]) + vfloat8(attributesBPixelDeltaX[ATTRIBUTE_SPECULAR_G]) * Delta1To8MultiplierF;
				if constexpr (hasSpecular) attributeSpecularB = vfloat8(attributesBRow[ATTRIBUTE_SPECULAR_B]) + vfloat8(attributesBPixelDeltaX[ATTRIBUTE_SPECULAR_B]) * Delta1To8MultiplierF;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowX = vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_X]) + vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_X]) * Delta1To8MultiplierF;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowY = vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_Y]) + vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_Y]) * Delta1To8MultiplierF;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowZ = vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_Z]) + vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_Z]) * Delta1To8MultiplierF;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowR = vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_R]) + vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_R]) * Delta1To8MultiplierF;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowG = vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_G]) + vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_G]) * Delta1To8MultiplierF;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowB = vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_B]) + vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_B]) * Delta1To8MultiplierF;

				vfix8 edgeA = blockEdgeA;
				vfix8 edgeB = blockEdgeB;
				vfix8 edgeC = blockEdgeC;

				for (int32 i = 0; i < 8; i++)
				{
					DrawPixelRow<zEnable, zWriteEnable, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, shadowMapFunc, shadowMapProjection>(
						(vuint8*)frameBuffer,
						(vfloat8*)depthBuffer,
						VectorMath::CmpGt(edgeA, vfix8()) & VectorMath::CmpGt(edgeB, vfix8()) & VectorMath::CmpGt(edgeC, vfix8()),
						attributeZ,
						attributeU,
						attributeV,
						attributeR,
						attributeG,
						attributeB,
						attributeSpecularR,
						attributeSpecularG,
						attributeSpecularB,
						attributeShadowX,
						attributeShadowY,
						attributeShadowZ,
						attributeShadowR,
						attributeShadowG,
						attributeShadowB,
						hasTexture ? textureBuffer : nullptr,
						hasTexture ? textureWidthMask : 0,
						hasTexture ? textureHeightMask : 0,
						hasTexture ? textureWidthExponent : 0,
						shadowMapFunc != ShadowMapFunc::None ? shadowMap : nullptr,
						shadowMapFunc != ShadowMapFunc::None ? shadowMapSize : 0,
						shadowMapFunc != ShadowMapFunc::None ? shadowMapSizeMask : 0,
						shadowMapFunc != ShadowMapFunc::None ? shadowMapSizeExponent : 0,
						blendMode == BlendMode::Alpha || blendMode == BlendMode::Add ? alpha : 0);

					attributeZ += attributesPixelDeltaY[ATTRIBUTE_Z];
					if constexpr (hasTexture) attributeU += attributesPixelDeltaY[ATTRIBUTE_U];
					if constexpr (hasTexture) attributeV += attributesPixelDeltaY[ATTRIBUTE_V];
					if constexpr (hasColor) attributeR += attributesPixelDeltaY[ATTRIBUTE_R];
					if constexpr (hasColor) attributeG += attributesPixelDeltaY[ATTRIBUTE_G];
					if constexpr (hasColor) attributeB += attributesPixelDeltaY[ATTRIBUTE_B];
					if constexpr (hasSpecular) attributeSpecularR += attributesBPixelDeltaY[ATTRIBUTE_SPECULAR_R];
					if constexpr (hasSpecular) attributeSpecularG += attributesBPixelDeltaY[ATTRIBUTE_SPECULAR_G];
					if constexpr (hasSpecular) attributeSpecularB += attributesBPixelDeltaY[ATTRIBUTE_SPECULAR_B];
					if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowX += shadowAttributesPixelDeltaY[ATTRIBUTE_SHADOW_X];
					if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowY += shadowAttributesPixelDeltaY[ATTRIBUTE_SHADOW_Y];
					if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowZ += shadowAttributesPixelDeltaY[ATTRIBUTE_SHADOW_Z];
					if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowR += shadowAttributesPixelDeltaY[ATTRIBUTE_SHADOW_R];
					if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowG += shadowAttributesPixelDeltaY[ATTRIBUTE_SHADOW_G];
					if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowB += shadowAttributesPixelDeltaY[ATTRIBUTE_SHADOW_B];

					edgeA += edgeADeltaY;
					edgeB += edgeBDeltaY;
					edgeC += edgeCDeltaY;
					frameBuffer += stride;
					if constexpr (zEnable || zWriteEnable) depthBuffer += stride;
				}

				attributesRow += attributesDeltaY;
				if constexpr (hasSpecular) attributesBRow += attributesBDeltaY;
				if constexpr (shadowMapFunc != ShadowMapFunc::None) shadowAttributesRow += attributesDeltaY;

				frameBuffer += strideBlock;
				if constexpr (zEnable || zWriteEnable) depthBuffer += strideBlock;
			}
			else if (isDrawing)
			{
				// End of the current line of blocks.
				int32 strideRowEnd = boundingBox.Max.X + 1 - blockX;
				frameBuffer += strideRowEnd;
				if constexpr (zEnable || zWriteEnable) depthBuffer += strideRowEnd;
				break;
			}

			cornerEdgeA += cornerEdgeADeltaX;
			cornerEdgeB += cornerEdgeBDeltaX;
			cornerEdgeC += cornerEdgeCDeltaX;
			blockEdgeA += blockEdgeADeltaX;
			blockEdgeB += blockEdgeBDeltaX;
			blockEdgeC += blockEdgeCDeltaX;
			blockAttributes += blockAttributesDeltaX;
			if constexpr (hasSpecular) blockAttributesB += blockAttributesBDeltaX;
			if constexpr (shadowMapFunc != ShadowMapFunc::None) blockShadowAttributes += blockShadowAttributesDeltaX;
			frameBuffer += halfspace_mul(1);
			if constexpr (zEnable || zWriteEnable) depthBuffer += halfspace_mul(1);
		}

		cornerEdgeARow += cornerEdgeADeltaY;
		cornerEdgeBRow += cornerEdgeBDeltaY;
		cornerEdgeCRow += cornerEdgeCDeltaY;
		blockEdgeARow += blockEdgeADeltaY;
		blockEdgeBRow += blockEdgeBDeltaY;
		blockEdgeCRow += blockEdgeCDeltaY;
		blockAttributesRow += blockAttributesDeltaY;
		if constexpr (hasSpecular) blockAttributesBRow += blockAttributesBDeltaY;
		if constexpr (shadowMapFunc != ShadowMapFunc::None) blockShadowAttributesRow += blockShadowAttributesDeltaY;
		frameBuffer += strideRow;
		if constexpr (zEnable || zWriteEnable) depthBuffer += strideRow;
	}

	return true;
}
template<bool zEnable, bool zWriteEnable, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
__forceinline void FragmentRasterizer::DrawPixelRow(
	vuint8 *frameBuffer,
	vfloat8 *depthBuffer,
	vuint8 writeMask,
	const vfloat8 &attributeZ,
	const vfloat8 &attributeU,
	const vfloat8 &attributeV,
	const vfloat8 &attributeR,
	const vfloat8 &attributeG,
	const vfloat8 &attributeB,
	const vfloat8 &attributeSpecularR,
	const vfloat8 &attributeSpecularG,
	const vfloat8 &attributeSpecularB,
	const vfloat8 &attributeShadowX,
	const vfloat8 &attributeShadowY,
	const vfloat8 &attributeShadowZ,
	const vfloat8 &attributeShadowR,
	const vfloat8 &attributeShadowG,
	const vfloat8 &attributeShadowB,
	const Color *textureBuffer,
	int32 textureWidthMask,
	int32 textureHeightMask,
	int32 textureWidthExponent,
	const float *shadowMap,
	int32 shadowMapSize,
	int32 shadowMapSizeMask,
	int32 shadowMapSizeExponent,
	int32 alpha) const
{
	// This function is inlined by the compiler.
	// Because every combination of template parameters leads to a unique code path, this function
	// compiles to many different small functions, which can be considered "pixel shaders".
	// All if statements refer to template parameters, therefore this function is, indeed, 100% branchless.

	vuint8 pixels = vuint8(frameBuffer);
	vfloat8 depth;
	vuint8 color;
	vuint8 specular;
	vuint8 shadowColor;

	if constexpr (zEnable || zWriteEnable) depth = vfloat8(depthBuffer);
	if constexpr (hasColor) color = (vuint8)((vint8)attributeR << 16 | (vint8)attributeG << 8 | (vint8)attributeB);
	if constexpr (hasSpecular) specular = (vuint8)((vint8)attributeSpecularR << 16 | (vint8)attributeSpecularG << 8 | (vint8)attributeSpecularB);
	if constexpr (shadowMapFunc != ShadowMapFunc::None) shadowColor = (vuint8)((vint8)attributeShadowR << 16 | (vint8)attributeShadowG << 8 | (vint8)attributeShadowB);

	vushort16 alphaBroadcast;
	vushort16 alphaQ15;
	vushort16 oneMinusAlphaQ15;

	if constexpr (blendMode == BlendMode::Alpha || blendMode == BlendMode::Add)
	{
		alphaBroadcast = vushort16(alpha);
		alphaQ15 = vushort16(alpha << 7 | alpha >> 1);
		oneMinusAlphaQ15 = vushort16((255 - alpha) << 7 | (255 - alpha) >> 1);
	}

	if constexpr (zEnable)
	{
		writeMask &= VectorMath::CmpLe(depth, attributeZ);
	}

	vuint8 textureColor;

	if constexpr (hasTexture)
	{
		if constexpr (!textureFilteringEnable)
		{
			textureColor = vuint8::Read((uint32*)textureBuffer, (vint8)attributeU & textureWidthMask | ((vint8)attributeV & textureHeightMask) << textureWidthExponent, writeMask);

			if constexpr (blendMode == BlendMode::TransparencyKey)
			{
				writeMask &= VectorMath::CmpGt((vint8)textureColor, vint8());
			}
		}
		else
		{
			vfloat8 u = attributeU - .5f;
			vfloat8 v = attributeV - .5f;
			vint8 u0 = (vint8)u;
			vint8 v0 = (vint8)v;

			vuint8 du0 = (vuint8)(vint8)((u - (vfloat8)u0) * 255);
			vuint8 dv0 = (vuint8)(vint8)((v - (vfloat8)v0) * 255);
			vuint8 du1 = vuint8(255) - du0;
			vuint8 dv1 = vuint8(255) - dv0;

			vint8 u1 = (u0 + 1) & textureWidthMask;
			vint8 v1 = (v0 + 1) & textureHeightMask;
			u0 &= textureWidthMask;
			v0 &= textureHeightMask;

			vuint8 rgb00 = vuint8::Read((uint32*)textureBuffer, u0 | v0 << textureWidthExponent, writeMask);
			vuint8 rgb01 = vuint8::Read((uint32*)textureBuffer, u0 | v1 << textureWidthExponent, writeMask);
			vuint8 rgb10 = vuint8::Read((uint32*)textureBuffer, u1 | v0 << textureWidthExponent, writeMask);
			vuint8 rgb11 = vuint8::Read((uint32*)textureBuffer, u1 | v1 << textureWidthExponent, writeMask);

			vuint8 r00 = rgb00 >> 16 & 0xff;
			vuint8 g00 = rgb00 >> 8 & 0xff;
			vuint8 b00 = rgb00 & 0xff;

			vuint8 r01 = rgb01 >> 16 & 0xff;
			vuint8 g01 = rgb01 >> 8 & 0xff;
			vuint8 b01 = rgb01 & 0xff;

			vuint8 r10 = rgb10 >> 16 & 0xff;
			vuint8 g10 = rgb10 >> 8 & 0xff;
			vuint8 b10 = rgb10 & 0xff;

			vuint8 r11 = rgb11 >> 16 & 0xff;
			vuint8 g11 = rgb11 >> 8 & 0xff;
			vuint8 b11 = rgb11 & 0xff;

			vuint8 r = ((r00 * du1 + r10 * du0) * dv1 + (r01 * du1 + r11 * du0) * dv0) >> 16;
			vuint8 g = ((g00 * du1 + g10 * du0) * dv1 + (g01 * du1 + g11 * du0) * dv0) >> 16;
			vuint8 b = ((b00 * du1 + b10 * du0) * dv1 + (b01 * du1 + b11 * du0) * dv0) >> 16;

			textureColor = r << 16 | g << 8 | b;

			if constexpr (blendMode == BlendMode::TransparencyKey)
			{
				vuint8 m00 = (du1 * dv1) & VectorMath::CmpEq(rgb00, vuint8());
				vuint8 m01 = (du1 * dv0) & VectorMath::CmpEq(rgb01, vuint8());
				vuint8 m10 = (du0 * dv1) & VectorMath::CmpEq(rgb10, vuint8());
				vuint8 m11 = (du0 * dv0) & VectorMath::CmpEq(rgb11, vuint8());

				writeMask &= VectorMath::CmpLt((vint8)(m00 + m01 + m10 + m11), vint8(128 << 8));
			}
		}
	}

	// When rendering shadows without PCF, this value is a bitmask that selects either color, or shadowColor.
	vuint8 shadowMask = vuint8(-1);

	// When rendering PCF shadows, this value represents the lightness between 0 and 255.
	vbyte32 shadowAlpha;
	vbyte32 oneMinusShadowAlpha;

	if constexpr (shadowMapFunc != ShadowMapFunc::None)
	{
		vfloat8 u;
		vfloat8 v;
		vint8 u0;
		vint8 v0;
		vfloat8 depthCompare;
		vint8 faceOffset;

		if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
		{
			u = attributeShadowX;
			v = attributeShadowY;
			u0 = (vint8)u;
			v0 = (vint8)v;

			shadowMask = VectorMath::CmpEq(u0 >> shadowMapSizeExponent | v0 >> shadowMapSizeExponent, vint8());
			depthCompare = (attributeShadowZ - RenderStates.ShadowMapDepthBias) * RenderStates.Precomputed.InverseClipNear;
		}
		else if constexpr (shadowMapProjection == ShadowMapProjection::Cubemap)
		{
			vfloat8 absX = VectorMath::Abs(attributeShadowX);
			vfloat8 absY = VectorMath::Abs(attributeShadowY);
			vfloat8 absZ = VectorMath::Abs(attributeShadowZ);

			// Figure out which cubemap face is being rendered.
			vuint8 faceX = VectorMath::CmpGe(absX, absY) & VectorMath::CmpGt(absX, absZ);
			vuint8 faceY = VectorMath::CmpGe(absY, absX) & VectorMath::CmpGt(absY, absZ);
			vuint8 faceZ = VectorMath::CmpGe(absZ, absX) & VectorMath::CmpGt(absZ, absY);

			vuint8 signX = VectorMath::CmpGt(attributeShadowX, vfloat8());
			vuint8 signY = VectorMath::CmpGt(attributeShadowY, vfloat8());
			vuint8 signZ = VectorMath::CmpGt(attributeShadowZ, vfloat8());

			vfloat8 attributeShadowXReciprocal = attributeShadowX.Reciprocal;
			vfloat8 attributeShadowYReciprocal = attributeShadowY.Reciprocal;
			vfloat8 attributeShadowZReciprocal = attributeShadowZ.Reciprocal;

			u = VectorMath::Reinterpret(
				faceX & VectorMath::Reinterpret<vuint8>(-attributeShadowZ * attributeShadowXReciprocal) |
				faceY & ((~signY << 31) ^ VectorMath::Reinterpret<vuint8>(attributeShadowX * attributeShadowYReciprocal)) |
				faceZ & VectorMath::Reinterpret<vuint8>(attributeShadowX * attributeShadowZReciprocal)
			);

			v = VectorMath::Reinterpret(
				faceX & ((signX << 31) ^ VectorMath::Reinterpret<vuint8>(attributeShadowY * attributeShadowXReciprocal)) |
				faceY & VectorMath::Reinterpret<vuint8>(attributeShadowZ * attributeShadowYReciprocal) |
				faceZ & ((signZ << 31) ^ VectorMath::Reinterpret<vuint8>(attributeShadowY * attributeShadowZReciprocal))
			);

			u = (u + 1) * (float)(shadowMapSize >> 1);
			v = (v + 1) * (float)(shadowMapSize >> 1);

			if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				// Offset texels by half only for shadow cubemaps. In normal shadow maps, this was done in the vertex shader.
				u -= .5f;
				v -= .5f;
			}

			u0 = (vint8)u;
			v0 = (vint8)v;

			vfloat8 w = VectorMath::Reinterpret(
				faceX & ((~signX << 31) ^ VectorMath::Reinterpret<vuint8>(attributeShadowX)) |
				faceY & ((~signY << 31) ^ VectorMath::Reinterpret<vuint8>(attributeShadowY)) |
				faceZ & ((~signZ << 31) ^ VectorMath::Reinterpret<vuint8>(attributeShadowZ))
			);

			depthCompare = (w - RenderStates.ShadowMapDepthBias) * RenderStates.Precomputed.InverseClipNear;

			// Canonical cubemap face order:
			// x pos = 0
			// x neg = 1
			// y pos = 2
			// y neg = 3
			// z pos = 4
			// z neg = 5
			vuint8 faceIndex = VectorMath::Select(
				VectorMath::Select(
					VectorMath::Select(vuint8(5), vuint8(4), signZ),
					VectorMath::Select(vuint8(3), vuint8(2), signY),
					faceY
				),
				~signX >> 31,
				faceX
			);

			faceOffset = vint8(faceIndex << (shadowMapSizeExponent + shadowMapSizeExponent));
		}

		if constexpr (shadowMapFunc == ShadowMapFunc::Point)
		{
			vint8 offset = u0 & shadowMapSizeMask | (v0 & shadowMapSizeMask) << shadowMapSizeExponent;

			if constexpr (shadowMapProjection == ShadowMapProjection::Cubemap)
			{
				offset += faceOffset;
			}

			shadowMask &= VectorMath::CmpGt(depthCompare, vfloat8::Read(shadowMap, offset, writeMask).Reciprocal);
		}
		else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
		{
			vuint8 du0 = (vuint8)(vint8)((u - (vfloat8)u0) * 255);
			vuint8 dv0 = (vuint8)(vint8)((v - (vfloat8)v0) * 255);
			vuint8 du1 = vuint8(255) - du0;
			vuint8 dv1 = vuint8(255) - dv0;

			vint8 u1 = (u0 + 1) & shadowMapSizeMask;
			vint8 v1 = (v0 + 1) & shadowMapSizeMask;
			u0 &= shadowMapSizeMask;
			v0 &= shadowMapSizeMask;

			vint8 offset00 = u0 | v0 << shadowMapSizeExponent;
			vint8 offset01 = u0 | v1 << shadowMapSizeExponent;
			vint8 offset10 = u1 | v0 << shadowMapSizeExponent;
			vint8 offset11 = u1 | v1 << shadowMapSizeExponent;

			if constexpr (shadowMapProjection == ShadowMapProjection::Cubemap)
			{
				offset00 += faceOffset;
				offset01 += faceOffset;
				offset10 += faceOffset;
				offset11 += faceOffset;
			}

			vuint8 m00 = (du1 * dv1) & VectorMath::CmpGt(depthCompare, vfloat8::Read(shadowMap, offset00, writeMask).Reciprocal);
			vuint8 m01 = (du1 * dv0) & VectorMath::CmpGt(depthCompare, vfloat8::Read(shadowMap, offset01, writeMask).Reciprocal);
			vuint8 m10 = (du0 * dv1) & VectorMath::CmpGt(depthCompare, vfloat8::Read(shadowMap, offset10, writeMask).Reciprocal);
			vuint8 m11 = (du0 * dv0) & VectorMath::CmpGt(depthCompare, vfloat8::Read(shadowMap, offset11, writeMask).Reciprocal);

			shadowAlpha = VectorMath::Shuffle((vbyte32)((m00 + m01 + m10 + m11) >> 8), BroadcastByteToInt32Mask);
			shadowAlpha &= (vbyte32)shadowMask;
			oneMinusShadowAlpha = vbyte32(255) - shadowAlpha;
		}
	}

	vuint8 outputColor;

	if constexpr (hasTexture && hasColor && hasSpecular)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = VectorMath::Pack(
					VectorMath::AddSaturation8(color.Low8 * textureColor.Low8 >> 8, specular.Low8),
					VectorMath::AddSaturation8(color.High8 * textureColor.High8 >> 8, specular.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = VectorMath::Pack(
					VectorMath::Select(VectorMath::AddSaturation8(color.Low8 * textureColor.Low8 >> 8, specular.Low8), shadowColor.Low8 * textureColor.Low8 >> 8, shadowMask.Low8),
					VectorMath::Select(VectorMath::AddSaturation8(color.High8 * textureColor.High8 >> 8, specular.High8), shadowColor.High8 * textureColor.High8 >> 8, shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = VectorMath::Pack(
					((shadowColor.Low8 * textureColor.Low8 >> 8) * shadowAlpha.Low8 >> 8) + (VectorMath::AddSaturation8(color.Low8 * textureColor.Low8 >> 8, specular.Low8) * oneMinusShadowAlpha.Low8 >> 8),
					((shadowColor.High8 * textureColor.High8 >> 8) * shadowAlpha.High8 >> 8) + (VectorMath::AddSaturation8(color.High8 * textureColor.High8 >> 8, specular.High8) * oneMinusShadowAlpha.High8 >> 8)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = VectorMath::Pack(
				VectorMath::MultiplyFixQ15(VectorMath::AddSaturation8(color.Low8 * textureColor.Low8 >> 8, specular.Low8), alphaQ15) + VectorMath::MultiplyFixQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MultiplyFixQ15(VectorMath::AddSaturation8(color.High8 * textureColor.High8 >> 8, specular.High8), alphaQ15) + VectorMath::MultiplyFixQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8(color.Low8 * textureColor.Low8 >> 8, specular.Low8) * pixels.Low8 >> 8,
				VectorMath::AddSaturation8(color.High8 * textureColor.High8 >> 8, specular.High8) * pixels.High8 >> 8
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8(VectorMath::AddSaturation8(color.Low8 * textureColor.Low8 >> 8, specular.Low8) * alphaBroadcast >> 8, pixels.Low8),
				VectorMath::AddSaturation8(VectorMath::AddSaturation8(color.High8 * textureColor.High8 >> 8, specular.High8) * alphaBroadcast >> 8, pixels.High8)
			);
		}
	}
	else if constexpr (hasTexture && hasColor)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = VectorMath::Pack(
					color.Low8 * textureColor.Low8 >> 8,
					color.High8 * textureColor.High8 >> 8
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = VectorMath::Pack(
					VectorMath::Select(color.Low8, shadowColor.Low8, shadowMask.Low8) * textureColor.Low8 >> 8,
					VectorMath::Select(color.High8, shadowColor.High8, shadowMask.High8) * textureColor.High8 >> 8
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = VectorMath::Pack(
					((shadowColor.Low8 * textureColor.Low8 >> 8) * shadowAlpha.Low8 >> 8) + ((color.Low8 * textureColor.Low8 >> 8) * oneMinusShadowAlpha.Low8 >> 8),
					((shadowColor.High8 * textureColor.High8 >> 8) * shadowAlpha.High8 >> 8) + ((color.High8 * textureColor.High8 >> 8) * oneMinusShadowAlpha.High8 >> 8)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = VectorMath::Pack(
				VectorMath::MultiplyFixQ15(color.Low8 * textureColor.Low8 >> 8, alphaQ15) + VectorMath::MultiplyFixQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MultiplyFixQ15(color.High8 * textureColor.High8 >> 8, alphaQ15) + VectorMath::MultiplyFixQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = VectorMath::Pack(
				(color.Low8 * textureColor.Low8 >> 8) * pixels.Low8 >> 8,
				(color.High8 * textureColor.High8 >> 8) * pixels.High8 >> 8
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8((color.Low8 * textureColor.Low8 >> 8) * alphaBroadcast >> 8, pixels.Low8),
				VectorMath::AddSaturation8((color.High8 * textureColor.High8 >> 8) * alphaBroadcast >> 8, pixels.High8)
			);
		}
	}
	else if constexpr (hasTexture)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = textureColor;
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = VectorMath::Pack(
					VectorMath::Select(textureColor.Low8, (shadowColor.Low8 * textureColor.Low8 >> 8), shadowMask.Low8),
					VectorMath::Select(textureColor.High8, (shadowColor.High8 * textureColor.High8 >> 8), shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = VectorMath::Pack(
					((shadowColor.Low8 * textureColor.Low8 >> 8) * shadowAlpha.Low8 >> 8) + (textureColor.Low8 * oneMinusShadowAlpha.Low8 >> 8),
					((shadowColor.High8 * textureColor.High8 >> 8) * shadowAlpha.High8 >> 8) + (textureColor.High8 * oneMinusShadowAlpha.High8 >> 8)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = VectorMath::Pack(
				VectorMath::MultiplyFixQ15(textureColor.Low8, alphaQ15) + VectorMath::MultiplyFixQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MultiplyFixQ15(textureColor.High8, alphaQ15) + VectorMath::MultiplyFixQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = VectorMath::Pack(
				textureColor.Low8 * pixels.Low8 >> 8,
				textureColor.High8 * pixels.High8 >> 8
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8(textureColor.Low8 * alphaBroadcast >> 8, pixels.Low8),
				VectorMath::AddSaturation8(textureColor.High8 * alphaBroadcast >> 8, pixels.High8)
			);
		}
	}
	else if constexpr (hasColor && hasSpecular)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = VectorMath::Pack(
					VectorMath::AddSaturation8(color.Low8, specular.Low8),
					VectorMath::AddSaturation8(color.High8, specular.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = VectorMath::Pack(
					VectorMath::Select(VectorMath::AddSaturation8(color.Low8, specular.Low8), shadowColor.Low8, shadowMask.Low8),
					VectorMath::Select(VectorMath::AddSaturation8(color.High8, specular.High8), shadowColor.High8, shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = VectorMath::Pack(
					(shadowColor.Low8 * shadowAlpha.Low8 >> 8) + (VectorMath::AddSaturation8(color.Low8, specular.Low8) * oneMinusShadowAlpha.Low8 >> 8),
					(shadowColor.High8 * shadowAlpha.High8 >> 8) + (VectorMath::AddSaturation8(color.High8, specular.High8) * oneMinusShadowAlpha.High8 >> 8)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = VectorMath::Pack(
				VectorMath::MultiplyFixQ15(VectorMath::AddSaturation8(color.Low8, specular.Low8), alphaQ15) + VectorMath::MultiplyFixQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MultiplyFixQ15(VectorMath::AddSaturation8(color.High8, specular.High8), alphaQ15) + VectorMath::MultiplyFixQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8(color.Low8, specular.Low8) * pixels.Low8 >> 8,
				VectorMath::AddSaturation8(color.High8, specular.High8) * pixels.High8 >> 8
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8(VectorMath::AddSaturation8(color.Low8, specular.Low8) * alphaBroadcast >> 8, pixels.Low8),
				VectorMath::AddSaturation8(VectorMath::AddSaturation8(color.High8, specular.High8) * alphaBroadcast >> 8, pixels.High8)
			);
		}
	}
	else if constexpr (hasColor)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = color;
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = VectorMath::Pack(
					VectorMath::Select(color.Low8, shadowColor.Low8, shadowMask.Low8),
					VectorMath::Select(color.High8, shadowColor.High8, shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = VectorMath::Pack(
					(shadowColor.Low8 * shadowAlpha.Low8 >> 8) + (color.Low8 * oneMinusShadowAlpha.Low8 >> 8),
					(shadowColor.High8 * shadowAlpha.High8 >> 8) + (color.High8 * oneMinusShadowAlpha.High8 >> 8)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = VectorMath::Pack(
				VectorMath::MultiplyFixQ15(color.Low8, alphaQ15) + VectorMath::MultiplyFixQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MultiplyFixQ15(color.High8, alphaQ15) + VectorMath::MultiplyFixQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = VectorMath::Pack(
				color.Low8 * pixels.Low8 >> 8,
				color.High8 * pixels.High8 >> 8
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = VectorMath::Pack(
				VectorMath::AddSaturation8(color.Low8 * alphaBroadcast >> 8, pixels.Low8),
				VectorMath::AddSaturation8(color.High8 * alphaBroadcast >> 8, pixels.High8)
			);
		}
	}

	vuint8::Write(frameBuffer, outputColor, writeMask);

	if constexpr (zWriteEnable)
	{
		vfloat8::Write(depthBuffer, attributeZ, writeMask);
	}
}