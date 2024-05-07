#pragma once
#include "FastPix3D.h"
#include "RenderTarget.h"
#include "Matrix4f.h"
#include "Vector3f.h"
#include "Light.h"
#include "Texture.h"
#include "INotify.h"

enum class Workload
{
	Full,
	Half1,
	Half2,
	Quarter1,
	Quarter2,
	Quarter3,
	Quarter4
};

enum class RenderPass
{
	Fragments,
	ShadowMap,
	ShadowMapStencil
};

enum class CullMode
{
	None,
	Back,
	Front
};

enum class StencilFunc
{
	Always,
	Zero,
	NotZero
};

enum class BlendMode
{
	None,
	TransparencyKey,
	Alpha,
	Multiply,
	Add
};

class PrecomputedLightValues
{
public:
	Vector3f PositionCameraSpace;
	Vector3f CameraSpaceVector;
	float CameraSpaceVectorDot;
	float CameraSpaceVectorDotIntensity;
};

class PrecomputedRenderStates
{
public:
	int32 ShadowMapWidthExponent;
	int32 ShadowMapHeightExponent;
	Matrix4f CameraSpaceRotationPart;
	float InverseClipNear;
	int32 TextureWidthExponent;
	int32 TextureHeightExponent;
	Vector2f InverseTextureSize;
	PrecomputedLightValues Lights[8];
};

class RenderStates : public INotify
{
private:
	PrecomputedRenderStates Precomputed;

	Workload _Workload;
	RenderPass _RenderPass;

	RenderTarget _FrameBuffer;
	RenderTarget _DepthBuffer;
	RenderTarget _StencilBuffer;
	RenderTarget _ShadowMap;

	Matrix4f _CameraSpace;
	float _ClipNear;
	float _Zoom;
	CullMode _CullMode;

	bool _ZEnable;
	bool _ZWriteEnable;
	::StencilFunc _StencilFunc;

	const Texture *_Texture;
	Vector2f _TextureSize;
	BlendMode _BlendMode;
	float _Alpha;

	bool _FogEnable;
	float _FogNear;
	float _FogFar;
	Color _FogColor;

	bool _LightsEnable;
	Color _AmbientLight;

	Matrix4f _ShadowLightMatrix;
	float _ShadowLightZoom;
	float _ShadowMapDepthBias;

public:
	property_getset(::Workload, Workload)
	{
		return _Workload;
	}
	property_set(::Workload, Workload)
	{
		_Workload = value;
	}
	property_getset(::RenderPass, RenderPass)
	{
		return _RenderPass;
	}
	property_set(::RenderPass, RenderPass)
	{
		_RenderPass = value;
	}

	property_getset(RenderTarget, FrameBuffer)
	{
		return _FrameBuffer;
	}
	property_set(const RenderTarget&, FrameBuffer)
	{
		_FrameBuffer = value;
	}
	property_getset(RenderTarget, DepthBuffer)
	{
		return _DepthBuffer;
	}
	property_set(const RenderTarget&, DepthBuffer)
	{
		_DepthBuffer = value;
	}
	property_getset(RenderTarget, StencilBuffer)
	{
		return _StencilBuffer;
	}
	property_set(const RenderTarget&, StencilBuffer)
	{
		_StencilBuffer = value;
	}
	property_getset(RenderTarget, ShadowMap)
	{
		return _ShadowMap;
	}
	property_set(const RenderTarget&, ShadowMap)
	{
		_ShadowMap = value;
		Precomputed.ShadowMapWidthExponent = Math::GetExponent(_ShadowMap.Width);
		Precomputed.ShadowMapHeightExponent = Math::GetExponent(_ShadowMap.Height);
	}

	property_getset(Matrix4f, CameraSpace)
	{
		return _CameraSpace;
	}
	property_set(const Matrix4f&, CameraSpace)
	{
		_CameraSpace = value;
		Precomputed.CameraSpaceRotationPart = _CameraSpace.RotationPart;
	}
	property_getset(float, ClipNear)
	{
		return _ClipNear;
	}
	property_set(float, ClipNear)
	{
		_ClipNear = value;
		Precomputed.InverseClipNear = 1 / _ClipNear;
	}
	property_getset(float, Zoom)
	{
		return _Zoom;
	}
	property_set(float, Zoom)
	{
		_Zoom = value;
	}
	property_getset(::CullMode, CullMode)
	{
		return _CullMode;
	}
	property_set(::CullMode, CullMode)
	{
		_CullMode = value;
	}

	property_getset(bool, ZEnable)
	{
		return _ZEnable;
	}
	property_set(bool, ZEnable)
	{
		_ZEnable = value;
	}
	property_getset(bool, ZWriteEnable)
	{
		return _ZWriteEnable;
	}
	property_set(bool, ZWriteEnable)
	{
		_ZWriteEnable = value;
	}
	property_getset(::StencilFunc, StencilFunc)
	{
		return _StencilFunc;
	}
	property_set(::StencilFunc, StencilFunc)
	{
		_StencilFunc = value;
	}

	property_getset(const ::Texture*, Texture)
	{
		return _Texture;
	}
	property_set(const ::Texture*, Texture)
	{
		_Texture = value;
		Precomputed.TextureWidthExponent = Texture ? Math::GetExponent(Texture->Width) : 0;
		Precomputed.TextureHeightExponent = Texture ? Math::GetExponent(Texture->Height) : 0;
	}
	property_getset(Vector2f, TextureSize)
	{
		return _TextureSize;
	}
	property_set(const Vector2f&, TextureSize)
	{
		_TextureSize = value;
		Precomputed.InverseTextureSize = Vector2f(1 / _TextureSize.X, 1 / _TextureSize.Y);
	}
	property_getset(::BlendMode, BlendMode)
	{
		return _BlendMode;
	}
	property_set(::BlendMode, BlendMode)
	{
		_BlendMode = value;
	}
	property_getset(float, Alpha)
	{
		return _Alpha;
	}
	property_set(float, Alpha)
	{
		_Alpha = value;
	}

	property_getset(bool, FogEnable)
	{
		return _FogEnable;
	}
	property_set(bool, FogEnable)
	{
		_FogEnable = value;
	}
	property_getset(float, FogNear)
	{
		return _FogNear;
	}
	property_set(float, FogNear)
	{
		_FogNear = value;
	}
	property_getset(float, FogFar)
	{
		return _FogFar;
	}
	property_set(float, FogFar)
	{
		_FogFar = value;
	}
	property_getset(::Color, FogColor)
	{
		return _FogColor;
	}
	property_set(const ::Color&, FogColor)
	{
		_FogColor = value;
	}

	property_getset(bool, LightsEnable)
	{
		return _LightsEnable;
	}
	property_set(bool, LightsEnable)
	{
		_LightsEnable = value;
	}
	property_getset(::Color, AmbientLight)
	{
		return _AmbientLight;
	}
	property_set(const ::Color&, AmbientLight)
	{
		_AmbientLight = value;
	}
	Light Lights[8];

	property_getset(Matrix4f, ShadowLightMatrix)
	{
		return _ShadowLightMatrix;
	}
	property_set(const Matrix4f&, ShadowLightMatrix)
	{
		_ShadowLightMatrix = value;
	}
	property_getset(float, ShadowLightZoom)
	{
		return _ShadowLightZoom;
	}
	property_set(float, ShadowLightZoom)
	{
		_ShadowLightZoom = value;
	}
	property_getset(float, ShadowMapDepthBias)
	{
		return _ShadowMapDepthBias;
	}
	property_set(float, ShadowMapDepthBias)
	{
		_ShadowMapDepthBias = value;
	}

	RenderStates()
	{
		Workload = Workload::Full;
		RenderPass = RenderPass::Fragments;
		CameraSpace = Matrix4f::Identity();
		ClipNear = 1;
		Zoom = 1;
		CullMode = CullMode::Back;
		ZEnable = true;
		ZWriteEnable = true;
		StencilFunc = StencilFunc::Always;
		Texture = nullptr;
		TextureSize = Vector2f(1);
		BlendMode = BlendMode::None;
		Alpha = 1;
		FogEnable = false;
		FogNear = 0;
		FogFar = 1000;
		LightsEnable = false;
		AmbientLight = Color(127, 127, 127);
		ShadowLightMatrix = Matrix4f::Identity();
		ShadowLightZoom = 1;
		ShadowMapDepthBias = 0;

		for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
		{
			Lights[i].NotifyParent = this;
		}
	}

	void SetShadowLightMatrixFromLight(int32 index)
	{
		ShadowLightMatrix = Matrix4f::Translate(-Lights[index].Position) * Matrix4f::RotateY(-Lights[index].Rotation.X) * Matrix4f::RotateX(-Lights[index].Rotation.Y);
	}

private:
	void Notify() override
	{
		for (int32 i = 0; i < sizeof(Lights) / sizeof(Light); i++)
		{
			if (Lights[i].Enabled)
			{
				Precomputed.Lights[i].PositionCameraSpace = CameraSpace * Lights[i].Position;
				Precomputed.Lights[i].CameraSpaceVector =
					Matrix4f::RotateY(Lights[i].Rotation.X) *
					Matrix4f::RotateX(Lights[i].Rotation.Y) *
					Precomputed.CameraSpaceRotationPart *
					Vector3f(0, 0, -1);
				Precomputed.Lights[i].CameraSpaceVectorDot = Precomputed.Lights[i].CameraSpaceVector.DotProduct(Precomputed.Lights[i].CameraSpaceVector);
				Precomputed.Lights[i].CameraSpaceVectorDotIntensity = Precomputed.Lights[i].CameraSpaceVectorDot * Lights[i].Intensity;
			}
		}
	}

	friend class Rasterizer;
	friend class ShadowMapRasterizer;
	friend class ShadowMapStencilRasterizer;
};