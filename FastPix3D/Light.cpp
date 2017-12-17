#include "FastPix3D.h"

Color Light::AmbientColor;

void Light::Initialize()
{
	AmbientColor = Color(255, 255, 255);
}

Light::Light()
{
	Enabled = true;
	Type = LightType::Directional;
	Intensity = 1;
	DiffuseColor = Color(255, 255, 255);
	Position = Vector3f();
	Rotation = Vector3f();
}
Light::Light(bool enabled)
{
	Enabled = enabled;
	Type = LightType::Directional;
	Intensity = 1;
	DiffuseColor = Color(255, 255, 255);
	Position = Vector3f();
	Rotation = Vector3f();
}
Light::Light(bool enabled, LightType type)
{
	Enabled = enabled;
	Type = type;
	Intensity = 1;
	DiffuseColor = Color(255, 255, 255);
	Position = Vector3f();
	Rotation = Vector3f();
}