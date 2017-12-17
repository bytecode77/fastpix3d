class FASTPIX3D_API Light
{
private:
	static void Initialize();
public:
	static Color AmbientColor;

	bool Enabled;
	LightType Type;
	float Intensity;
	Color DiffuseColor;
	Vector3f Position;
	Vector3f Rotation;

	Light();
	Light(bool enabled);
	Light(bool enabled, LightType type);

	friend class Device;
};