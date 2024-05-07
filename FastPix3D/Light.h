#pragma once
#include "FastPix3D.h"
#include "Vector3f.h"
#include "Color.h"
#include "INotify.h"

enum class LightType
{
	Directional,
	Point,
	Spot
};

class Light
{
private:
	bool _Enabled;
	LightType _Type;
	float _Intensity;
	Color _Color;
	Vector3f _Position;
	Vector3f _Rotation;
	INotify *NotifyParent = nullptr;

	Light() :
		_Enabled(false),
		_Type(LightType::Directional),
		_Intensity(1),
		_Color(::Color(255, 255, 255))
	{
	}

public:
	property_getset(bool, Enabled)
	{
		return _Enabled;
	}
	property_set(bool, Enabled)
	{
		_Enabled = value;
		NotifyParent->Notify();
	}
	property_getset(LightType, Type)
	{
		return _Type;
	}
	property_set(LightType, Type)
	{
		_Type = value;
		NotifyParent->Notify();
	}
	property_getset(float, Intensity)
	{
		return _Intensity;
	}
	property_set(float, Intensity)
	{
		_Intensity = value;
		NotifyParent->Notify();
	}
	property_getset(::Color, Color)
	{
		return _Color;
	}
	property_set(const ::Color&, Color)
	{
		_Color = value;
		NotifyParent->Notify();
	}
	property_getset(Vector3f, Position)
	{
		return _Position;
	}
	property_set(const Vector3f&, Position)
	{
		_Position = value;
		NotifyParent->Notify();
	}
	property_getset(Vector3f, Rotation)
	{
		return _Rotation;
	}
	property_set(const Vector3f&, Rotation)
	{
		_Rotation = value;
		NotifyParent->Notify();
	}

	friend class RenderStates;
};