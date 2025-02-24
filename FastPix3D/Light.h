#pragma once
#include "FastPix3D.h"
#include "Math/Color.h"
#include "Math/VectorMath.h"

enum class LightType
{
	Directional,
	Point,
	Spot
};

class PrecomputedLight
{
public:
	float SpecularIntensity;
	vfloat3 ColorF;
	vfloat3 PositionViewSpace;
	vfloat3 DirectionViewSpace;
	float DirectionViewSpaceDotIntensity;
};

class Light
{
private:
	PrecomputedLight Precomputed;
	bool _Enabled;
	LightType _Type;
	float _Intensity;
	Color _Color;
	vfloat3 _Position;
	vfloat3 _Rotation;
	std::function<void()> OnChanged;

	Light() :
		_Enabled(false),
		_Type(LightType::Directional),
		_Intensity(1),
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
		if (OnChanged) OnChanged();
	}
	property_get(LightType, Type)
	{
		return _Type;
	}
	property_set(LightType, Type)
	{
		_Type = value;
		if (OnChanged) OnChanged();
	}
	property_get(float, Intensity)
	{
		return _Intensity;
	}
	property_set(float, Intensity)
	{
		_Intensity = value;
		if (OnChanged) OnChanged();
	}
	property_get(::Color, Color)
	{
		return _Color;
	}
	property_set(const ::Color&, Color)
	{
		_Color = value;
		if (OnChanged) OnChanged();
	}
	property_get(vfloat3, Position)
	{
		return _Position;
	}
	property_set(const vfloat3&, Position)
	{
		_Position = value;
		if (OnChanged) OnChanged();
	}
	property_get(vfloat3, Rotation)
	{
		return _Rotation;
	}
	property_set(const vfloat3&, Rotation)
	{
		_Rotation = value;
		if (OnChanged) OnChanged();
	}

	friend class RenderStates;
	friend class FragmentRasterizer;
};