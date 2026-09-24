#pragma once
#include "FastPix3D.h"
#include "Math/Color.h"
#include "Math/Matrix4.h"
#include "Math/VectorMath.h"
#include "RenderTarget.h"
#include "Mesh/Texture.h"

enum class Rasterizer
{
	Fragments,
	Wireframe,
	ShadowMap
};

enum class CullMode
{
	None,
	Back,
	Front
};

enum class DepthMode
{
	None,
	Read,
	ReadWrite
};

enum class BlendMode
{
	None,
	TransparencyKey,
	Alpha,
	Add,
	Multiply
};

enum class ShadowMapFunc
{
	None,
	Point,
	Pcf
};

enum class ShadowMapProjection
{
	Perspective,
	Cubemap
};

enum class LightType
{
	Directional,
	Point,
	Spot
};

class RenderStates;

class FASTPIX3D_API Light
{
private:
	struct Precomputed
	{
		vfloat3 ColorIntensity;
		vfloat3 ConeAngleCos;
		vfloat3 ConeAngleScale;
		vfloat3 ColorSpecularIntensity;
		vfloat3 PositionViewSpace;
		vfloat3 DirectionViewSpace;
	};

	RenderStates *Parent;
	Light::Precomputed Precomputed;
	bool _Enabled;
	LightType _Type;
	float _Intensity;
	float _ConeAngle;
	Color _Color;
	vfloat3 _Position;
	vfloat3 _Rotation;

	Light() :
		Parent(nullptr),
		_Enabled(false),
		_Type(LightType::Directional),
		_Intensity(1),
		_ConeAngle(45),
		_Color(::Color(255, 255, 255))
	{
	}

public:
	property_get(bool, Enabled)
	{
		return _Enabled;
	}
	property_set(bool, Enabled)
	{
		_Enabled = value;
		LightChanged();
	}
	property_get(LightType, Type)
	{
		return _Type;
	}
	property_set(LightType, Type)
	{
		_Type = value;
		LightChanged();
	}
	property_get(float, Intensity)
	{
		return _Intensity;
	}
	property_set(float, Intensity)
	{
		_Intensity = value;
		LightChanged();
	}
	property_get(float, ConeAngle)
	{
		return _ConeAngle;
	}
	property_set(float, ConeAngle)
	{
		_ConeAngle = value;
		LightChanged();
	}
	property_get(::Color, Color)
	{
		return _Color;
	}
	property_set(const ::Color&, Color)
	{
		_Color = value;
		LightChanged();
	}
	property_get(vfloat3, Position)
	{
		return _Position;
	}
	property_set(const vfloat3&, Position)
	{
		_Position = value;
		LightChanged();
	}
	property_get(vfloat3, Rotation)
	{
		return _Rotation;
	}
	property_set(const vfloat3&, Rotation)
	{
		_Rotation = value;
		LightChanged();
	}

private:
	void LightChanged();

	friend class RenderStates;
	friend class FragmentRasterizer;
};

class FASTPIX3D_API RenderStates
{
private:
	struct Precomputed
	{
		vfloat3 ProjectionScale;
		vfloat3 ProjectionScaleShadowMap;
		Matrix4 ModelViewMatrix;
		Matrix4 NormalMatrix;
		float InverseClipNear;
		vfloat2 InverseTextureSize;
		int32 LightsMaxIndex = -1;
		Matrix4 ShadowLightMatrix;
		Matrix4 ShadowLightModelMatrix;
	};

	RenderStates::Precomputed Precomputed;

	Rasterizer _Rasterizer = Rasterizer::Fragments;

	RenderTarget _FrameBuffer;
	RenderTarget _DepthBuffer;
	RenderTarget _ShadowMap;

	Matrix4 _ViewMatrix = Matrix4::Identity();
	Matrix4 _ModelMatrix = Matrix4::Identity();
	float _ClipNear = 1;
	float _ClipFar = 1000;
	float _Zoom = 1;
	DepthMode _DepthMode = DepthMode::ReadWrite;
	CullMode _CullMode = CullMode::Back;
	Color _WireframeColor = Color(255, 255, 255);
	float _WireframeDepthBias = 1;

	bool _TextureEnable = true;
	const Texture *_Texture = nullptr;
	bool _TextureFilteringEnable = false;
	vfloat2 _TextureSize = vfloat2(1);
	BlendMode _BlendMode = BlendMode::None;
	float _Alpha = 1;
	float _SpecularExponent = 0;
	float _SpecularIntensity = 0;

	bool _FogEnable = false;
	float _FogNear = 0;
	float _FogFar = 1000;
	Color _FogColor;

	bool _LightsEnable = false;
	Color _AmbientLight = Color(127, 127, 127);

	::ShadowMapFunc _ShadowMapFunc = ShadowMapFunc::None;
	::ShadowMapProjection _ShadowMapProjection = ShadowMapProjection::Perspective;
	int32 _ShadowLightIndex = 0;
	float _ShadowLightZoom = 1;
	float _ShadowMapDepthBias = 1;

public:
	property_get(::Rasterizer, Rasterizer)
	{
		return _Rasterizer;
	}
	property_set(::Rasterizer, Rasterizer)
	{
		_Rasterizer = value;
	}

	property_get(RenderTarget, FrameBuffer)
	{
		return _FrameBuffer;
	}
	property_set(const RenderTarget&, FrameBuffer)
	{
		_FrameBuffer = value;
		PrecomputeProjectionScale();
	}
	property_get(RenderTarget, DepthBuffer)
	{
		return _DepthBuffer;
	}
	property_set(const RenderTarget&, DepthBuffer)
	{
		_DepthBuffer = value;
	}
	property_get(RenderTarget, ShadowMap)
	{
		return _ShadowMap;
	}
	property_set(const RenderTarget&, ShadowMap)
	{
		_ShadowMap = value;
	}

	property_get(Matrix4, ViewMatrix)
	{
		return _ViewMatrix;
	}
	property_set(const Matrix4&, ViewMatrix)
	{
		_ViewMatrix = value;
		PrecomputeModelViewMatrix();
		PrecomputeLights();
	}
	property_get(Matrix4, ModelMatrix)
	{
		return _ModelMatrix;
	}
	property_set(const Matrix4&, ModelMatrix)
	{
		_ModelMatrix = value;
		PrecomputeModelViewMatrix();
		PrecomputeShadowLightMatrix();
	}
	property_get(float, ClipNear)
	{
		return _ClipNear;
	}
	property_set(float, ClipNear)
	{
		_ClipNear = value;
		PrecomputeInverseClipNear();
		PrecomputeProjectionScale();
	}
	property_get(float, ClipFar)
	{
		return _ClipFar;
	}
	property_set(float, ClipFar)
	{
		_ClipFar = value;
	}
	property_get(float, Zoom)
	{
		return _Zoom;
	}
	property_set(float, Zoom)
	{
		_Zoom = value;
		PrecomputeProjectionScale();
	}
	property_get(::DepthMode, DepthMode)
	{
		return _DepthMode;
	}
	property_set(::DepthMode, DepthMode)
	{
		_DepthMode = value;
	}
	property_get(::CullMode, CullMode)
	{
		return _CullMode;
	}
	property_set(::CullMode, CullMode)
	{
		_CullMode = value;
	}
	property_get(Color, WireframeColor)
	{
		return _WireframeColor;
	}
	property_set(Color, WireframeColor)
	{
		_WireframeColor = value;
	}
	property_get(float, WireframeDepthBias)
	{
		return _WireframeDepthBias;
	}
	property_set(float, WireframeDepthBias)
	{
		_WireframeDepthBias = value;
	}

	property_get(bool, TextureEnable)
	{
		return _TextureEnable;
	}
	property_set(bool, TextureEnable)
	{
		_TextureEnable = value;
	}
	property_get(const ::Texture*, Texture)
	{
		return _Texture;
	}
	property_set(const ::Texture*, Texture)
	{
		_Texture = value;
	}
	property_get(bool, TextureFilteringEnable)
	{
		return _TextureFilteringEnable;
	}
	property_set(bool, TextureFilteringEnable)
	{
		_TextureFilteringEnable = value;
	}
	property_get(vfloat2, TextureSize)
	{
		return _TextureSize;
	}
	property_set(const vfloat2&, TextureSize)
	{
		_TextureSize = value;
		PrecomputeInverseTextureSize();
	}
	property_get(::BlendMode, BlendMode)
	{
		return _BlendMode;
	}
	property_set(::BlendMode, BlendMode)
	{
		_BlendMode = value;
	}
	property_get(float, Alpha)
	{
		return _Alpha;
	}
	property_set(float, Alpha)
	{
		_Alpha = value;
	}
	property_get(float, SpecularExponent)
	{
		return _SpecularExponent;
	}
	property_set(float, SpecularExponent)
	{
		_SpecularExponent = value;
	}
	property_get(float, SpecularIntensity)
	{
		return _SpecularIntensity;
	}
	property_set(float, SpecularIntensity)
	{
		_SpecularIntensity = value;
		PrecomputeLights();
	}

	property_get(bool, FogEnable)
	{
		return _FogEnable;
	}
	property_set(bool, FogEnable)
	{
		_FogEnable = value;
	}
	property_get(float, FogNear)
	{
		return _FogNear;
	}
	property_set(float, FogNear)
	{
		_FogNear = value;
	}
	property_get(float, FogFar)
	{
		return _FogFar;
	}
	property_set(float, FogFar)
	{
		_FogFar = value;
	}
	property_get(::Color, FogColor)
	{
		return _FogColor;
	}
	property_set(const ::Color&, FogColor)
	{
		_FogColor = value;
	}

	property_get(bool, LightsEnable)
	{
		return _LightsEnable;
	}
	property_set(bool, LightsEnable)
	{
		_LightsEnable = value;
	}
	property_get(::Color, AmbientLight)
	{
		return _AmbientLight;
	}
	property_set(const ::Color&, AmbientLight)
	{
		_AmbientLight = value;
	}
	Light Lights[8];

	property_get(::ShadowMapFunc, ShadowMapFunc)
	{
		return _ShadowMapFunc;
	}
	property_set(::ShadowMapFunc, ShadowMapFunc)
	{
		_ShadowMapFunc = value;
	}
	property_get(::ShadowMapProjection, ShadowMapProjection)
	{
		return _ShadowMapProjection;
	}
	property_set(::ShadowMapProjection, ShadowMapProjection)
	{
		_ShadowMapProjection = value;
	}
	property_get(int32, ShadowLightIndex)
	{
		return _ShadowLightIndex;
	}
	property_set(int32, ShadowLightIndex)
	{
		_ShadowLightIndex = value;
		PrecomputeShadowLightMatrix();
	}
	property_get(float, ShadowLightZoom)
	{
		return _ShadowLightZoom;
	}
	property_set(float, ShadowLightZoom)
	{
		_ShadowLightZoom = value;
		PrecomputeProjectionScaleShadowMap();
	}
	property_get(float, ShadowMapDepthBias)
	{
		return _ShadowMapDepthBias;
	}
	property_set(float, ShadowMapDepthBias)
	{
		_ShadowMapDepthBias = value;
		PrecomputeProjectionScaleShadowMap();
	}

	RenderStates();
	RenderStates(const RenderStates &other);

private:
	void PrecomputeInverseClipNear();
	void PrecomputeProjectionScale();
	void PrecomputeProjectionScaleShadowMap();
	void PrecomputeInverseTextureSize();
	void PrecomputeModelViewMatrix();
	void PrecomputeShadowLightMatrix();
	void PrecomputeLights();

public:
	RenderStates& operator =(const RenderStates& other);

	friend class Light;
	friend class FragmentRasterizer;
	friend class WireframeRasterizer;
	friend class ShadowMapRasterizer;
};