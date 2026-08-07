#include "RenderStates.h"

RenderStates::RenderStates()
{
	for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
	{
		Lights[i].Parent = this;
	}

	UpdateClipNear();
	UpdateTextureSize();
	UpdateModelViewMatrix();
}
RenderStates::RenderStates(const RenderStates &renderStates)
{
	*this = renderStates;
}

void RenderStates::SetWorkload(int32 threadIndex, int32 threadCount)
{
	if (threadIndex < 0 || threadIndex >= threadCount) throw std::out_of_range("threadIndex must be in the range 0..threadCount.");
	if (threadCount != 1 && threadCount != 2 && threadCount != 4 && threadCount != 8 && threadCount != 16 && threadCount != 32) throw std::invalid_argument("threadCount must be 1, 2, 4, 8, 16 or 32.");

	Workload = (::Workload)(threadIndex | threadCount << 8);
}

void RenderStates::UpdateClipNear()
{
	Precomputed.InverseClipNear = 1 / _ClipNear;
}
void RenderStates::UpdateTextureSize()
{
	Precomputed.InverseTextureSize = vfloat2(1 / _TextureSize.X, 1 / _TextureSize.Y);
}
void RenderStates::UpdateModelViewMatrix()
{
	Precomputed.ModelViewMatrix = _ModelMatrix * _ViewMatrix;
	Precomputed.NormalMatrix = Precomputed.ModelViewMatrix.RotationPart;
}
void RenderStates::UpdateShadowLightMatrix()
{
	Precomputed.ShadowLightMatrix = Matrix4f::Translate(-Lights[ShadowLightIndex].Position) * Matrix4f::RotateY(-Lights[ShadowLightIndex].Rotation.X) * Matrix4f::RotateX(-Lights[ShadowLightIndex].Rotation.Y);
	Precomputed.ShadowLightModelMatrix = _ModelMatrix * Precomputed.ShadowLightMatrix;
}
void RenderStates::PrecomputeLights()
{
	Matrix4f viewMatrixRotationPart = ViewMatrix.RotationPart;
	Precomputed.LightsMaxIndex = -1;

	for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
	{
		Light &light = Lights[i];

		if (light.Enabled)
		{
			light.Precomputed.SpecularIntensity = light.Intensity * SpecularIntensity;
			light.Precomputed.ColorF = (vfloat3)light.Color;
			light.Precomputed.PositionViewSpace = ViewMatrix * light.Position;
			light.Precomputed.DirectionViewSpace = Matrix4f::RotateX(light.Rotation.Y) * Matrix4f::RotateY(light.Rotation.X) * viewMatrixRotationPart * vfloat3(0, 0, -1);
			light.Precomputed.DirectionViewSpaceDotIntensity = light.Precomputed.DirectionViewSpace.SquaredLength * Lights[i].Intensity;

			Precomputed.LightsMaxIndex = i;
		}
	}

	UpdateShadowLightMatrix();
}

RenderStates& RenderStates::operator=(const RenderStates& renderStates)
{
	if (this != &renderStates)
	{
		Precomputed = renderStates.Precomputed;
		_Workload = renderStates._Workload;
		_Rasterizer = renderStates._Rasterizer;
		_FrameBuffer = renderStates._FrameBuffer;
		_DepthBuffer = renderStates._DepthBuffer;
		_ShadowMap = renderStates._ShadowMap;
		_ViewMatrix = renderStates._ViewMatrix;
		_ModelMatrix = renderStates._ModelMatrix;
		_ClipNear = renderStates._ClipNear;
		_ClipFar = renderStates._ClipFar;
		_Zoom = renderStates._Zoom;
		_DepthMode = renderStates._DepthMode;
		_CullMode = renderStates._CullMode;
		_WireframeColor = renderStates._WireframeColor;
		_WireframeDepthBias = renderStates._WireframeDepthBias;
		_TextureEnable = renderStates._TextureEnable;
		_Texture = renderStates._Texture;
		_TextureFilteringEnable = renderStates._TextureFilteringEnable;
		_TextureSize = renderStates._TextureSize;
		_BlendMode = renderStates._BlendMode;
		_Alpha = renderStates._Alpha;
		_SpecularExponent = renderStates._SpecularExponent;
		_SpecularIntensity = renderStates._SpecularIntensity;
		_FogEnable = renderStates._FogEnable;
		_FogNear = renderStates._FogNear;
		_FogFar = renderStates._FogFar;
		_FogColor = renderStates._FogColor;
		_LightsEnable = renderStates._LightsEnable;
		_AmbientLight = renderStates._AmbientLight;
		_ShadowMapFunc = renderStates._ShadowMapFunc;
		_ShadowMapProjection = renderStates._ShadowMapProjection;
		_ShadowLightIndex = renderStates._ShadowLightIndex;
		_ShadowLightZoom = renderStates._ShadowLightZoom;
		_ShadowMapDepthBias = renderStates._ShadowMapDepthBias;
		_CountTotalTriangles = renderStates._CountTotalTriangles;
		_CountRenderedTriangles = renderStates._CountRenderedTriangles;

		for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
		{
			Lights[i] = renderStates.Lights[i];
			Lights[i].Parent = this;
		}
	}

	return *this;
}

void Light::LightChanged()
{
	Parent->PrecomputeLights();
}