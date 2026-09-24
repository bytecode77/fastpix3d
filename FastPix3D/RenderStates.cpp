#include "RenderStates.h"

RenderStates::RenderStates()
{
	for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
	{
		Lights[i].Parent = this;
	}

	PrecomputeInverseClipNear();
	PrecomputeProjectionScale();
	PrecomputeProjectionScaleShadowMap();
	PrecomputeInverseTextureSize();
	PrecomputeModelViewMatrix();
}
RenderStates::RenderStates(const RenderStates &other)
{
	*this = other;
}

void RenderStates::PrecomputeInverseClipNear()
{
	Precomputed.InverseClipNear = 1 / _ClipNear;
}
void RenderStates::PrecomputeProjectionScale()
{
	Precomputed.ProjectionScale = vfloat3(_Zoom, _Zoom * _FrameBuffer.Width / _FrameBuffer.Height, _ClipNear);
}
void RenderStates::PrecomputeProjectionScaleShadowMap()
{
	Precomputed.ProjectionScaleShadowMap = vfloat3(_ShadowLightZoom, _ShadowLightZoom, 1 / _ShadowMapDepthBias);
}
void RenderStates::PrecomputeInverseTextureSize()
{
	Precomputed.InverseTextureSize = vfloat2(1 / _TextureSize.X, 1 / _TextureSize.Y);
}
void RenderStates::PrecomputeModelViewMatrix()
{
	Precomputed.ModelViewMatrix = _ModelMatrix * _ViewMatrix;
	Precomputed.NormalMatrix = Precomputed.ModelViewMatrix.RotationPart;
}
void RenderStates::PrecomputeShadowLightMatrix()
{
	Precomputed.ShadowLightMatrix = Matrix4::Translate(-Lights[ShadowLightIndex].Position) * Matrix4::RotateY(-Lights[ShadowLightIndex].Rotation.X) * Matrix4::RotateX(-Lights[ShadowLightIndex].Rotation.Y);
	Precomputed.ShadowLightModelMatrix = _ModelMatrix * Precomputed.ShadowLightMatrix;
}
void RenderStates::PrecomputeLights()
{
	Matrix4 viewMatrixRotationPart = ViewMatrix.RotationPart;
	Precomputed.LightsMaxIndex = -1;

	for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
	{
		Light &light = Lights[i];

		if (light.Enabled)
		{
			light.Precomputed.ColorIntensity = (vfloat3)light.Color * light.Intensity;
			float coneAngleCos = Math::Cos(light.ConeAngle);
			light.Precomputed.ConeAngleCos = vfloat3(coneAngleCos);
			light.Precomputed.ConeAngleScale = vfloat3(1 / (1 - coneAngleCos));
			light.Precomputed.ColorSpecularIntensity = light.Precomputed.ColorIntensity * SpecularIntensity;
			light.Precomputed.PositionViewSpace = ViewMatrix * light.Position;
			light.Precomputed.DirectionViewSpace = Matrix4::RotateX(light.Rotation.Y) * Matrix4::RotateY(light.Rotation.X) * viewMatrixRotationPart * vfloat3(0, 0, -1);

			Precomputed.LightsMaxIndex = i;
		}
	}

	PrecomputeShadowLightMatrix();
}

RenderStates& RenderStates::operator =(const RenderStates& other)
{
	if (this != &other)
	{
		Precomputed = other.Precomputed;
		_Rasterizer = other._Rasterizer;
		_FrameBuffer = other._FrameBuffer;
		_DepthBuffer = other._DepthBuffer;
		_ShadowMap = other._ShadowMap;
		_ViewMatrix = other._ViewMatrix;
		_ModelMatrix = other._ModelMatrix;
		_ClipNear = other._ClipNear;
		_ClipFar = other._ClipFar;
		_Zoom = other._Zoom;
		_DepthMode = other._DepthMode;
		_CullMode = other._CullMode;
		_WireframeColor = other._WireframeColor;
		_WireframeDepthBias = other._WireframeDepthBias;
		_TextureEnable = other._TextureEnable;
		_Texture = other._Texture;
		_TextureFilteringEnable = other._TextureFilteringEnable;
		_TextureSize = other._TextureSize;
		_BlendMode = other._BlendMode;
		_Alpha = other._Alpha;
		_SpecularExponent = other._SpecularExponent;
		_SpecularIntensity = other._SpecularIntensity;
		_FogEnable = other._FogEnable;
		_FogNear = other._FogNear;
		_FogFar = other._FogFar;
		_FogColor = other._FogColor;
		_LightsEnable = other._LightsEnable;
		_AmbientLight = other._AmbientLight;
		_ShadowMapFunc = other._ShadowMapFunc;
		_ShadowMapProjection = other._ShadowMapProjection;
		_ShadowLightIndex = other._ShadowLightIndex;
		_ShadowLightZoom = other._ShadowLightZoom;
		_ShadowMapDepthBias = other._ShadowMapDepthBias;

		for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
		{
			Lights[i] = other.Lights[i];
			Lights[i].Parent = this;
		}
	}

	return *this;
}

void Light::LightChanged()
{
	Parent->PrecomputeLights();
}