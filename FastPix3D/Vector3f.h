struct FASTPIX3D_API Vector3f
{
public:
	float X, Y, Z;

	Vector3f();
	Vector3f(float x, float y, float z);

	float getLength();

	Vector3f Normalize();
	Vector3f Normalize(float length);
	float DotProduct(Vector3f a);
	Vector3f CrossProduct(Vector3f a);

	Vector3f operator +(Vector3f vector);
	Vector3f operator +();
	Vector3f operator -(Vector3f vector);
	Vector3f operator -();
	Vector3f operator *(Vector3f vector);
	Vector3f operator *(float a);
	Vector3f operator /(float a);
	void operator +=(Vector3f vector);
	void operator -=(Vector3f vector);
	void operator *=(float a);
	void operator /=(float a);
	bool operator ==(Vector3f vector);
	bool operator !=(Vector3f vector);
};