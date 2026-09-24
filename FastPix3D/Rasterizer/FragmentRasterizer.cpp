#include "FragmentRasterizer.h"
#include "../Math/Box2i.h"
#include "../RasterizerMath.h"

bool FragmentRasterizer::DrawTriangle(const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	RasterizerVertex v1 = RasterizerVertex(_v1);
	RasterizerVertex v2 = RasterizerVertex(_v2);
	RasterizerVertex v3 = RasterizerVertex(_v3);

	// Transform vertices to view space.
	v1.Position = RenderStates.Precomputed.ModelViewMatrix * v1.Position;
	v2.Position = RenderStates.Precomputed.ModelViewMatrix * v2.Position;
	v3.Position = RenderStates.Precomputed.ModelViewMatrix * v3.Position;

	// Back-face culling.
	bool isFrontFace;
	if (RasterizerMath::IsTriangleCulled(RenderStates.CullMode, v1.Position, v2.Position, v3.Position, isFrontFace))
	{
		return false;
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
		return false;
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
		vfloat3 vertex1ShadowColor = vertex1Color;
		vfloat3 vertex2ShadowColor = vertex2Color;
		vfloat3 vertex3ShadowColor = vertex3Color;

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
						);

						if (hasSpecular)
						{
							specular = VectorMath::Pow(VectorMath::Max(vfloat3(
								v1.Normals.DotProduct((light.Precomputed.DirectionViewSpace - v1.Position.Normalize()).Normalize()),
								v2.Normals.DotProduct((light.Precomputed.DirectionViewSpace - v2.Position.Normalize()).Normalize()),
								v3.Normals.DotProduct((light.Precomputed.DirectionViewSpace - v3.Position.Normalize()).Normalize())
							), vfloat3()), RenderStates.SpecularExponent);
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
						);

						if (hasSpecular)
						{
							specular = VectorMath::Pow(VectorMath::Max(vfloat3(
								v1.Normals.DotProduct((distance1.Normalize() - v1.Position.Normalize()).Normalize()),
								v2.Normals.DotProduct((distance2.Normalize() - v2.Position.Normalize()).Normalize()),
								v3.Normals.DotProduct((distance3.Normalize() - v3.Position.Normalize()).Normalize())
							), vfloat3()), RenderStates.SpecularExponent);
						}

						break;
					}
					case LightType::Spot:
					{
						vfloat3 distance1 = light.Precomputed.PositionViewSpace - v1.Position;
						vfloat3 distance2 = light.Precomputed.PositionViewSpace - v2.Position;
						vfloat3 distance3 = light.Precomputed.PositionViewSpace - v3.Position;

						vfloat3 direction1 = distance1.Normalize();
						vfloat3 direction2 = distance2.Normalize();
						vfloat3 direction3 = distance3.Normalize();

						vfloat3 cone = VectorMath::Max((vfloat3(
							direction1.DotProduct(light.Precomputed.DirectionViewSpace),
							direction2.DotProduct(light.Precomputed.DirectionViewSpace),
							direction3.DotProduct(light.Precomputed.DirectionViewSpace)
						) - light.Precomputed.ConeAngleCos) * light.Precomputed.ConeAngleScale, vfloat3());

						diffuse = vfloat3(
							distance1.DotProduct(v1.Normals) * distance1.InverseSquaredLength,
							distance2.DotProduct(v2.Normals) * distance2.InverseSquaredLength,
							distance3.DotProduct(v3.Normals) * distance3.InverseSquaredLength
						) * cone;

						if (hasSpecular)
						{
							specular = VectorMath::Pow(VectorMath::Max(vfloat3(
								v1.Normals.DotProduct((direction1 - v1.Position.Normalize()).Normalize()),
								v2.Normals.DotProduct((direction2 - v2.Position.Normalize()).Normalize()),
								v3.Normals.DotProduct((direction3 - v3.Position.Normalize()).Normalize())
							), vfloat3()), RenderStates.SpecularExponent) * cone;
						}

						break;
					}
				}

				diffuse = VectorMath::Max(diffuse, vfloat3());

				vertex1Color += light.Precomputed.ColorIntensity * VectorMath::Shuffle<0, 0, 0>(diffuse);
				vertex2Color += light.Precomputed.ColorIntensity * VectorMath::Shuffle<1, 1, 1>(diffuse);
				vertex3Color += light.Precomputed.ColorIntensity * VectorMath::Shuffle<2, 2, 2>(diffuse);

				if (hasSpecular)
				{
					v1.Specular += light.Precomputed.ColorSpecularIntensity * VectorMath::Shuffle<0, 0, 0>(specular);
					v2.Specular += light.Precomputed.ColorSpecularIntensity * VectorMath::Shuffle<1, 1, 1>(specular);
					v3.Specular += light.Precomputed.ColorSpecularIntensity * VectorMath::Shuffle<2, 2, 2>(specular);
				}

				if (RenderStates.ShadowMapFunc != ShadowMapFunc::None && i != RenderStates.ShadowLightIndex)
				{
					vertex1ShadowColor += light.Precomputed.ColorIntensity * VectorMath::Shuffle<0, 0, 0>(diffuse);
					vertex2ShadowColor += light.Precomputed.ColorIntensity * VectorMath::Shuffle<1, 1, 1>(diffuse);
					vertex3ShadowColor += light.Precomputed.ColorIntensity * VectorMath::Shuffle<2, 2, 2>(diffuse);
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

	bool hasColor = !RenderStates.TextureEnable || !RenderStates.Texture || hasSpecular || !(v1.Color > 254) || !(v2.Color > 254) || !(v3.Color > 254);

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		return DrawClippedTriangle(v1, v2, v3, hasColor, hasSpecular);
	}
	else if (vertex1Visible && vertex2Visible)
	{
		RasterizerVertex v23;
		RasterizerVertex v31;

		ClipEdges(hasColor, hasSpecular, v2, v3, v3, v1, v23, v31);
		return DrawClippedTriangle(v31, v1, v23, hasColor, hasSpecular) | DrawClippedTriangle(v1, v2, v23, hasColor, hasSpecular);
	}
	else if (vertex2Visible && vertex3Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v31;

		ClipEdges(hasColor, hasSpecular, v1, v2, v3, v1, v12, v31);
		return DrawClippedTriangle(v3, v31, v2, hasColor, hasSpecular) | DrawClippedTriangle(v12, v2, v31, hasColor, hasSpecular);
	}
	else if (vertex1Visible && vertex3Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v23;

		ClipEdges(hasColor, hasSpecular, v1, v2, v2, v3, v12, v23);
		return DrawClippedTriangle(v1, v12, v23, hasColor, hasSpecular) | DrawClippedTriangle(v3, v1, v23, hasColor, hasSpecular);
	}
	else if (vertex1Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v31;

		ClipEdges(hasColor, hasSpecular, v1, v2, v3, v1, v12, v31);
		return DrawClippedTriangle(v1, v12, v31, hasColor, hasSpecular);
	}
	else if (vertex2Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v23;

		ClipEdges(hasColor, hasSpecular, v1, v2, v2, v3, v12, v23);
		return DrawClippedTriangle(v2, v23, v12, hasColor, hasSpecular);
	}
	else if (vertex3Visible)
	{
		RasterizerVertex v23;
		RasterizerVertex v31;

		ClipEdges(hasColor, hasSpecular, v2, v3, v3, v1, v23, v31);
		return DrawClippedTriangle(v3, v31, v23, hasColor, hasSpecular);
	}
	else
	{
		return false;
	}
}

void FragmentRasterizer::ClipEdges(bool hasColor, bool hasSpecular, const RasterizerVertex &edge1a, const RasterizerVertex &edge1b, const RasterizerVertex &edge2a, const RasterizerVertex &edge2b, RasterizerVertex &intersection1, RasterizerVertex &intersection2) const
{
	// When a triangle intersects the near clipping plane, two intersection points need to be computed.

	float t1 = (RenderStates.ClipNear - edge1a.Position.Z) / (edge1b.Position.Z - edge1a.Position.Z);
	float t2 = (RenderStates.ClipNear - edge2a.Position.Z) / (edge2b.Position.Z - edge2a.Position.Z);

	intersection1.Position = VectorMath::MulAdd(edge1b.Position - edge1a.Position, vfloat3(t1), edge1a.Position);
	intersection2.Position = VectorMath::MulAdd(edge2b.Position - edge2a.Position, vfloat3(t2), edge2a.Position);

	if (RenderStates.TextureEnable && RenderStates.Texture)
	{
		intersection1.TextureCoordinates = edge1a.TextureCoordinates + (edge1b.TextureCoordinates - edge1a.TextureCoordinates) * t1;
		intersection2.TextureCoordinates = edge2a.TextureCoordinates + (edge2b.TextureCoordinates - edge2a.TextureCoordinates) * t2;
	}

	if (hasColor)
	{
		intersection1.Color = VectorMath::MulAdd(edge1b.Color - edge1a.Color, vfloat3(t1), edge1a.Color);
		intersection2.Color = VectorMath::MulAdd(edge2b.Color - edge2a.Color, vfloat3(t2), edge2a.Color);
	}

	if (hasSpecular)
	{
		intersection1.Specular = VectorMath::MulAdd(edge1b.Specular - edge1a.Specular, vfloat3(t1), edge1a.Specular);
		intersection2.Specular = VectorMath::MulAdd(edge2b.Specular - edge2a.Specular, vfloat3(t2), edge2a.Specular);
	}

	if (RenderStates.ShadowMapFunc != ShadowMapFunc::None)
	{
		intersection1.ShadowXyz = VectorMath::MulAdd(edge1b.ShadowXyz - edge1a.ShadowXyz, vfloat3(t1), edge1a.ShadowXyz);
		intersection2.ShadowXyz = VectorMath::MulAdd(edge2b.ShadowXyz - edge2a.ShadowXyz, vfloat3(t2), edge2a.ShadowXyz);

		intersection1.ShadowColor = VectorMath::MulAdd(edge1b.ShadowColor - edge1a.ShadowColor, vfloat3(t1), edge1a.ShadowColor);
		intersection2.ShadowColor = VectorMath::MulAdd(edge2b.ShadowColor - edge2a.ShadowColor, vfloat3(t2), edge2a.ShadowColor);
	}
}
bool FragmentRasterizer::DrawClippedTriangle(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3, bool hasColor, bool hasSpecular) const
{
	// Project vertices to screen space.
	v1.Position = RasterizerMath::Project(v1.Position, RenderStates.Precomputed.ProjectionScale);
	v2.Position = RasterizerMath::Project(v2.Position, RenderStates.Precomputed.ProjectionScale);
	v3.Position = RasterizerMath::Project(v3.Position, RenderStates.Precomputed.ProjectionScale);

	if (RasterizerMath::IsTriangleOutsideClipVolume(v1.Position, v2.Position, v3.Position))
	{
		// Triangle is completely outside of the view frustum.
		return false;
	}

	// When using template parameters for render states, the compiler will evaluate branches at compile time.
	// Branches in hot loops are extremely expensive and are implemented using template parameters, or SIMD binary logic & write masks.

#ifdef DEBUG_ONE_PIXELSHADER
	// Reduce compilation time from 2 minutes to seconds when debugging by only compiling the most relevant pixel shaders.
	if (RenderStates.TextureEnable && RenderStates.Texture)
	{
		switch (RenderStates.DepthMode)
		{
			case DepthMode::None:
				DrawClippedTriangle<DepthMode::None, true, false, false, false, BlendMode::None, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
				break;
			case DepthMode::Read:
				DrawClippedTriangle<DepthMode::Read, true, false, false, false, BlendMode::None, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
				break;
			case DepthMode::ReadWrite:
				DrawClippedTriangle<DepthMode::ReadWrite, true, false, false, false, BlendMode::None, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
				break;
		}
	}
#else
	if (hasColor)
	{
		if (hasSpecular)
		{
			DrawClippedTriangle<true, true>(v1, v2, v3);
		}
		else
		{
			DrawClippedTriangle<true, false>(v1, v2, v3);
		}
	}
	else
	{
		DrawClippedTriangle<false, false>(v1, v2, v3);
	}
#endif

	return true;
}
template<bool hasColor, bool hasSpecular>
void FragmentRasterizer::DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const
{
	if (RenderStates.TextureEnable && RenderStates.Texture)
	{
		if (RenderStates.TextureFilteringEnable)
		{
			DrawClippedTriangle<true, true, hasColor, hasSpecular>(v1, v2, v3);
		}
		else
		{
			DrawClippedTriangle<true, false, hasColor, hasSpecular>(v1, v2, v3);
		}
	}
	else
	{
		DrawClippedTriangle<false, false, hasColor, hasSpecular>(v1, v2, v3);
	}
}
template<bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular>
void FragmentRasterizer::DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const
{
	switch (RenderStates.DepthMode)
	{
		case DepthMode::None:
			DrawClippedTriangle<DepthMode::None, hasTexture, textureFilteringEnable, hasColor, hasSpecular>(v1, v2, v3);
			break;
		case DepthMode::Read:
			DrawClippedTriangle<DepthMode::Read, hasTexture, textureFilteringEnable, hasColor, hasSpecular>(v1, v2, v3);
			break;
		case DepthMode::ReadWrite:
			DrawClippedTriangle<DepthMode::ReadWrite, hasTexture, textureFilteringEnable, hasColor, hasSpecular>(v1, v2, v3);
			break;
	}
}
template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular>
void FragmentRasterizer::DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const
{
	switch (RenderStates.BlendMode)
	{
		case BlendMode::None:
			DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::None>(v1, v2, v3);
			break;
		case BlendMode::TransparencyKey:
			if constexpr (hasTexture)
			{
				DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::TransparencyKey>(v1, v2, v3);
			}
			else
			{
				DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::None>(v1, v2, v3);
			}
			break;
		case BlendMode::Alpha:
			DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Alpha>(v1, v2, v3);
			break;
		case BlendMode::Add:
			DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Add>(v1, v2, v3);
			break;
		case BlendMode::Multiply:
			DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, BlendMode::Multiply>(v1, v2, v3);
			break;
	}
}
template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode>
void FragmentRasterizer::DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const
{
	switch (RenderStates.ShadowMapFunc)
	{
		case ShadowMapFunc::None:
			DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, ShadowMapFunc::None, ShadowMapProjection::Perspective>(v1, v2, v3);
			break;
		case ShadowMapFunc::Point:
			if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
			{
				DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, ShadowMapFunc::Point, ShadowMapProjection::Perspective>(v1, v2, v3);
			}
			else
			{
				DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, ShadowMapFunc::Point, ShadowMapProjection::Cubemap>(v1, v2, v3);
			}
			break;
		case ShadowMapFunc::Pcf:
			if (RenderStates.ShadowMapProjection == ShadowMapProjection::Perspective)
			{
				DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, ShadowMapFunc::Pcf, ShadowMapProjection::Perspective>(v1, v2, v3);
			}
			else
			{
				DrawClippedTriangle<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, ShadowMapFunc::Pcf, ShadowMapProjection::Cubemap>(v1, v2, v3);
			}
			break;
	}
}
template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
void FragmentRasterizer::DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const
{
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

	int32 partitionOffset = halfspace_mul(WorkPartition.GetOffset(halfspace_div(boundingBox.Min.Y)));
	int32 partitionIncrement = halfspace_mul(WorkPartition.ThreadCount);

	fix32 yStart = boundingBoxFix.Min.Y + fix(partitionOffset, fixExponent);

	if (boundingBox.Min.X >= boundingBox.Max.X || unfix(yStart, fixExponent) >= boundingBox.Max.Y)
	{
		return;
	}

	if constexpr (shadowMapFunc != ShadowMapFunc::None)
	{
		if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
		{
			float offset = (float)(RenderStates.ShadowMap.Width >> 1);
			float scale = RenderStates.ShadowLightZoom * offset;

			if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				// PCF is the equivalent of bilinear texture filtering on the shadow map, so offset texels by half.
				offset -= 0.5f;
			}

			float offset1 = offset * v1.ShadowXyz.Z;
			float offset2 = offset * v2.ShadowXyz.Z;
			float offset3 = offset * v3.ShadowXyz.Z;

			// X/Y = Shadow map UV coordinates (multiplied by texture resolution)
			// Z   = Depth to light source

			// Shadow maps of point lights need to be perspective corrected by both the camera space and the light space.

			v1.ShadowXyz.X = fmaf(v1.ShadowXyz.X, scale, offset1);
			v1.ShadowXyz.Y = fmaf(-v1.ShadowXyz.Y, scale, offset1);

			v2.ShadowXyz.X = fmaf(v2.ShadowXyz.X, scale, offset2);
			v2.ShadowXyz.Y = fmaf(-v2.ShadowXyz.Y, scale, offset2);

			v3.ShadowXyz.X = fmaf(v3.ShadowXyz.X, scale, offset3);
			v3.ShadowXyz.Y = fmaf(-v3.ShadowXyz.Y, scale, offset3);
		}

		v1.ShadowXyz *= v1.Position.Z;
		v2.ShadowXyz *= v2.Position.Z;
		v3.ShadowXyz *= v3.Position.Z;
	}


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

	int32 alpha = (int32)(RenderStates.Alpha * 256);

	// Edge equation
	vfix4 edgeDeltaX = vfix4(v1Screen.Y, v2Screen.Y, v3Screen.Y) - vfix4(v2Screen.Y, v3Screen.Y, v1Screen.Y);
	vfix4 edgeDeltaY = vfix4(v2Screen.X, v3Screen.X, v1Screen.X) - vfix4(v1Screen.X, v2Screen.X, v3Screen.X);
	vfix4 blockEdgeDeltaX = halfspace_mul(edgeDeltaX);
	vfix4 blockEdgeDeltaY = edgeDeltaY * partitionIncrement;

	// Top-left fill convention.
	vfix4 edgeStart = vfix4(fix(1, fixExponent)) & (vint4)(VectorMath::CmpLt(edgeDeltaX, vfix4()) | VectorMath::CmpEq(edgeDeltaX, vfix4()) & VectorMath::CmpGt(edgeDeltaY, vfix4()));

	// Edge equation at top-left corner of the bounding box.
	edgeStart += vfix4(
		(fix32)unfix((fix64)edgeDeltaX[0] * (boundingBoxFix.Min.X - v1Screen.X) + (fix64)edgeDeltaY[0] * (yStart - v1Screen.Y), fixExponent),
		(fix32)unfix((fix64)edgeDeltaX[1] * (boundingBoxFix.Min.X - v2Screen.X) + (fix64)edgeDeltaY[1] * (yStart - v2Screen.Y), fixExponent),
		(fix32)unfix((fix64)edgeDeltaX[2] * (boundingBoxFix.Min.X - v3Screen.X) + (fix64)edgeDeltaY[2] * (yStart - v3Screen.Y), fixExponent)
	);

	// Maximum edge values at the four corners of the block.
	// Lanes 0-2 contain edges A, B and C, lane 3 is ignored.
	vfix4 cornerEdgeMaxRow = edgeStart + VectorMath::Max(blockEdgeDeltaX, vfix4()) + VectorMath::Max(halfspace_mul(edgeDeltaY), vfix4());

	vfix8 blockEdgeARow = VectorMath::Broadcast<0>(edgeStart) + VectorMath::Broadcast<0>(edgeDeltaX) * Delta1To8Multiplier;
	vfix8 blockEdgeBRow = VectorMath::Broadcast<1>(edgeStart) + VectorMath::Broadcast<1>(edgeDeltaX) * Delta1To8Multiplier;
	vfix8 blockEdgeCRow = VectorMath::Broadcast<2>(edgeStart) + VectorMath::Broadcast<2>(edgeDeltaX) * Delta1To8Multiplier;

	vfix8 blockEdgeADeltaX = VectorMath::Broadcast<0>(blockEdgeDeltaX);
	vfix8 blockEdgeBDeltaX = VectorMath::Broadcast<1>(blockEdgeDeltaX);
	vfix8 blockEdgeCDeltaX = VectorMath::Broadcast<2>(blockEdgeDeltaX);
	vfix8 blockEdgeADeltaY = VectorMath::Broadcast<0>(blockEdgeDeltaY);
	vfix8 blockEdgeBDeltaY = VectorMath::Broadcast<1>(blockEdgeDeltaY);
	vfix8 blockEdgeCDeltaY = VectorMath::Broadcast<2>(blockEdgeDeltaY);

	vfix8 edgeADeltaY = VectorMath::Broadcast<0>(edgeDeltaY);
	vfix8 edgeBDeltaY = VectorMath::Broadcast<1>(edgeDeltaY);
	vfix8 edgeCDeltaY = VectorMath::Broadcast<2>(edgeDeltaY);

	// Barycentric determinants for interpolation of vertex attributes.
	float barycentricScale = (float)fix(1, fixExponent) / ((fix64)edgeDeltaX[1] * edgeDeltaY[2] - (fix64)edgeDeltaY[1] * edgeDeltaX[2]);
	vfloat2 barycentricL1 = (vfloat2)vfix2(edgeDeltaX[1], edgeDeltaY[1]) * barycentricScale;
	vfloat2 barycentricL2 = (vfloat2)vfix2(edgeDeltaX[2], edgeDeltaY[2]) * barycentricScale;

	// Barycentric coordinates at the top-left corner of the bounding box.
	float barycentricStartL1 = unfixf((boundingBoxFix.Min.X - v3Screen.X) * barycentricL1.X + (yStart - v3Screen.Y) * barycentricL1.Y, fixExponent);
	float barycentricStartL2 = unfixf((boundingBoxFix.Min.X - v3Screen.X) * barycentricL2.X + (yStart - v3Screen.Y) * barycentricL2.Y, fixExponent);

	vfloat8 v1Attributes = vfloat8(v1.Position.Z, hasTexture ? v1.TextureCoordinates.X : 0, hasTexture ? v1.TextureCoordinates.Y : 0, hasColor ? v1.Color.X : 0, hasColor ? v1.Color.Y : 0, hasColor ? v1.Color.Z : 0);
	vfloat8 v2Attributes = vfloat8(v2.Position.Z, hasTexture ? v2.TextureCoordinates.X : 0, hasTexture ? v2.TextureCoordinates.Y : 0, hasColor ? v2.Color.X : 0, hasColor ? v2.Color.Y : 0, hasColor ? v2.Color.Z : 0);
	vfloat8 v3Attributes = vfloat8(v3.Position.Z, hasTexture ? v3.TextureCoordinates.X : 0, hasTexture ? v3.TextureCoordinates.Y : 0, hasColor ? v3.Color.X : 0, hasColor ? v3.Color.Y : 0, hasColor ? v3.Color.Z : 0);
	vfloat8 v1AttributesRelative = v1Attributes - v3Attributes;
	vfloat8 v2AttributesRelative = v2Attributes - v3Attributes;

	// Vertex attributes at the top-left corner of the bounding box.
	vfloat8 blockAttributesRow = VectorMath::MulAdd(v1AttributesRelative, vfloat8(barycentricStartL1), VectorMath::MulAdd(v2AttributesRelative, vfloat8(barycentricStartL2), v3Attributes));

	// Attribute deltas per pixel.
	vfloat8 attributesDeltaX = VectorMath::MulAdd(v1AttributesRelative, vfloat8(barycentricL1.X), v2AttributesRelative * barycentricL2.X);
	vfloat8 attributesDeltaY = VectorMath::MulAdd(v1AttributesRelative, vfloat8(barycentricL1.Y), v2AttributesRelative * barycentricL2.Y);

	// Deltas per block for interpolation of vertex attributes.
	vfloat8 blockAttributesDeltaX = halfspace_mulf(attributesDeltaX);
	vfloat8 blockAttributesDeltaY = attributesDeltaY * (float)partitionIncrement;

	vfloat8 blockAttributesBRow;
	vfloat8 attributesBDeltaX;
	vfloat8 attributesBDeltaY;
	vfloat8 blockAttributesBDeltaX;
	vfloat8 blockAttributesBDeltaY;

	if constexpr (hasSpecular)
	{
		vfloat8 v1AttributesB = vfloat8(v1.Specular);
		vfloat8 v2AttributesB = vfloat8(v2.Specular);
		vfloat8 v3AttributesB = vfloat8(v3.Specular);
		vfloat8 v1AttributesBRelative = v1AttributesB - v3AttributesB;
		vfloat8 v2AttributesBRelative = v2AttributesB - v3AttributesB;

		blockAttributesBRow = VectorMath::MulAdd(v1AttributesBRelative, vfloat8(barycentricStartL1), VectorMath::MulAdd(v2AttributesBRelative, vfloat8(barycentricStartL2), v3AttributesB));
		attributesBDeltaX = VectorMath::MulAdd(v1AttributesBRelative, vfloat8(barycentricL1.X), v2AttributesBRelative * barycentricL2.X);
		attributesBDeltaY = VectorMath::MulAdd(v1AttributesBRelative, vfloat8(barycentricL1.Y), v2AttributesBRelative * barycentricL2.Y);

		blockAttributesBDeltaX = halfspace_mulf(attributesBDeltaX);
		blockAttributesBDeltaY = attributesBDeltaY * (float)partitionIncrement;
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
		vfloat8 v1ShadowAttributesRelative = v1ShadowAttributes - v3ShadowAttributes;
		vfloat8 v2ShadowAttributesRelative = v2ShadowAttributes - v3ShadowAttributes;

		blockShadowAttributesRow = VectorMath::MulAdd(v1ShadowAttributesRelative, vfloat8(barycentricStartL1), VectorMath::MulAdd(v2ShadowAttributesRelative, vfloat8(barycentricStartL2), v3ShadowAttributes));
		shadowAttributesDeltaX = VectorMath::MulAdd(v1ShadowAttributesRelative, vfloat8(barycentricL1.X), v2ShadowAttributesRelative * barycentricL2.X);
		shadowAttributesDeltaY = VectorMath::MulAdd(v1ShadowAttributesRelative, vfloat8(barycentricL1.Y), v2ShadowAttributesRelative * barycentricL2.Y);

		blockShadowAttributesDeltaX = halfspace_mulf(shadowAttributesDeltaX);
		blockShadowAttributesDeltaY = shadowAttributesDeltaY * (float)partitionIncrement;
	}

	int32 bufferStart = boundingBox.Min.X + unfix(yStart, fixExponent) * RenderStates.FrameBuffer.Width;
	Color *frameBuffer = RenderStates.FrameBuffer.GetBuffer<Color>(bufferStart);
	float *depthBuffer = RenderStates.DepthBuffer.GetBuffer<float>(bufferStart);

	float *shadowMap = RenderStates.ShadowMap.GetBuffer<float>();
	int32 shadowMapSize = RenderStates.ShadowMap.Width;
	int32 shadowMapSizeMask = shadowMapSize - 1;
	int32 shadowMapSizeExponent = RenderStates.ShadowMap.WidthExponent;

	int32 stride = RenderStates.FrameBuffer.Width; // Advance to next row in 8x8 block.
	int32 strideBlock = -halfspace_mul(RenderStates.FrameBuffer.Width); // Rewind to top-left corner of 8x8 block.
	int32 strideRow = RenderStates.FrameBuffer.Width * partitionIncrement - (boundingBox.Max.X + 1 - boundingBox.Min.X); // Advance to next row of blocks.

	for (int32 blockY = unfix(yStart, fixExponent); blockY < boundingBox.Max.Y; blockY += partitionIncrement)
	{
		bool isDrawing = false;
		vfix4 cornerEdgeMax = cornerEdgeMaxRow;
		vfix8 blockEdgeA = blockEdgeARow;
		vfix8 blockEdgeB = blockEdgeBRow;
		vfix8 blockEdgeC = blockEdgeCRow;
		vfloat8 blockAttributes = blockAttributesRow;
		vfloat8 blockShadowAttributes = blockShadowAttributesRow;
		vfloat8 blockAttributesB = blockAttributesBRow;

		for (int32 blockX = boundingBox.Min.X; blockX < boundingBox.Max.X; blockX += halfspace_mul(1))
		{
			// Each edge must be positive at at least one corner of the block.
			if (VectorMath::CmpGtMask(cornerEdgeMax, vfix4()) == 0xfff)
			{
				isDrawing = true;

				float d = 1 / blockAttributes[ATTRIBUTE_Z];
				vfloat8 attributesRow = blockAttributes * d;

				// Do perspective correction only once per block.
				vfloat8 attributesPixelDeltaX = VectorMath::NegMulAdd(attributesRow, vfloat8(attributesDeltaX[ATTRIBUTE_Z]), attributesDeltaX) * d;
				vfloat8 attributesPixelDeltaY = VectorMath::NegMulAdd(attributesRow, vfloat8(attributesDeltaY[ATTRIBUTE_Z]), attributesDeltaY) * d;

				vfloat8 attributesBRow;
				vfloat8 attributesBPixelDeltaX;
				vfloat8 attributesBPixelDeltaY;

				if constexpr (hasSpecular)
				{
					attributesBRow = blockAttributesB * d;
					attributesBPixelDeltaX = VectorMath::NegMulAdd(attributesBRow, vfloat8(attributesDeltaX[ATTRIBUTE_Z]), attributesBDeltaX) * d;
					attributesBPixelDeltaY = VectorMath::NegMulAdd(attributesBRow, vfloat8(attributesDeltaY[ATTRIBUTE_Z]), attributesBDeltaY) * d;
				}

				vfloat8 shadowAttributesRow;
				vfloat8 shadowAttributesPixelDeltaX;
				vfloat8 shadowAttributesPixelDeltaY;

				if constexpr (shadowMapFunc != ShadowMapFunc::None)
				{
					if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
					{
						float shadowD = 1 / blockShadowAttributes[ATTRIBUTE_SHADOW_Z];
						vfloat8 inverseShadowD = vfloat8(shadowD, shadowD, d, d, d, d);

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

						shadowAttributesRow = blockShadowAttributes * inverseShadowD;
						shadowAttributesPixelDeltaX = VectorMath::NegMulAdd(shadowAttributesRow, dx, shadowAttributesDeltaX) * inverseShadowD;
						shadowAttributesPixelDeltaY = VectorMath::NegMulAdd(shadowAttributesRow, dy, shadowAttributesDeltaY) * inverseShadowD;
					}
					else if constexpr (shadowMapProjection == ShadowMapProjection::Cubemap)
					{
						shadowAttributesRow = blockShadowAttributes * d;
						shadowAttributesPixelDeltaX = VectorMath::NegMulAdd(shadowAttributesRow, vfloat8(attributesDeltaX[ATTRIBUTE_Z]), shadowAttributesDeltaX) * d;
						shadowAttributesPixelDeltaY = VectorMath::NegMulAdd(shadowAttributesRow, vfloat8(attributesDeltaY[ATTRIBUTE_Z]), shadowAttributesDeltaY) * d;
					}
				}

				Color *textureBuffer = nullptr;
				int32 textureWidthMask = 0;
				int32 textureHeightMask = 0;
				int32 textureWidthExponent = 0;

				if constexpr (hasTexture)
				{
					// ATTRIBUTE_U and ATTRIBUTE_V are the inverse of the pixel size in texture space.
					//   - inverse pixel size = 32
					//   - Means, the pixel is 32 times smaller than a pixel on the screen
					//   - Result: MipMap level is 6

					// 1. Retrieve IEEE 754 exponent of the inverse pixel size.
					// 2. Find the Max(exponent) for both U and V.
					// 3. Compute MipMap level from the exponent.

					vint8 size = VectorMath::Max(
						VectorMath::Reinterpret<vint8>(attributesPixelDeltaX) & 0x7fffffff,
						VectorMath::Reinterpret<vint8>(attributesPixelDeltaY) & 0x7fffffff
					);

					vint8 mipLevel = VectorMath::Clamp((size >> 23) - 126, vint8(), vint8(RenderStates.Texture->MipLevels));
					int32 mipLevelU = mipLevel[ATTRIBUTE_U];
					int32 mipLevelV = mipLevel[ATTRIBUTE_V];

					textureBuffer = RenderStates.Texture->MipMaps[mipLevelU][mipLevelV];
					textureWidthMask = (RenderStates.Texture->Width >> mipLevelU) - 1;
					textureHeightMask = (RenderStates.Texture->Height >> mipLevelV) - 1;
					textureWidthExponent = RenderStates.Texture->WidthExponent - mipLevelU;

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

					if constexpr (textureFilteringEnable)
					{
						attributesRow[ATTRIBUTE_U] -= .5f;
						attributesRow[ATTRIBUTE_V] -= .5f;
					}
				}

				vfloat8 attributeZ = VectorMath::MulAdd(vfloat8(attributesDeltaX[ATTRIBUTE_Z]), Delta1To8MultiplierF, vfloat8(blockAttributes[ATTRIBUTE_Z]));
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
				if constexpr (hasTexture) attributeU = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_U]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_U]));
				if constexpr (hasTexture) attributeV = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_V]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_V]));
				if constexpr (hasColor) attributeR = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_R]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_R]));
				if constexpr (hasColor) attributeG = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_G]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_G]));
				if constexpr (hasColor) attributeB = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_B]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_B]));
				if constexpr (hasSpecular) attributeSpecularR = VectorMath::MulAdd(vfloat8(attributesBPixelDeltaX[ATTRIBUTE_SPECULAR_R]), Delta1To8MultiplierF, vfloat8(attributesBRow[ATTRIBUTE_SPECULAR_R]));
				if constexpr (hasSpecular) attributeSpecularG = VectorMath::MulAdd(vfloat8(attributesBPixelDeltaX[ATTRIBUTE_SPECULAR_G]), Delta1To8MultiplierF, vfloat8(attributesBRow[ATTRIBUTE_SPECULAR_G]));
				if constexpr (hasSpecular) attributeSpecularB = VectorMath::MulAdd(vfloat8(attributesBPixelDeltaX[ATTRIBUTE_SPECULAR_B]), Delta1To8MultiplierF, vfloat8(attributesBRow[ATTRIBUTE_SPECULAR_B]));
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowX = VectorMath::MulAdd(vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_X]), Delta1To8MultiplierF, vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_X]));
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowY = VectorMath::MulAdd(vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_Y]), Delta1To8MultiplierF, vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_Y]));
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowZ = VectorMath::MulAdd(vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_Z]), Delta1To8MultiplierF, vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_Z]));
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowR = VectorMath::MulAdd(vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_R]), Delta1To8MultiplierF, vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_R]));
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowG = VectorMath::MulAdd(vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_G]), Delta1To8MultiplierF, vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_G]));
				if constexpr (shadowMapFunc != ShadowMapFunc::None) attributeShadowB = VectorMath::MulAdd(vfloat8(shadowAttributesPixelDeltaX[ATTRIBUTE_SHADOW_B]), Delta1To8MultiplierF, vfloat8(shadowAttributesRow[ATTRIBUTE_SHADOW_B]));

				vfix8 edgeA = blockEdgeA;
				vfix8 edgeB = blockEdgeB;
				vfix8 edgeC = blockEdgeC;

				for (int32 i = 0; i < 8; i++)
				{
					DrawPixelVector<depthMode, hasTexture, textureFilteringEnable, hasColor, hasSpecular, blendMode, shadowMapFunc, shadowMapProjection>(
						(vuint8*)frameBuffer,
						(vfloat8*)depthBuffer,
						VectorMath::CmpGt(edgeA | edgeB | edgeC, vfix8()),
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
						textureBuffer,
						textureWidthMask,
						textureHeightMask,
						textureWidthExponent,
						shadowMap,
						shadowMapSize,
						shadowMapSizeMask,
						shadowMapSizeExponent,
						alpha);

					attributeZ += attributesDeltaY[ATTRIBUTE_Z];
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
					if constexpr (depthMode != DepthMode::None) depthBuffer += stride;
				}

				frameBuffer += strideBlock;
				if constexpr (depthMode != DepthMode::None) depthBuffer += strideBlock;
			}
			else if (isDrawing)
			{
				// End of the current line of blocks.
				int32 strideRowEnd = boundingBox.Max.X + 1 - blockX;
				frameBuffer += strideRowEnd;
				if constexpr (depthMode != DepthMode::None) depthBuffer += strideRowEnd;
				break;
			}

			cornerEdgeMax += blockEdgeDeltaX;
			blockEdgeA += blockEdgeADeltaX;
			blockEdgeB += blockEdgeBDeltaX;
			blockEdgeC += blockEdgeCDeltaX;
			blockAttributes += blockAttributesDeltaX;
			if constexpr (hasSpecular) blockAttributesB += blockAttributesBDeltaX;
			if constexpr (shadowMapFunc != ShadowMapFunc::None) blockShadowAttributes += blockShadowAttributesDeltaX;
			frameBuffer += halfspace_mul(1);
			if constexpr (depthMode != DepthMode::None) depthBuffer += halfspace_mul(1);
		}

		cornerEdgeMaxRow += blockEdgeDeltaY;
		blockEdgeARow += blockEdgeADeltaY;
		blockEdgeBRow += blockEdgeBDeltaY;
		blockEdgeCRow += blockEdgeCDeltaY;
		blockAttributesRow += blockAttributesDeltaY;
		if constexpr (hasSpecular) blockAttributesBRow += blockAttributesBDeltaY;
		if constexpr (shadowMapFunc != ShadowMapFunc::None) blockShadowAttributesRow += blockShadowAttributesDeltaY;
		frameBuffer += strideRow;
		if constexpr (depthMode != DepthMode::None) depthBuffer += strideRow;
	}
}
template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
__forceinline void FragmentRasterizer::DrawPixelVector(
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

	if constexpr (depthMode != DepthMode::None)
	{
		writeMask &= VectorMath::CmpLe(vfloat8(depthBuffer), attributeZ);
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
			vint8 u0 = (vint8)attributeU;
			vint8 v0 = (vint8)attributeV;

			vuint8 du0 = (vuint8)(vint8)((attributeU - (vfloat8)u0) * 256);
			vuint8 dv0 = (vuint8)(vint8)((attributeV - (vfloat8)v0) * 256);

			vint8 u1 = (u0 + 1) & textureWidthMask;
			vint8 v1 = (v0 + 1) & textureHeightMask;
			u0 &= textureWidthMask;
			v0 &= textureHeightMask;

			vuint8 rgb00 = vuint8::Read((uint32*)textureBuffer, u0 | v0 << textureWidthExponent, writeMask);
			vuint8 rgb01 = vuint8::Read((uint32*)textureBuffer, u0 | v1 << textureWidthExponent, writeMask);
			vuint8 rgb10 = vuint8::Read((uint32*)textureBuffer, u1 | v0 << textureWidthExponent, writeMask);
			vuint8 rgb11 = vuint8::Read((uint32*)textureBuffer, u1 | v1 << textureWidthExponent, writeMask);

			vbyte32 du = VectorMath::Shuffle((vbyte32)du0, BroadcastByteToInt32Mask);
			vbyte32 dv = VectorMath::Shuffle((vbyte32)dv0, BroadcastByteToInt32Mask);

			vushort16 duQ15Low = du.Low8 << 7;
			vushort16 duQ15High = du.High8 << 7;
			vushort16 dvQ15Low = dv.Low8 << 7;
			vushort16 dvQ15High = dv.High8 << 7;

			vushort16 rgb00Low = rgb00.Low8;
			vushort16 rgb00High = rgb00.High8;
			vushort16 rgb01Low = rgb01.Low8;
			vushort16 rgb01High = rgb01.High8;

			vushort16 topLow = rgb00Low + VectorMath::MulQ15(rgb10.Low8 - rgb00Low, duQ15Low);
			vushort16 topHigh = rgb00High + VectorMath::MulQ15(rgb10.High8 - rgb00High, duQ15High);
			vushort16 bottomLow = rgb01Low + VectorMath::MulQ15(rgb11.Low8 - rgb01Low, duQ15Low);
			vushort16 bottomHigh = rgb01High + VectorMath::MulQ15(rgb11.High8 - rgb01High, duQ15High);

			textureColor = (vuint8)VectorMath::Pack(
				topLow + VectorMath::MulQ15(bottomLow - topLow, dvQ15Low),
				topHigh + VectorMath::MulQ15(bottomHigh - topHigh, dvQ15High)
			);

			if constexpr (blendMode == BlendMode::TransparencyKey)
			{
				vuint8 du1 = vuint8(256) - du0;
				vuint8 dv1 = vuint8(256) - dv0;

				vuint8 m00 = (du1 * dv1) & VectorMath::CmpEq(rgb00, vuint8());
				vuint8 m01 = (du1 * dv0) & VectorMath::CmpEq(rgb01, vuint8());
				vuint8 m10 = (du0 * dv1) & VectorMath::CmpEq(rgb10, vuint8());
				vuint8 m11 = (du0 * dv0) & VectorMath::CmpEq(rgb11, vuint8());

				writeMask &= VectorMath::CmpLt((vint8)(m00 + m01 + m10 + m11), 128 << 8);
			}
		}
	}

	// When rendering shadows without PCF, this value is a bitmask that selects either color, or shadowColor.
	vuint8 shadowMask = vuint8(-1);

	// When rendering PCF shadows, this value represents the lightness between 0 and 256 as Q15.
	vushort16 shadowAlphaQ15Low;
	vushort16 shadowAlphaQ15High;

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
			depthCompare = attributeShadowZ;
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

			depthCompare = VectorMath::Max(absX, VectorMath::Max(absY, absZ));

			u = VectorMath::Reinterpret(
				faceX & (VectorMath::Reinterpret<vuint8>(attributeShadowZ) ^ (signX << 31)) |
				faceY & VectorMath::Reinterpret<vuint8>(attributeShadowX) |
				faceZ & (VectorMath::Reinterpret<vuint8>(attributeShadowX) ^ (~signZ << 31))
			);

			v = VectorMath::Reinterpret(
				(faceX | faceZ) & (VectorMath::Reinterpret<vuint8>(attributeShadowY) ^ 0x80000000) |
				faceY & (VectorMath::Reinterpret<vuint8>(attributeShadowZ) ^ (~signY << 31))
			);

			vfloat8 scale = vfloat8((float)(shadowMapSize >> 1));
			vfloat8 projectedScale = scale * depthCompare.Reciprocal;
			u = VectorMath::MulAdd(u, projectedScale, scale);
			v = VectorMath::MulAdd(v, projectedScale, scale);

			if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				// Offset texels by half only for shadow cubemaps. In normal shadow maps, this was done in the vertex shader.
				u -= .5f;
				v -= .5f;
			}

			u0 = (vint8)u;
			v0 = (vint8)v;

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

			vuint8 shadowDepthMask = VectorMath::CmpGt(depthCompare * vfloat8::Read(shadowMap, offset, writeMask), vfloat8(1));

			if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
			{
				shadowMask &= shadowDepthMask;
			}
			else if constexpr (shadowMapProjection == ShadowMapProjection::Cubemap)
			{
				shadowMask = shadowDepthMask;
			}
		}
		else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
		{
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

			vfloat8 shadow00 = vfloat8::Read(shadowMap, offset00, writeMask);
			vfloat8 shadow01 = vfloat8::Read(shadowMap, offset01, writeMask);
			vfloat8 shadow10 = vfloat8::Read(shadowMap, offset10, writeMask);
			vfloat8 shadow11 = vfloat8::Read(shadowMap, offset11, writeMask);

			vuint8 du0 = (vuint8)(vint8)((u - (vfloat8)u0) * 256);
			vuint8 dv0 = (vuint8)(vint8)((v - (vfloat8)v0) * 256);
			vuint8 du1 = vuint8(256) - du0;
			vuint8 dv1 = vuint8(256) - dv0;

			if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
			{
				depthCompare = VectorMath::Max(depthCompare, vfloat8());
			}

			vfloat8 inverseDepthCompare = depthCompare.Reciprocal;
			vuint8 m00 = (du1 * dv1) & VectorMath::CmpGt(shadow00, inverseDepthCompare);
			vuint8 m01 = (du1 * dv0) & VectorMath::CmpGt(shadow01, inverseDepthCompare);
			vuint8 m10 = (du0 * dv1) & VectorMath::CmpGt(shadow10, inverseDepthCompare);
			vuint8 m11 = (du0 * dv0) & VectorMath::CmpGt(shadow11, inverseDepthCompare);

			vuint8 shadowAlphaQ15 = VectorMath::Min((m00 + m01 + m10 + m11) >> 1, 32767);

			if constexpr (shadowMapProjection == ShadowMapProjection::Perspective)
			{
				shadowAlphaQ15 &= shadowMask;
			}

			shadowAlphaQ15Low = (vushort16)VectorMath::Shuffle((vbyte32)shadowAlphaQ15, BroadcastShortToLow8Mask);
			shadowAlphaQ15High = (vushort16)VectorMath::Shuffle((vbyte32)shadowAlphaQ15, BroadcastShortToHigh8Mask);
		}
	}

	vuint8 pixels;
	vuint8 color;
	vuint8 specular;
	vuint8 shadowColor;

	if constexpr (blendMode == BlendMode::Alpha || blendMode == BlendMode::Add || blendMode == BlendMode::Multiply) pixels = vuint8(frameBuffer);
	if constexpr (hasColor) color = (vuint8)((vint8)attributeR << 16 | (vint8)attributeG << 8 | (vint8)attributeB);
	if constexpr (hasSpecular) specular = (vuint8)((vint8)attributeSpecularR << 16 | (vint8)attributeSpecularG << 8 | (vint8)attributeSpecularB);
	if constexpr (shadowMapFunc != ShadowMapFunc::None) shadowColor = (vuint8)((vint8)attributeShadowR << 16 | (vint8)attributeShadowG << 8 | (vint8)attributeShadowB);

	vushort16 alphaQ15;
	vushort16 oneMinusAlphaQ15;

	if constexpr (blendMode == BlendMode::Alpha)
	{
		alphaQ15 = vushort16(Math::Min(alpha << 7, 32767));
		oneMinusAlphaQ15 = vushort16(Math::Min((256 - alpha) << 7, 32767));
	}

	vuint8 outputColor;

	if constexpr (hasTexture && hasColor && hasSpecular)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::AddSaturated8(color.Low8 * textureColor.Low8 >> 8, specular.Low8),
					VectorMath::AddSaturated8(color.High8 * textureColor.High8 >> 8, specular.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::Select(VectorMath::AddSaturated8(color.Low8 * textureColor.Low8 >> 8, specular.Low8), shadowColor.Low8 * textureColor.Low8 >> 8, shadowMask.Low8),
					VectorMath::Select(VectorMath::AddSaturated8(color.High8 * textureColor.High8 >> 8, specular.High8), shadowColor.High8 * textureColor.High8 >> 8, shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::LerpQ15(VectorMath::AddSaturated8(color.Low8 * textureColor.Low8 >> 8, specular.Low8), shadowColor.Low8 * textureColor.Low8 >> 8, shadowAlphaQ15Low),
					VectorMath::LerpQ15(VectorMath::AddSaturated8(color.High8 * textureColor.High8 >> 8, specular.High8), shadowColor.High8 * textureColor.High8 >> 8, shadowAlphaQ15High)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::MulQ15(VectorMath::AddSaturated8(color.Low8 * textureColor.Low8 >> 8, specular.Low8), alphaQ15) + VectorMath::MulQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MulQ15(VectorMath::AddSaturated8(color.High8 * textureColor.High8 >> 8, specular.High8), alphaQ15) + VectorMath::MulQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8(VectorMath::AddSaturated8(color.Low8 * textureColor.Low8 >> 8, specular.Low8) * alpha >> 8, pixels.Low8),
				VectorMath::AddSaturated8(VectorMath::AddSaturated8(color.High8 * textureColor.High8 >> 8, specular.High8) * alpha >> 8, pixels.High8)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8(color.Low8 * textureColor.Low8 >> 8, specular.Low8) * pixels.Low8 >> 8,
				VectorMath::AddSaturated8(color.High8 * textureColor.High8 >> 8, specular.High8) * pixels.High8 >> 8
			);
		}
	}
	else if constexpr (hasTexture && hasColor)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = (vuint8)VectorMath::Pack(
					color.Low8 * textureColor.Low8 >> 8,
					color.High8 * textureColor.High8 >> 8
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::Select(color.Low8, shadowColor.Low8, shadowMask.Low8) * textureColor.Low8 >> 8,
					VectorMath::Select(color.High8, shadowColor.High8, shadowMask.High8) * textureColor.High8 >> 8
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::LerpQ15(color.Low8 * textureColor.Low8 >> 8, shadowColor.Low8 * textureColor.Low8 >> 8, shadowAlphaQ15Low),
					VectorMath::LerpQ15(color.High8 * textureColor.High8 >> 8, shadowColor.High8 * textureColor.High8 >> 8, shadowAlphaQ15High)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::MulQ15(color.Low8 * textureColor.Low8 >> 8, alphaQ15) + VectorMath::MulQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MulQ15(color.High8 * textureColor.High8 >> 8, alphaQ15) + VectorMath::MulQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8((color.Low8 * textureColor.Low8 >> 8) * alpha >> 8, pixels.Low8),
				VectorMath::AddSaturated8((color.High8 * textureColor.High8 >> 8) * alpha >> 8, pixels.High8)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = (vuint8)VectorMath::Pack(
				(color.Low8 * textureColor.Low8 >> 8) * pixels.Low8 >> 8,
				(color.High8 * textureColor.High8 >> 8) * pixels.High8 >> 8
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
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::Select(textureColor.Low8, (shadowColor.Low8 * textureColor.Low8 >> 8), shadowMask.Low8),
					VectorMath::Select(textureColor.High8, (shadowColor.High8 * textureColor.High8 >> 8), shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::LerpQ15(textureColor.Low8, shadowColor.Low8 * textureColor.Low8 >> 8, shadowAlphaQ15Low),
					VectorMath::LerpQ15(textureColor.High8, shadowColor.High8 * textureColor.High8 >> 8, shadowAlphaQ15High)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::MulQ15(textureColor.Low8, alphaQ15) + VectorMath::MulQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MulQ15(textureColor.High8, alphaQ15) + VectorMath::MulQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8(textureColor.Low8 * alpha >> 8, pixels.Low8),
				VectorMath::AddSaturated8(textureColor.High8 * alpha >> 8, pixels.High8)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = (vuint8)VectorMath::Pack(
				textureColor.Low8 * pixels.Low8 >> 8,
				textureColor.High8 * pixels.High8 >> 8
			);
		}
	}
	else if constexpr (hasColor && hasSpecular)
	{
		if constexpr (blendMode == BlendMode::None || blendMode == BlendMode::TransparencyKey)
		{
			if constexpr (shadowMapFunc == ShadowMapFunc::None)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::AddSaturated8(color.Low8, specular.Low8),
					VectorMath::AddSaturated8(color.High8, specular.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Point)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::Select(VectorMath::AddSaturated8(color.Low8, specular.Low8), shadowColor.Low8, shadowMask.Low8),
					VectorMath::Select(VectorMath::AddSaturated8(color.High8, specular.High8), shadowColor.High8, shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::LerpQ15(VectorMath::AddSaturated8(color.Low8, specular.Low8), shadowColor.Low8, shadowAlphaQ15Low),
					VectorMath::LerpQ15(VectorMath::AddSaturated8(color.High8, specular.High8), shadowColor.High8, shadowAlphaQ15High)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::MulQ15(VectorMath::AddSaturated8(color.Low8, specular.Low8), alphaQ15) + VectorMath::MulQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MulQ15(VectorMath::AddSaturated8(color.High8, specular.High8), alphaQ15) + VectorMath::MulQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8(VectorMath::AddSaturated8(color.Low8, specular.Low8) * alpha >> 8, pixels.Low8),
				VectorMath::AddSaturated8(VectorMath::AddSaturated8(color.High8, specular.High8) * alpha >> 8, pixels.High8)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8(color.Low8, specular.Low8) * pixels.Low8 >> 8,
				VectorMath::AddSaturated8(color.High8, specular.High8) * pixels.High8 >> 8
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
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::Select(color.Low8, shadowColor.Low8, shadowMask.Low8),
					VectorMath::Select(color.High8, shadowColor.High8, shadowMask.High8)
				);
			}
			else if constexpr (shadowMapFunc == ShadowMapFunc::Pcf)
			{
				outputColor = (vuint8)VectorMath::Pack(
					VectorMath::LerpQ15(color.Low8, shadowColor.Low8, shadowAlphaQ15Low),
					VectorMath::LerpQ15(color.High8, shadowColor.High8, shadowAlphaQ15High)
				);
			}
		}
		else if constexpr (blendMode == BlendMode::Alpha)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::MulQ15(color.Low8, alphaQ15) + VectorMath::MulQ15(pixels.Low8, oneMinusAlphaQ15),
				VectorMath::MulQ15(color.High8, alphaQ15) + VectorMath::MulQ15(pixels.High8, oneMinusAlphaQ15)
			);
		}
		else if constexpr (blendMode == BlendMode::Add)
		{
			outputColor = (vuint8)VectorMath::Pack(
				VectorMath::AddSaturated8(color.Low8 * alpha >> 8, pixels.Low8),
				VectorMath::AddSaturated8(color.High8 * alpha >> 8, pixels.High8)
			);
		}
		else if constexpr (blendMode == BlendMode::Multiply)
		{
			outputColor = (vuint8)VectorMath::Pack(
				color.Low8 * pixels.Low8 >> 8,
				color.High8 * pixels.High8 >> 8
			);
		}
	}

	vuint8::Write(frameBuffer, outputColor, writeMask);

	if constexpr (depthMode == DepthMode::ReadWrite)
	{
		vfloat8::Write(depthBuffer, attributeZ, writeMask);
	}
}