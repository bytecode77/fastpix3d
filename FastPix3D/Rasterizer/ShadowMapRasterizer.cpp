#include "ShadowMapRasterizer.h"
#include "../Math/Box2i.h"
#include "../RasterizerMath.h"

void ShadowMapRasterizer::DrawTriangle(const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	ShadowMapRasterizerVertex v1 = ShadowMapRasterizerVertex(_v1);
	ShadowMapRasterizerVertex v2 = ShadowMapRasterizerVertex(_v2);
	ShadowMapRasterizerVertex v3 = ShadowMapRasterizerVertex(_v3);

	// Transform vertices to shadow map space.
	v1.Position = RenderStates.Precomputed.ShadowLightModelMatrix * v1.Position;
	v2.Position = RenderStates.Precomputed.ShadowLightModelMatrix * v2.Position;
	v3.Position = RenderStates.Precomputed.ShadowLightModelMatrix * v3.Position;

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

	bool rendered = false;

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		// All vertices are in front of the near clipping plane.
		rendered = DrawClippedTriangle(v1, v2, v3);
	}
	else
	{
		// 1 or 2 vertices are in front of the near clipping plane.
		ShadowMapRasterizerVertex v12;
		ShadowMapRasterizerVertex v23;
		ShadowMapRasterizerVertex v31;

		v12.Position = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.Position, v2.Position);
		v23.Position = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.Position, v3.Position);
		v31.Position = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.Position, v1.Position);

		if (RenderStates.TextureEnable && RenderStates.Texture && RenderStates.Texture->HasTransparencyKey)
		{
			v12.TextureCoordinates = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.TextureCoordinates, v2.TextureCoordinates);
			v23.TextureCoordinates = Math::Interpolate(RenderStates.ClipNear, v2.Position.Z, v3.Position.Z, v2.TextureCoordinates, v3.TextureCoordinates);
			v31.TextureCoordinates = Math::Interpolate(RenderStates.ClipNear, v3.Position.Z, v1.Position.Z, v3.TextureCoordinates, v1.TextureCoordinates);
		}

		if (vertex1Visible && vertex2Visible)
		{
			rendered = DrawClippedTriangle(v31, v1, v23);
			rendered |= DrawClippedTriangle(v1, v2, v23);
		}
		else if (vertex2Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v2);
			rendered |= DrawClippedTriangle(v12, v2, v31);
		}
		else if (vertex1Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v23);
			rendered |= DrawClippedTriangle(v3, v1, v23);
		}
		else if (vertex1Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v31);
		}
		else if (vertex2Visible)
		{
			rendered = DrawClippedTriangle(v2, v23, v12);
		}
		else if (vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v23);
		}
	}

	if (rendered && RasterizerMath::GetWorkloadThreadIndex(RenderStates.Workload) == 0)
	{
		Statistics.RenderedTriangleCount++;
	}
}
bool ShadowMapRasterizer::DrawClippedTriangle(const ShadowMapRasterizerVertex &v1, const ShadowMapRasterizerVertex &v2, const ShadowMapRasterizerVertex &v3) const
{
	if (RenderStates.TextureEnable && RenderStates.Texture && RenderStates.Texture->HasTransparencyKey)
	{
		// Textures with a transparency key (e.g. a fence or tree leaves) need to be considered for shadow mapping.
		return DrawClippedTriangle<true>(v1, v2, v3);
	}
	else
	{
		return DrawClippedTriangle<false>(v1, v2, v3);
	}
}
template<bool hasTexture>
bool ShadowMapRasterizer::DrawClippedTriangle(ShadowMapRasterizerVertex v1, ShadowMapRasterizerVertex v2, ShadowMapRasterizerVertex v3) const
{
	// Transform vertices to clip space.
	v1.Position = RasterizerMath::ToClipSpace(v1.Position, RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Width, RenderStates.ShadowLightZoom, RenderStates.ClipNear);
	v2.Position = RasterizerMath::ToClipSpace(v2.Position, RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Width, RenderStates.ShadowLightZoom, RenderStates.ClipNear);
	v3.Position = RasterizerMath::ToClipSpace(v3.Position, RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Width, RenderStates.ShadowLightZoom, RenderStates.ClipNear);

	if (RasterizerMath::IsTriangleOutsideClipVolume(v1.Position, v2.Position, v3.Position))
	{
		// Triangle is completely outside of the view frustum.
		return false;
	}

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

	vfloat4 v1Attributes = vfloat4(v1.Position.Z, hasTexture ? v1.TextureCoordinates.X : 0, hasTexture ? v1.TextureCoordinates.Y : 0);
	vfloat4 v2Attributes = vfloat4(v2.Position.Z, hasTexture ? v2.TextureCoordinates.X : 0, hasTexture ? v2.TextureCoordinates.Y : 0);
	vfloat4 v3Attributes = vfloat4(v3.Position.Z, hasTexture ? v3.TextureCoordinates.X : 0, hasTexture ? v3.TextureCoordinates.Y : 0);

	// Vertex attributes at top-left corner of the bounding box & deltas per pixel for interpolation.
	vfloat4 blockAttributesRow = v1Attributes * barycentricStartL1 + v2Attributes * barycentricStartL2 + v3Attributes * barycentricStartL3;
	vfloat4 attributesDeltaX = v1Attributes * barycentricL1.X + v2Attributes * barycentricL2.X + v3Attributes * barycentricL3.X;
	vfloat4 attributesDeltaY = v1Attributes * barycentricL1.Y + v2Attributes * barycentricL2.Y + v3Attributes * barycentricL3.Y;

	// Deltas per block for interpolation of vertex attributes.
	vfloat4 blockAttributesDeltaX = halfspace_mulf(attributesDeltaX);
	vfloat4 blockAttributesDeltaY = attributesDeltaY * (float)workloadIncrement;

	float *shadowMap = RenderStates.ShadowMap.GetBuffer<float>(boundingBox.Min.X + unfix(yStart, fixExponent) * RenderStates.ShadowMap.Width);

	int32 stride = RenderStates.ShadowMap.Width; // Advance to next row in 8x8 block.
	int32 strideBlock = -halfspace_mul(RenderStates.ShadowMap.Width); // Rewind to top-left corner of 8x8 block.
	int32 strideRow = RenderStates.ShadowMap.Width * workloadIncrement - (boundingBox.Max.X + 1 - boundingBox.Min.X); // Advance to next row of blocks.

	for (int32 blockY = unfix(yStart, fixExponent); blockY < boundingBox.Max.Y; blockY += workloadIncrement)
	{
		bool isDrawing = false;
		vfix4 cornerEdgeA = cornerEdgeARow;
		vfix4 cornerEdgeB = cornerEdgeBRow;
		vfix4 cornerEdgeC = cornerEdgeCRow;
		vfix8 blockEdgeA = blockEdgeARow;
		vfix8 blockEdgeB = blockEdgeBRow;
		vfix8 blockEdgeC = blockEdgeCRow;
		vfloat4 blockAttributes = blockAttributesRow;

		for (int32 blockX = boundingBox.Min.X; blockX < boundingBox.Max.X; blockX += halfspace_mul(1))
		{
			// At least one corner of the 8x8 block is inside the triangle.
			if (VectorMath::CmpGtMask(cornerEdgeA, vfix4()) != 0 &&
				VectorMath::CmpGtMask(cornerEdgeB, vfix4()) != 0 &&
				VectorMath::CmpGtMask(cornerEdgeC, vfix4()) != 0)
			{
				isDrawing = true;

				vfloat4 attributesRow;
				vfloat4 attributesPixelDeltaX;
				vfloat4 attributesPixelDeltaY;

				if constexpr (hasTexture)
				{
					attributesRow = blockAttributes / blockAttributes[ATTRIBUTE_Z];
					attributesRow[ATTRIBUTE_Z] = blockAttributes[ATTRIBUTE_Z];

					// Do perspective correction only once per block.
					attributesPixelDeltaX = (blockAttributes + attributesDeltaX) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaX[ATTRIBUTE_Z]) - attributesRow;
					attributesPixelDeltaY = (blockAttributes + attributesDeltaY) / (blockAttributes[ATTRIBUTE_Z] + attributesDeltaY[ATTRIBUTE_Z]) - attributesRow;
					attributesPixelDeltaX[ATTRIBUTE_Z] = attributesDeltaX[ATTRIBUTE_Z];
					attributesPixelDeltaY[ATTRIBUTE_Z] = attributesDeltaY[ATTRIBUTE_Z];
				}
				else
				{
					// Without a texture, there is only depth and nothing else to perspectively correct.
					attributesRow = blockAttributes;
					attributesPixelDeltaX = attributesDeltaX;
					attributesPixelDeltaY = attributesDeltaY;
				}

				Color *textureBuffer;
				int32 textureWidthMask;
				int32 textureHeightMask;
				int32 textureWidthExponent;

				if constexpr (hasTexture)
				{
					textureBuffer = RenderStates.Texture->Mip0;
					textureWidthMask = RenderStates.Texture->Width - 1;
					textureHeightMask = RenderStates.Texture->Height - 1;
					textureWidthExponent = Math::GetExponent(RenderStates.Texture->Width);
				}

				vfloat8 attributeZ = vfloat8(attributesRow[ATTRIBUTE_Z]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_Z]) * Delta1To8MultiplierF;
				vfloat8 attributeU;
				vfloat8 attributeV;

				if constexpr (hasTexture)
				{
					attributeU = vfloat8(attributesRow[ATTRIBUTE_U]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_U]) * Delta1To8MultiplierF;
					attributeV = vfloat8(attributesRow[ATTRIBUTE_V]) + vfloat8(attributesPixelDeltaX[ATTRIBUTE_V]) * Delta1To8MultiplierF;
				}

				vfix8 edgeA = blockEdgeA;
				vfix8 edgeB = blockEdgeB;
				vfix8 edgeC = blockEdgeC;

				for (int32 i = 0; i < 8; i++)
				{
					DrawPixelRow<hasTexture>(
						(vfloat8*)shadowMap,
						VectorMath::CmpGt(edgeA, vfix8()) & VectorMath::CmpGt(edgeB, vfix8()) & VectorMath::CmpGt(edgeC, vfix8()),
						attributeZ,
						attributeU,
						attributeV,
						hasTexture ? textureBuffer : nullptr,
						hasTexture ? textureWidthMask : 0,
						hasTexture ? textureHeightMask : 0,
						hasTexture ? textureWidthExponent : 0);

					attributeZ += attributesPixelDeltaY[ATTRIBUTE_Z];
					if constexpr (hasTexture) attributeU += attributesPixelDeltaY[ATTRIBUTE_U];
					if constexpr (hasTexture) attributeV += attributesPixelDeltaY[ATTRIBUTE_V];

					edgeA += edgeADeltaY;
					edgeB += edgeBDeltaY;
					edgeC += edgeCDeltaY;
					shadowMap += stride;
				}

				attributesRow += attributesDeltaY;
				shadowMap += strideBlock;
			}
			else if (isDrawing)
			{
				// End of the current line of blocks.
				shadowMap += boundingBox.Max.X + 1 - blockX;
				break;
			}

			cornerEdgeA += cornerEdgeADeltaX;
			cornerEdgeB += cornerEdgeBDeltaX;
			cornerEdgeC += cornerEdgeCDeltaX;
			blockEdgeA += blockEdgeADeltaX;
			blockEdgeB += blockEdgeBDeltaX;
			blockEdgeC += blockEdgeCDeltaX;
			blockAttributes += blockAttributesDeltaX;
			shadowMap += halfspace_mul(1);
		}

		cornerEdgeARow += cornerEdgeADeltaY;
		cornerEdgeBRow += cornerEdgeBDeltaY;
		cornerEdgeCRow += cornerEdgeCDeltaY;
		blockEdgeARow += blockEdgeADeltaY;
		blockEdgeBRow += blockEdgeBDeltaY;
		blockEdgeCRow += blockEdgeCDeltaY;
		blockAttributesRow += blockAttributesDeltaY;
		shadowMap += strideRow;
	}

	return true;
}
template<bool hasTexture>
__forceinline void ShadowMapRasterizer::DrawPixelRow(
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
		// Mask out pixels where the texel is transparent.
		vuint8 textureColor = vuint8::Read((uint32*)textureBuffer, (vint8)attributeU & textureWidthMask | ((vint8)attributeV & textureHeightMask) << textureWidthExponent, writeMask);
		writeMask &= VectorMath::CmpGt((vint8)textureColor, vint8());
	}

	vfloat8::Write(shadowMap, VectorMath::Max(vfloat8(shadowMap), attributeZ), writeMask);
}