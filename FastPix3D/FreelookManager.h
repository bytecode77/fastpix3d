class FASTPIX3D_API FreelookManager
{
private:
	clock_t OldTime;
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Translation;
	Vector3f RotationTranslation;
	float Speed;
public:
	FreelookManager();
	FreelookManager(Vector3f position);
	FreelookManager(Vector3f position, Vector3f rotation);
	FreelookManager(Vector3f position, Vector3f rotation, float speed);

	Matrix Update();
	void StopVelocities();

	Matrix getCameraSpace();
	Matrix getCameraSpaceRotationOnly();
	Vector3f getPosition();
	Vector3f getRotation();
	float getSpeed();

	void setPosition(Vector3f position);
	void setRotation(Vector3f rotation);
	void setSpeed(float speed);
};