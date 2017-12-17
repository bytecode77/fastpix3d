struct FASTPIX3D_API Matrix
{
private:
	float Values[16], ValuesRotation[16];

	void ResetRotation();
	void UpdateRotation();
public:
	static Matrix Identity();
	static Matrix Scale(Vector3f size);
	static Matrix RotateX(float angle);
	static Matrix RotateY(float angle);
	static Matrix RotateZ(float angle);
	static Matrix RotateXYZ(Vector3f angles);
	static Matrix Translate(Vector3f offset);

	Matrix Multiply(Matrix vector);
	Vector3f Multiply(Vector3f vector);
	Vector3f MultiplyRotationOnly(Vector3f vector);

	Matrix operator *(Matrix matrix);
	Vector3f operator *(Vector3f vector);
};