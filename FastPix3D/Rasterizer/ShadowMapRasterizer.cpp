#include "ShadowMapRasterizer.h"
#include "../Math/Box2i.h"
#include "../RasterizerMath.h"

bool ShadowMapRasterizer::DrawTriangle(const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	RasterizerVertex v1 = RasterizerVertex(_v1);
	RasterizerVertex v2 = RasterizerVertex(_v2);
	RasterizerVertex v3 = RasterizerVertex(_v3);

	// Transform vertices to shadow map space.
	v1.Position = RenderStates.Precomputed.ShadowLightModelMatrix * v1.Position;
	v2.Position = RenderStates.Precomputed.ShadowLightModelMatrix * v2.Position;
	v3.Position = RenderStates.Precomputed.ShadowLightModelMatrix * v3.Position;

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

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		return DrawClippedTriangle(v1, v2, v3);
	}
	else if (vertex1Visible && vertex2Visible)
	{
		RasterizerVertex v23;
		RasterizerVertex v31;

		ClipEdges(v2, v3, v3, v1, v23, v31);
		return DrawClippedTriangle(v31, v1, v23) | DrawClippedTriangle(v1, v2, v23);
	}
	else if (vertex2Visible && vertex3Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v31;

		ClipEdges(v1, v2, v3, v1, v12, v31);
		return DrawClippedTriangle(v3, v31, v2) | DrawClippedTriangle(v12, v2, v31);
	}
	else if (vertex1Visible && vertex3Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v23;

		ClipEdges(v1, v2, v2, v3, v12, v23);
		return DrawClippedTriangle(v1, v12, v23) | DrawClippedTriangle(v3, v1, v23);
	}
	else if (vertex1Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v31;

		ClipEdges(v1, v2, v3, v1, v12, v31);
		return DrawClippedTriangle(v1, v12, v31);
	}
	else if (vertex2Visible)
	{
		RasterizerVertex v12;
		RasterizerVertex v23;

		ClipEdges(v1, v2, v2, v3, v12, v23);
		return DrawClippedTriangle(v2, v23, v12);
	}
	else if (vertex3Visible)
	{
		RasterizerVertex v23;
		RasterizerVertex v31;

		ClipEdges(v2, v3, v3, v1, v23, v31);
		return DrawClippedTriangle(v3, v31, v23);
	}
	else
	{
		return false;
	}
}
void ShadowMapRasterizer::ClipEdges(const RasterizerVertex &edge1a, const RasterizerVertex &edge1b, const RasterizerVertex &edge2a, const RasterizerVertex &edge2b, RasterizerVertex &intersection1, RasterizerVertex &intersection2) const
{
	// When a triangle intersects the near clipping plane, two intersection points need to be computed.

	float t1 = (RenderStates.ClipNear - edge1a.Position.Z) / (edge1b.Position.Z - edge1a.Position.Z);
	float t2 = (RenderStates.ClipNear - edge2a.Position.Z) / (edge2b.Position.Z - edge2a.Position.Z);

	intersection1.Position = VectorMath::MulAdd(edge1b.Position - edge1a.Position, vfloat3(t1), edge1a.Position);
	intersection2.Position = VectorMath::MulAdd(edge2b.Position - edge2a.Position, vfloat3(t2), edge2a.Position);

	if (RenderStates.TextureEnable && RenderStates.Texture && RenderStates.Texture->HasTransparencyKey)
	{
		intersection1.TextureCoordinates = edge1a.TextureCoordinates + (edge1b.TextureCoordinates - edge1a.TextureCoordinates) * t1;
		intersection2.TextureCoordinates = edge2a.TextureCoordinates + (edge2b.TextureCoordinates - edge2a.TextureCoordinates) * t2;
	}
}
bool ShadowMapRasterizer::DrawClippedTriangle(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3) const
{
	// Project vertices to screen space.
	v1.Position = RasterizerMath::Project(v1.Position, RenderStates.Precomputed.ProjectionScaleShadowMap);
	v2.Position = RasterizerMath::Project(v2.Position, RenderStates.Precomputed.ProjectionScaleShadowMap);
	v3.Position = RasterizerMath::Project(v3.Position, RenderStates.Precomputed.ProjectionScaleShadowMap);

	if (RasterizerMath::IsTriangleOutsideClipVolume(v1.Position, v2.Position, v3.Position))
	{
		// Triangle is completely outside of the view frustum.
		return false;
	}

	if (RenderStates.TextureEnable && RenderStates.Texture && RenderStates.Texture->HasTransparencyKey)
	{
		// Textures with a transparency key (e.g. a fence or tree leaves) need to be considered for shadow mapping.
		DrawClippedTriangle<true>(v1, v2, v3);
	}
	else
	{
		DrawClippedTriangle<false>(v1, v2, v3);
	}

	return true;
}
template<bool hasTexture>
void ShadowMapRasterizer::DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const
{
	// Project vertices screen space.
	vfloat2 v1ScreenF = RasterizerMath::ToScreenSpaceF(v1.Position, RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Width);
	vfloat2 v2ScreenF = RasterizerMath::ToScreenSpaceF(v2.Position, RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Width);
	vfloat2 v3ScreenF = RasterizerMath::ToScreenSpaceF(v3.Position, RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Width);

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
			Math::Min(halfspace_upper_fix(Math::Max(v1Screen.X, v2Screen.X, v3Screen.X), fixExponent), fix(RenderStates.ShadowMap.Width, fixExponent) - 1),
			Math::Min(halfspace_upper_fix(Math::Max(v1Screen.Y, v2Screen.Y, v3Screen.Y), fixExponent), fix(RenderStates.ShadowMap.Width, fixExponent) - 1)
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

	if constexpr (hasTexture)
	{
		// Scale texture coordinates by texture size.
		v1.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v2.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v3.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;

		// Texture coordinates must be positive.
		RasterizerMath::MakeTextureCoordinatesPositive(v1.TextureCoordinates, v2.TextureCoordinates, v3.TextureCoordinates);

		// Store UV multiplied by texture resolution divided by Z.
		v1.TextureCoordinates.X *= v1.Position.Z * RenderStates.Texture->Width;
		v1.TextureCoordinates.Y *= v1.Position.Z * RenderStates.Texture->Height;
		v2.TextureCoordinates.X *= v2.Position.Z * RenderStates.Texture->Width;
		v2.TextureCoordinates.Y *= v2.Position.Z * RenderStates.Texture->Height;
		v3.TextureCoordinates.X *= v3.Position.Z * RenderStates.Texture->Width;
		v3.TextureCoordinates.Y *= v3.Position.Z * RenderStates.Texture->Height;
	}

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

	vfloat4 v1Attributes = vfloat4(v1.Position.Z, hasTexture ? v1.TextureCoordinates.X : 0, hasTexture ? v1.TextureCoordinates.Y : 0);
	vfloat4 v2Attributes = vfloat4(v2.Position.Z, hasTexture ? v2.TextureCoordinates.X : 0, hasTexture ? v2.TextureCoordinates.Y : 0);
	vfloat4 v3Attributes = vfloat4(v3.Position.Z, hasTexture ? v3.TextureCoordinates.X : 0, hasTexture ? v3.TextureCoordinates.Y : 0);
	vfloat4 v1AttributesRelative = v1Attributes - v3Attributes;
	vfloat4 v2AttributesRelative = v2Attributes - v3Attributes;

	// Vertex attributes at the top-left corner of the bounding box.
	vfloat4 blockAttributesRow = VectorMath::MulAdd(v1AttributesRelative, vfloat4(barycentricStartL1), VectorMath::MulAdd(v2AttributesRelative, vfloat4(barycentricStartL2), v3Attributes));

	// Attribute deltas per pixel.
	vfloat4 attributesDeltaX = VectorMath::MulAdd(v1AttributesRelative, vfloat4(barycentricL1.X), v2AttributesRelative * barycentricL2.X);
	vfloat4 attributesDeltaY = VectorMath::MulAdd(v1AttributesRelative, vfloat4(barycentricL1.Y), v2AttributesRelative * barycentricL2.Y);

	// Deltas per block for interpolation of vertex attributes.
	vfloat4 blockAttributesDeltaX = halfspace_mulf(attributesDeltaX);
	vfloat4 blockAttributesDeltaY = attributesDeltaY * (float)partitionIncrement;

	float *shadowMap = RenderStates.ShadowMap.GetBuffer<float>(boundingBox.Min.X + unfix(yStart, fixExponent) * RenderStates.ShadowMap.Width);

	int32 stride = RenderStates.ShadowMap.Width; // Advance to next row in 8x8 block.
	int32 strideBlock = -halfspace_mul(RenderStates.ShadowMap.Width); // Rewind to top-left corner of 8x8 block.
	int32 strideRow = RenderStates.ShadowMap.Width * partitionIncrement - (boundingBox.Max.X + 1 - boundingBox.Min.X); // Advance to next row of blocks.

	Color *textureBuffer = RenderStates.Texture->Mip0;
	int32 textureWidthMask = RenderStates.Texture->Width - 1;
	int32 textureHeightMask = RenderStates.Texture->Height - 1;
	int32 textureWidthExponent = RenderStates.Texture->WidthExponent;

	for (int32 blockY = unfix(yStart, fixExponent); blockY < boundingBox.Max.Y; blockY += partitionIncrement)
	{
		bool isDrawing = false;
		vfix4 cornerEdgeMax = cornerEdgeMaxRow;
		vfix8 blockEdgeA = blockEdgeARow;
		vfix8 blockEdgeB = blockEdgeBRow;
		vfix8 blockEdgeC = blockEdgeCRow;
		vfloat4 blockAttributes = blockAttributesRow;

		for (int32 blockX = boundingBox.Min.X; blockX < boundingBox.Max.X; blockX += halfspace_mul(1))
		{
			// Each edge must be positive at at least one corner of the block. 
			if (VectorMath::CmpGtMask(cornerEdgeMax, vfix4()) == 0xfff)
			{
				isDrawing = true;

				vfloat4 attributesRow;
				vfloat4 attributesPixelDeltaX;
				vfloat4 attributesPixelDeltaY;

				if constexpr (hasTexture)
				{
					float d = 1 / blockAttributes[ATTRIBUTE_Z];
					attributesRow = blockAttributes * d;

					// Do perspective correction only once per block.
					attributesPixelDeltaX = VectorMath::NegMulAdd(attributesRow, vfloat4(attributesDeltaX[ATTRIBUTE_Z]), attributesDeltaX) * d;
					attributesPixelDeltaY = VectorMath::NegMulAdd(attributesRow, vfloat4(attributesDeltaY[ATTRIBUTE_Z]), attributesDeltaY) * d;
				}
				else
				{
					// Without a texture, there is only depth and nothing else to perspectively correct.
					attributesRow = blockAttributes;
					attributesPixelDeltaX = attributesDeltaX;
					attributesPixelDeltaY = attributesDeltaY;
				}

				vfloat8 attributeZ = VectorMath::MulAdd(vfloat8(attributesDeltaX[ATTRIBUTE_Z]), Delta1To8MultiplierF, vfloat8(blockAttributes[ATTRIBUTE_Z]));
				vfloat8 attributeU;
				vfloat8 attributeV;
				if constexpr (hasTexture) attributeU = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_U]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_U]));
				if constexpr (hasTexture) attributeV = VectorMath::MulAdd(vfloat8(attributesPixelDeltaX[ATTRIBUTE_V]), Delta1To8MultiplierF, vfloat8(attributesRow[ATTRIBUTE_V]));

				vfix8 edgeA = blockEdgeA;
				vfix8 edgeB = blockEdgeB;
				vfix8 edgeC = blockEdgeC;

				for (int32 i = 0; i < 8; i++)
				{
					DrawPixelVector<hasTexture>(
						(vfloat8*)shadowMap,
						VectorMath::CmpGt(edgeA | edgeB | edgeC, vfix8()),
						attributeZ,
						attributeU,
						attributeV,
						textureBuffer,
						textureWidthMask,
						textureHeightMask,
						textureWidthExponent);

					attributeZ += attributesDeltaY[ATTRIBUTE_Z];
					if constexpr (hasTexture) attributeU += attributesPixelDeltaY[ATTRIBUTE_U];
					if constexpr (hasTexture) attributeV += attributesPixelDeltaY[ATTRIBUTE_V];

					edgeA += edgeADeltaY;
					edgeB += edgeBDeltaY;
					edgeC += edgeCDeltaY;
					shadowMap += stride;
				}

				shadowMap += strideBlock;
			}
			else if (isDrawing)
			{
				// End of the current line of blocks.
				shadowMap += boundingBox.Max.X + 1 - blockX;
				break;
			}

			cornerEdgeMax += blockEdgeDeltaX;
			blockEdgeA += blockEdgeADeltaX;
			blockEdgeB += blockEdgeBDeltaX;
			blockEdgeC += blockEdgeCDeltaX;
			blockAttributes += blockAttributesDeltaX;
			shadowMap += halfspace_mul(1);
		}

		cornerEdgeMaxRow += blockEdgeDeltaY;
		blockEdgeARow += blockEdgeADeltaY;
		blockEdgeBRow += blockEdgeBDeltaY;
		blockEdgeCRow += blockEdgeCDeltaY;
		blockAttributesRow += blockAttributesDeltaY;
		shadowMap += strideRow;
	}
}
template<bool hasTexture>
__forceinline void ShadowMapRasterizer::DrawPixelVector(
	vfloat8 *shadowMap,
	vuint8 writeMask,
	const vfloat8 &attributeZ,
	const vfloat8 &attributeU,
	const vfloat8 &attributeV,
	const Color *textureBuffer,
	int32 textureWidthMask,
	int32 textureHeightMask,
	int32 textureWidthExponent) const
{
	if constexpr (hasTexture)
	{
		writeMask &= VectorMath::CmpGt(attributeZ, vfloat8(shadowMap));

		vuint8 textureColor = vuint8::Read((uint32*)textureBuffer, (vint8)attributeU & textureWidthMask | ((vint8)attributeV & textureHeightMask) << textureWidthExponent, writeMask);

		writeMask = VectorMath::CmpGt((vint8)textureColor, vint8());
		vfloat8::Write(shadowMap, attributeZ, writeMask);
	}
	else
	{
		vfloat8::Write(shadowMap, VectorMath::Max(vfloat8(shadowMap), attributeZ), writeMask);
	}
}