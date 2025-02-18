#include "Rasterizer.h"
#include "Vector2i.h"
#include "RasterizerMath.h"

static const int32 SubdivExponent = 4;
static const int32 Subdiv1 = 1 << SubdivExponent;
static const int32  SubdivMask = Subdiv1 - 1;
static constexpr float InvertedSubdiv1 = 1.0f / Subdiv1;

Rasterizer::Rasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics) :
	RenderStates(renderStates),
	Statistics(statistics)
{
}
void Rasterizer::DrawTriangle(const Matrix4f &worldSpace, const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	RasterizerVertex v1 = RasterizerVertex(_v1);
	RasterizerVertex v2 = RasterizerVertex(_v2);
	RasterizerVertex v3 = RasterizerVertex(_v3);

	// Transform vertices to world space.
	v1.Position = worldSpace * v1.Position;
	v2.Position = worldSpace * v2.Position;
	v3.Position = worldSpace * v3.Position;

	// Back-face culling.
	if (RasterizerMath::IsTriangleCulled(RenderStates.CullMode, v1.Position, v2.Position, v3.Position))
	{
		return;
	}

	// Clip along near clipping plane.
	bool vertex1Visible = v1.Position.Z > RenderStates.ClipNear;
	bool vertex2Visible = v2.Position.Z > RenderStates.ClipNear;
	bool vertex3Visible = v3.Position.Z > RenderStates.ClipNear;

	if (!vertex1Visible && !vertex2Visible && !vertex3Visible)
	{
		// All vertices are behind the near clipping plane -> early out.
		return;
	}

	// Transform normals to world space (only required for lighting and sphere mapping).
	if (RenderStates.LightsEnable || RenderStates.Texture && RenderStates.Texture->Type == TextureType::SphereMap)
	{
		Matrix4f worldSpaceRotationPart = worldSpace.RotationPart;
		v1.Normals = worldSpaceRotationPart * v1.Normals;
		v2.Normals = worldSpaceRotationPart * v2.Normals;
		v3.Normals = worldSpaceRotationPart * v3.Normals;
	}

	// Compute texture coordinates for sphere mapping.
	if (RenderStates.Texture && RenderStates.Texture->Type == TextureType::SphereMap)
	{
		v1.TextureCoordinates.X = .5f + v1.Normals.X * .5f;
		v1.TextureCoordinates.Y = .5f - v1.Normals.Y * .5f;
		v2.TextureCoordinates.X = .5f + v2.Normals.X * .5f;
		v2.TextureCoordinates.Y = .5f - v2.Normals.Y * .5f;
		v3.TextureCoordinates.X = .5f + v3.Normals.X * .5f;
		v3.TextureCoordinates.Y = .5f - v3.Normals.Y * .5f;
	}

	// Change vertex colors based on lighting.
	if (RenderStates.LightsEnable)
	{
		Vector3f vertex1Color = (Vector3f)RenderStates.AmbientLight;
		Vector3f vertex2Color = (Vector3f)RenderStates.AmbientLight;
		Vector3f vertex3Color = (Vector3f)RenderStates.AmbientLight;

		for (int32 i = 0; i < sizeof(RenderStates.Lights) / sizeof(Light); i++)
		{
			const Light *light = &RenderStates.Lights[i];

			if (light->Enabled)
			{
				Vector3f diffuse;

				switch (light->Type)
				{
					case LightType::Directional:
					{
						diffuse = Vector3f(
							RenderStates.Precomputed.Lights[i].CameraSpaceVector.DotProduct(v1.Normals),
							RenderStates.Precomputed.Lights[i].CameraSpaceVector.DotProduct(v2.Normals),
							RenderStates.Precomputed.Lights[i].CameraSpaceVector.DotProduct(v3.Normals)
						) * RenderStates.Precomputed.Lights[i].CameraSpaceVectorDotIntensity;

						break;
					}
					case LightType::Point:
					{
						Vector3f distance1 = RenderStates.Precomputed.Lights[i].PositionCameraSpace - v1.Position;
						Vector3f distance2 = RenderStates.Precomputed.Lights[i].PositionCameraSpace - v2.Position;
						Vector3f distance3 = RenderStates.Precomputed.Lights[i].PositionCameraSpace - v3.Position;

						diffuse = Vector3f(
							distance1.DotProduct(v1.Normals) / distance1.DotProduct(distance1),
							distance2.DotProduct(v2.Normals) / distance2.DotProduct(distance2),
							distance3.DotProduct(v3.Normals) / distance3.DotProduct(distance3)
						) * light->Intensity;

						break;
					}
					case LightType::Spot:
					{
						Vector3f distance1 = RenderStates.Precomputed.Lights[i].PositionCameraSpace - v1.Position;
						Vector3f distance2 = RenderStates.Precomputed.Lights[i].PositionCameraSpace - v2.Position;
						Vector3f distance3 = RenderStates.Precomputed.Lights[i].PositionCameraSpace - v3.Position;

						diffuse = Vector3f(
							distance1.DotProduct(v1.Normals) / distance1.DotProduct(distance1) * (1 - acosf(distance1.Normalize().DotProduct(RenderStates.Precomputed.Lights[i].CameraSpaceVector))),
							distance2.DotProduct(v2.Normals) / distance2.DotProduct(distance2) * (1 - acosf(distance2.Normalize().DotProduct(RenderStates.Precomputed.Lights[i].CameraSpaceVector))),
							distance3.DotProduct(v3.Normals) / distance3.DotProduct(distance3) * (1 - acosf(distance3.Normalize().DotProduct(RenderStates.Precomputed.Lights[i].CameraSpaceVector)))
						) * light->Intensity;

						break;
					}
					default:
						throw;
				}

				if (diffuse.X > 0) vertex1Color += (Vector3f)light->Color * diffuse.X;
				if (diffuse.Y > 0) vertex2Color += (Vector3f)light->Color * diffuse.Y;
				if (diffuse.Z > 0) vertex3Color += (Vector3f)light->Color * diffuse.Z;
			}
		}

		if (vertex1Color.X < 256) v1.Color.X *= vertex1Color.X / 256;
		if (vertex1Color.Y < 256) v1.Color.Y *= vertex1Color.Y / 256;
		if (vertex1Color.Z < 256) v1.Color.Z *= vertex1Color.Z / 256;
		if (vertex2Color.X < 256) v2.Color.X *= vertex2Color.X / 256;
		if (vertex2Color.Y < 256) v2.Color.Y *= vertex2Color.Y / 256;
		if (vertex2Color.Z < 256) v2.Color.Z *= vertex2Color.Z / 256;
		if (vertex3Color.X < 256) v3.Color.X *= vertex3Color.X / 256;
		if (vertex3Color.Y < 256) v3.Color.Y *= vertex3Color.Y / 256;
		if (vertex3Color.Z < 256) v3.Color.Z *= vertex3Color.Z / 256;
	}

	// Normalize near clipping plane to 1.0
	v1.Position *= RenderStates.Precomputed.InverseClipNear;
	v2.Position *= RenderStates.Precomputed.InverseClipNear;
	v3.Position *= RenderStates.Precomputed.InverseClipNear;

	bool rendered = false;
	bool hasColors =
		!RenderStates.Texture ||
		v1.Color.X != 255 || v1.Color.Y != 255 || v1.Color.Z != 255 ||
		v2.Color.X != 255 || v2.Color.Y != 255 || v2.Color.Z != 255 ||
		v3.Color.X != 255 || v3.Color.Y != 255 || v3.Color.Z != 255;

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		// All vertices are in front of the near clipping plane.
		rendered = DrawClippedTriangle(v1, v2, v3, hasColors);
	}
	else
	{
		// 1 or 2 vertices are in front of the near clipping plane.
		RasterizerVertex v12;
		RasterizerVertex v23;
		RasterizerVertex v31;

		v12.Position = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Position, v2.Position);
		v23.Position = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Position, v3.Position);
		v31.Position = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Position, v1.Position);

		if (RenderStates.Texture)
		{
			v12.TextureCoordinates = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.TextureCoordinates, v2.TextureCoordinates);
			v23.TextureCoordinates = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.TextureCoordinates, v3.TextureCoordinates);
			v31.TextureCoordinates = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.TextureCoordinates, v1.TextureCoordinates);
		}

		if (hasColors)
		{
			v12.Color = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Color, v2.Color);
			v23.Color = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Color, v3.Color);
			v31.Color = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Color, v1.Color);
		}

		if (vertex1Visible && vertex2Visible)
		{
			rendered = DrawClippedTriangle(v31, v1, v23, hasColors);
			rendered |= DrawClippedTriangle(v1, v2, v23, hasColors);
		}
		else if (vertex2Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v2, hasColors);
			rendered |= DrawClippedTriangle(v12, v2, v31, hasColors);
		}
		else if (vertex1Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v23, hasColors);
			rendered |= DrawClippedTriangle(v3, v1, v23, hasColors);
		}
		else if (vertex1Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v31, hasColors);
		}
		else if (vertex2Visible)
		{
			rendered = DrawClippedTriangle(v2, v23, v12, hasColors);
		}
		else if (vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v23, hasColors);
		}
	}

	if (rendered)
	{
		Statistics.RenderedTriangleCount++;
	}
}
bool Rasterizer::DrawClippedTriangle(const RasterizerVertex &v1, const RasterizerVertex &v2, const RasterizerVertex &v3, bool hasColors) const
{
	if (RenderStates.Texture)
	{
		if (hasColors)
		{
			return DrawClippedTriangleT<true, true>(v1, v2, v3);
		}
		else
		{
			return DrawClippedTriangleT<true, false>(v1, v2, v3);
		}
	}
	else
	{
		return DrawClippedTriangleT<false, true>(v1, v2, v3);
	}
}
template<bool hasTexture, bool hasColors>
bool Rasterizer::DrawClippedTriangleT(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3) const
{
	// Project vertices to screen space.
	Vector2i v1Screen = RasterizerMath::ProjectVertex(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v1.Position, RenderStates.Zoom);
	Vector2i v2Screen = RasterizerMath::ProjectVertex(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v2.Position, RenderStates.Zoom);
	Vector2i v3Screen = RasterizerMath::ProjectVertex(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v3.Position, RenderStates.Zoom);

	if (RasterizerMath::ClipTriangleScreenSpace(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v1Screen, v2Screen, v3Screen))
	{
		// All projected screen coordinates are ouside of the screen.
		return false;
	}

	// Sort vertices by screen Y coordinate, so that v1y < v2y < v3y.
	if (v1Screen.Y > v2Screen.Y)
	{
		std::swap(v1, v2);
		std::swap(v1Screen, v2Screen);
	}

	if (v2Screen.Y > v3Screen.Y)
	{
		std::swap(v2, v3);
		std::swap(v2Screen, v3Screen);

		if (v1Screen.Y > v2Screen.Y)
		{
			std::swap(v1, v2);
			std::swap(v1Screen, v2Screen);
		}
	}

	if (v1Screen.Y == v3Screen.Y)
	{
		// Triangle is a 1px horizontal line.
		return false;
	}

	// Store inverse of Z from here on.
	v1.Position.Z = 1 / v1.Position.Z;
	v2.Position.Z = 1 / v2.Position.Z;
	v3.Position.Z = 1 / v3.Position.Z;

	if (hasTexture)
	{
		// Scale texture coordinates by texture size.
		v1.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v2.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v3.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;

		// Texture coordinates must be positive.
		RasterizerMath::MakeTextureCoordinatesPositive(v1.TextureCoordinates, v2.TextureCoordinates, v3.TextureCoordinates);

		v1.TextureCoordinates.X *= RenderStates.Texture->Width * v1.Position.Z;
		v1.TextureCoordinates.Y *= RenderStates.Texture->Height * v1.Position.Z;
		v2.TextureCoordinates.X *= RenderStates.Texture->Width * v2.Position.Z;
		v2.TextureCoordinates.Y *= RenderStates.Texture->Height * v2.Position.Z;
		v3.TextureCoordinates.X *= RenderStates.Texture->Width * v3.Position.Z;
		v3.TextureCoordinates.Y *= RenderStates.Texture->Height * v3.Position.Z;
	}

	if (hasColors)
	{
		v1.Color *= v1.Position.Z;
		v2.Color *= v2.Position.Z;
		v3.Color *= v3.Position.Z;
	}

	/*
	  - v1y < v2y < v3y, and v2y == v13y
	  - v13 is the intersection on the longer vertical edge (between vertex 1 and 3) at the y-coordinate of vertex 2.
	  - The triangle is divided into two halves that are drawn separately.

		v1
		   /\
		  /  \
		 /    \
		/      \
	v2  \.......X <----- v13
		  \      \
			\     \
			  \    \
				\   \
				  \  \
					-   v3

	  - Delta* variables are the deltas per pixel.
		(computed using the line between v2..v13)
	  - Delta*Subdiv variables are the deltas per subdiv (16 pixels).
	*/

	float d = (float)(v2Screen.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);
	float v13z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d;
	Vector2f v13Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d;
	Vector3f v13Color = v1.Color + (v3.Color - v1.Color) * d;

	ScanlineParameters p;

	// Compute deltas per pixel and per subdiv (16 pixels).
	int32 v13x = v1Screen.X + (v3Screen.X - v1Screen.X) * (v2Screen.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);
	d = 1.0f / (v13x - v2Screen.X);
	p.DeltaZ = (v13z - v2.Position.Z) * d;
	p.DeltaZSubdiv = p.DeltaZ * Subdiv1;
	if (hasTexture) p.DeltaTextureSubdiv = (v13Texture - v2.TextureCoordinates) * (d * Subdiv1);
	if (hasColors) p.DeltaColorSubdiv = (v13Color - v2.Color) * (d * Subdiv1);

	int32 yStart = Math::Clamp(v1Screen.Y, 0, RenderStates.FrameBuffer.Height);
	int32 yEnd = Math::Clamp(v2Screen.Y, 0, RenderStates.FrameBuffer.Height);

	int32 workloadOffset;
	int32 workloadIncrement;
	RasterizerMath::GetWorkloadParameters(RenderStates.Workload, yStart, workloadOffset, workloadIncrement);

	for (p.Y = yStart + workloadOffset; p.Y < yEnd; p.Y += workloadIncrement)
	{
		float d1 = (float)(p.Y - v1Screen.Y) / (v2Screen.Y - v1Screen.Y);
		float d2 = (float)(p.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);

		p.V1.X = v1Screen.X + (v2Screen.X - v1Screen.X) * d1;
		p.V1.Z = v1.Position.Z + (v2.Position.Z - v1.Position.Z) * d1;
		if (hasTexture) p.V1.Texture = v1.TextureCoordinates + (v2.TextureCoordinates - v1.TextureCoordinates) * d1;
		if (hasColors) p.V1.Color = v1.Color + (v2.Color - v1.Color) * d1;

		p.V2.X = v1Screen.X + (v3Screen.X - v1Screen.X) * d2;
		p.V2.Z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d2;
		if (hasTexture) p.V2.Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d2;
		if (hasColors) p.V2.Color = v1.Color + (v3.Color - v1.Color) * d2;

		DrawScanline<hasTexture, hasColors>(p);
	}

	yStart = Math::Clamp(v2Screen.Y, 0, RenderStates.FrameBuffer.Height);
	yEnd = Math::Clamp(v3Screen.Y, 0, RenderStates.FrameBuffer.Height);

	RasterizerMath::GetWorkloadParameters(RenderStates.Workload, yStart, workloadOffset, workloadIncrement);

	for (p.Y = yStart + workloadOffset; p.Y < yEnd; p.Y += workloadIncrement)
	{
		float d1 = (float)(p.Y - v2Screen.Y) / (v3Screen.Y - v2Screen.Y);
		float d2 = (float)(p.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);

		p.V1.X = v2Screen.X + (v3Screen.X - v2Screen.X) * d1;
		p.V1.Z = v2.Position.Z + (v3.Position.Z - v2.Position.Z) * d1;
		if (hasTexture) p.V1.Texture = v2.TextureCoordinates + (v3.TextureCoordinates - v2.TextureCoordinates) * d1;
		if (hasColors) p.V1.Color = v2.Color + (v3.Color - v2.Color) * d1;

		p.V2.X = v1Screen.X + (v3Screen.X - v1Screen.X) * d2;
		p.V2.Z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d2;
		if (hasTexture) p.V2.Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d2;
		if (hasColors) p.V2.Color = v1.Color + (v3.Color - v1.Color) * d2;

		DrawScanline<hasTexture, hasColors>(p);
	}

	return true;
}
template<bool hasTexture, bool hasColors>
void Rasterizer::DrawScanline(const ScanlineParameters &p) const
{
	// When using template parameters for render states, the compiler will optimize away unused code paths and branching.

	if (RenderStates.ZEnable)
	{
		if (RenderStates.ZWriteEnable)
		{
			switch (RenderStates.StencilFunc)
			{
				case StencilFunc::Always:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<true, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<true, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<true, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<true, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<true, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::Zero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<true, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<true, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<true, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<true, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<true, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::NotZero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<true, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<true, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<true, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<true, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<true, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
			}
		}
		else
		{
			switch (RenderStates.StencilFunc)
			{
				case StencilFunc::Always:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<true, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<true, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<true, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<true, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<true, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::Zero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<true, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<true, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<true, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<true, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<true, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::NotZero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<true, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<true, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<true, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<true, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<true, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
			}
		}
	}
	else
	{
		if (RenderStates.ZWriteEnable)
		{
			switch (RenderStates.StencilFunc)
			{
				case StencilFunc::Always:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<false, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<false, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<false, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<false, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<false, true, StencilFunc::Always, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::Zero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<false, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<false, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<false, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<false, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<false, true, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::NotZero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<false, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<false, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<false, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<false, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<false, true, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
			}
		}
		else
		{
			switch (RenderStates.StencilFunc)
			{
				case StencilFunc::Always:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<false, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<false, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<false, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<false, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<false, false, StencilFunc::Always, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::Zero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<false, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<false, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<false, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<false, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<false, false, StencilFunc::Zero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
				case StencilFunc::NotZero:
					switch (RenderStates.BlendMode)
					{
						case BlendMode::None: DrawScanlineT<false, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::None>(p); break;
						case BlendMode::TransparencyKey: DrawScanlineT<false, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::TransparencyKey>(p); break;
						case BlendMode::Alpha: DrawScanlineT<false, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Alpha>(p); break;
						case BlendMode::Multiply: DrawScanlineT<false, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Multiply>(p); break;
						case BlendMode::Add: DrawScanlineT<false, false, StencilFunc::NotZero, hasTexture, hasColors, BlendMode::Add>(p); break;
					}
					break;
			}
		}
	}
}
template<bool zEnable, bool zWriteEnable, StencilFunc stencilFunc, bool hasTexture, bool hasColors, BlendMode blendMode>
void Rasterizer::DrawScanlineT(ScanlineParameters p) const
{
	if (p.V1.X > p.V2.X)
	{
		// Scanline must go from left to right.
		std::swap(p.V1.X, p.V2.X);
		std::swap(p.V1.Z, p.V2.Z);
		if (hasTexture) std::swap(p.V1.Texture, p.V2.Texture);
		if (hasColors) std::swap(p.V1.Color, p.V2.Color);
	}

	if (p.V2.X < 0 || p.V1.X >= RenderStates.FrameBuffer.Width)
	{
		// Scanline is outside of the screen.
		return;
	}

	if (p.V1.X < 0)
	{
		// Clip scanline to left screen edge.
		float d = (float)p.V1.X / (p.V2.X - p.V1.X);
		p.V1.X = 0;
		p.V1.Z -= (p.V2.Z - p.V1.Z) * d;
		if (hasTexture) p.V1.Texture -= (p.V2.Texture - p.V1.Texture) * d;
		if (hasColors) p.V1.Color -= (p.V2.Color - p.V1.Color) * d;
	}

	if (p.V2.X >= RenderStates.FrameBuffer.Width)
	{
		// Clip scanline to right screen edge.
		p.V2.X = RenderStates.FrameBuffer.Width - 1;
	}

	int32 *frameBuffer = RenderStates.FrameBuffer.GetBuffer<int32>(p.V1.X + p.Y * RenderStates.FrameBuffer.Width);
	float *depthBuffer = RenderStates.DepthBuffer.GetBuffer<float>(p.V1.X + p.Y * RenderStates.FrameBuffer.Width);
	byte *stencilBuffer = RenderStates.StencilBuffer.GetBuffer<byte>(p.V1.X + p.Y * RenderStates.FrameBuffer.Width);
	int32 *textureBuffer;
	int32 textureWidthMask;
	int32 textureHeightMask;
	int32 textureWidthExponent;

	int32 alpha = blendMode == BlendMode::Alpha ? (int32)(RenderStates.Alpha * 255) : 0;
	int32 inverseAlpha = blendMode == BlendMode::Alpha ? 255 - alpha : 0;

	float z = p.V1.Z;
	Vector2f texture = p.V1.Texture;
	Vector3f color = p.V1.Color;
	Vector2f subdivTexture;
	Vector3f subdivColor;
	Vector2f subdivIncrementTexture;
	Vector3f subdivIncrementColor;

	int32 subdivCount = ((p.V2.X - p.V1.X) >> SubdivExponent) + 1;
	for (int32 i = 0; i < subdivCount; i++)
	{
		int32 subdivSize = i < subdivCount - 1 ? Subdiv1 : (p.V2.X - p.V1.X) & SubdivMask;

		float zStart = 1 / z;
		float zEnd = 1 / (z + p.DeltaZSubdiv);
		if (hasTexture) subdivTexture = texture * zStart;
		if (hasColors) subdivColor = color * zStart;
		if (hasTexture) subdivIncrementTexture = ((texture + p.DeltaTextureSubdiv) * zEnd - subdivTexture) * InvertedSubdiv1;
		if (hasColors) subdivIncrementColor = ((color + p.DeltaColorSubdiv) * zEnd - subdivColor) * InvertedSubdiv1;

		if (hasTexture)
		{
			int32 mipLevel = RasterizerMath::GetMipLevel(subdivIncrementTexture, RenderStates.Texture->MipLevels);
			textureBuffer = RenderStates.Texture->MipMaps[mipLevel];
			textureWidthMask = (RenderStates.Texture->Width >> mipLevel) - 1;
			textureHeightMask = (RenderStates.Texture->Height >> mipLevel) - 1;
			textureWidthExponent = RenderStates.Precomputed.TextureWidthExponent - mipLevel;

			if (mipLevel > 0)
			{
				float d = 1.0f / (1 << mipLevel);
				subdivTexture *= d;
				subdivIncrementTexture *= d;
			}
		}

		for (int32 j = 0; j < subdivSize; j++)
		{
			// All conditional statements are evaluated at compile time by leveraging template parameters.
			if (!zEnable || *depthBuffer <= z)
			{
				if (stencilFunc == StencilFunc::Always ||
					stencilFunc == StencilFunc::NotZero && *stencilBuffer ||
					stencilFunc == StencilFunc::Zero && !*stencilBuffer)
				{
					if (hasTexture)
					{
						byte *texel = (byte*)&textureBuffer[(int32)subdivTexture.X & textureWidthMask | ((int32)subdivTexture.Y & textureHeightMask) << textureWidthExponent];

						if (hasColors)
						{
							switch (blendMode)
							{
								case BlendMode::None:
								{
									*frameBuffer =
										texel[2] * (int32)subdivColor.X >> 8 << 16 |
										texel[1] * (int32)subdivColor.Y >> 8 << 8 |
										texel[0] * (int32)subdivColor.Z >> 8;

									if (zWriteEnable) *depthBuffer = z;
									break;
								}
								case BlendMode::TransparencyKey:
								{
									if (*(int32*)texel != 0xff00ff)
									{
										*frameBuffer =
											texel[2] * (int32)subdivColor.X >> 8 << 16 |
											texel[1] * (int32)subdivColor.Y >> 8 << 8 |
											texel[0] * (int32)subdivColor.Z >> 8;

										if (zWriteEnable) *depthBuffer = z;
									}
									break;
								}
								case BlendMode::Alpha:
								{
									int32 r = ((texel[2] * (int32)subdivColor.X >> 8) * alpha + ((byte*)frameBuffer)[2] * inverseAlpha) >> 8;
									int32 g = ((texel[1] * (int32)subdivColor.Y >> 8) * alpha + ((byte*)frameBuffer)[1] * inverseAlpha) >> 8;
									int32 b = ((texel[0] * (int32)subdivColor.Z >> 8) * alpha + ((byte*)frameBuffer)[0] * inverseAlpha) >> 8;

									*frameBuffer = r << 16 | g << 8 | b;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
								case BlendMode::Multiply:
								{
									int32 r = texel[2] * (int32)subdivColor.X * ((byte*)frameBuffer)[2] >> 16;
									int32 g = texel[1] * (int32)subdivColor.Y * ((byte*)frameBuffer)[1] >> 16;
									int32 b = texel[0] * (int32)subdivColor.Z * ((byte*)frameBuffer)[0] >> 16;

									*frameBuffer = r << 16 | g << 8 | b;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
								case BlendMode::Add:
								{
									int32 r = Math::Min((texel[2] * (int32)subdivColor.X >> 8) + ((byte*)frameBuffer)[2], 255);
									int32 g = Math::Min((texel[1] * (int32)subdivColor.Y >> 8) + ((byte*)frameBuffer)[1], 255);
									int32 b = Math::Min((texel[0] * (int32)subdivColor.Z >> 8) + ((byte*)frameBuffer)[0], 255);

									*frameBuffer = r << 16 | g << 8 | b;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
							}
						}
						else
						{
							switch (blendMode)
							{
								case BlendMode::None:
								{
									*frameBuffer = *(int32*)texel;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
								case BlendMode::TransparencyKey:
								{
									if (*(int32*)texel != 0xff00ff)
									{
										*frameBuffer = *(int32*)texel;
										if (zWriteEnable) *depthBuffer = z;
									}
									break;
								}
								case BlendMode::Alpha:
								{
									int32 r = (texel[2] * alpha + ((byte*)frameBuffer)[2] * inverseAlpha) >> 8;
									int32 g = (texel[1] * alpha + ((byte*)frameBuffer)[1] * inverseAlpha) >> 8;
									int32 b = (texel[0] * alpha + ((byte*)frameBuffer)[0] * inverseAlpha) >> 8;

									*frameBuffer = r << 16 | g << 8 | b;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
								case BlendMode::Multiply:
								{
									int32 r = texel[2] * ((byte*)frameBuffer)[2] >> 8;
									int32 g = texel[1] * ((byte*)frameBuffer)[1] >> 8;
									int32 b = texel[0] * ((byte*)frameBuffer)[0] >> 8;

									*frameBuffer = r << 16 | g << 8 | b;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
								case BlendMode::Add:
								{
									int32 r = Math::Min(texel[2] + ((byte*)frameBuffer)[2], 255);
									int32 g = Math::Min(texel[1] + ((byte*)frameBuffer)[1], 255);
									int32 b = Math::Min(texel[0] + ((byte*)frameBuffer)[0], 255);

									*frameBuffer = r << 16 | g << 8 | b;
									if (zWriteEnable) *depthBuffer = z;
									break;
								}
							}
						}
					}
					else
					{
						switch (blendMode)
						{
							case BlendMode::None:
							{
								*frameBuffer = (int32)subdivColor.X << 16 | (int32)subdivColor.Y << 8 | (int32)subdivColor.Z;
								break;
							}
							case BlendMode::Alpha:
							{
								int32 r = ((int32)subdivColor.X * alpha + ((byte*)frameBuffer)[2] * inverseAlpha) >> 8;
								int32 g = ((int32)subdivColor.Y * alpha + ((byte*)frameBuffer)[1] * inverseAlpha) >> 8;
								int32 b = ((int32)subdivColor.Z * alpha + ((byte*)frameBuffer)[0] * inverseAlpha) >> 8;

								*frameBuffer = r << 16 | g << 8 | b;
								break;
							}
							case BlendMode::Multiply:
							{
								int32 r = (int32)subdivColor.X * ((byte*)frameBuffer)[2] >> 8;
								int32 g = (int32)subdivColor.Y * ((byte*)frameBuffer)[1] >> 8;
								int32 b = (int32)subdivColor.Z * ((byte*)frameBuffer)[0] >> 8;

								*frameBuffer = r << 16 | g << 8 | b;
								break;
							}
							case BlendMode::Add:
							{
								int32 r = Math::Min((int32)subdivColor.X + ((byte*)frameBuffer)[2], 255);
								int32 g = Math::Min((int32)subdivColor.Y + ((byte*)frameBuffer)[1], 255);
								int32 b = Math::Min((int32)subdivColor.Z + ((byte*)frameBuffer)[0], 255);

								*frameBuffer = r << 16 | g << 8 | b;
								break;
							}
						}

						if (zWriteEnable) *depthBuffer = z;
					}
				}
			}

			frameBuffer++;
			if (zEnable || zWriteEnable) depthBuffer++;
			if (stencilFunc != StencilFunc::Always) stencilBuffer++;
			z += p.DeltaZ;
			if (hasTexture) subdivTexture += subdivIncrementTexture;
			if (hasColors) subdivColor += subdivIncrementColor;
		}

		if (hasTexture) texture += p.DeltaTextureSubdiv;
		if (hasColors) color += p.DeltaColorSubdiv;
	}
}