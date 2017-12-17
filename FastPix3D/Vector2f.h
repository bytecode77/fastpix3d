struct FASTPIX3D_API Vector2f
{
public:
	float X, Y;

	Vector2f();
	Vector2f(float x, float y);

	float getLength();
	Vector2f Normalize();
	Vector2f Normalize(float length);
	float DotProduct(Vector2f a);

	Vector2f operator +(Vector2f a);
	Vector2f operator +();
	Vector2f operator -(Vector2f a);
	Vector2f operator -();
	Vector2f operator *(Vector2f a);
	Vector2f operator *(float a);
	Vector2f operator /(float a);
	void operator +=(Vector2f a);
	void operator -=(Vector2f a);
	void operator *=(float a);
	void operator /=(float a);
	bool operator ==(Vector2f a);
	bool operator !=(Vector2f a);
};