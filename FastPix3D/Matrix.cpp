#include "FastPix3D.h"

Matrix Matrix::Identity()
{
	Matrix m;
	m.Values[0] = 1; m.Values[1] = 0; m.Values[2] = 0; m.Values[3] = 0;
	m.Values[4] = 0; m.Values[5] = 1; m.Values[6] = 0; m.Values[7] = 0;
	m.Values[8] = 0; m.Values[9] = 0; m.Values[10] = 1; m.Values[11] = 0;
	m.Values[12] = 0; m.Values[13] = 0; m.Values[14] = 0; m.Values[15] = 1;
	m.ResetRotation();
	return m;
}
Matrix Matrix::Scale(Vector3f size)
{
	Matrix m;
	m.Values[0] = size.X; m.Values[1] = 0; m.Values[2] = 0; m.Values[3] = 0;
	m.Values[4] = 0; m.Values[5] = size.Y; m.Values[6] = 0; m.Values[7] = 0;
	m.Values[8] = 0; m.Values[9] = 0; m.Values[10] = size.Z; m.Values[11] = 0;
	m.Values[12] = 0; m.Values[13] = 0; m.Values[14] = 0; m.Values[15] = 1;
	m.ResetRotation();
	return m;
}
Matrix Matrix::RotateX(float angle)
{
	Matrix m;
	m.Values[0] = 1; m.Values[1] = 0; m.Values[2] = 0; m.Values[3] = 0;
	m.Values[4] = 0; m.Values[5] = cosf(angle * DegToRad); m.Values[6] = sinf(angle * DegToRad); m.Values[7] = 0;
	m.Values[8] = 0; m.Values[9] = -sinf(angle * DegToRad); m.Values[10] = cosf(angle * DegToRad); m.Values[11] = 0;
	m.Values[12] = 0; m.Values[13] = 0; m.Values[14] = 0; m.Values[15] = 1;
	m.UpdateRotation();
	return m;
}
Matrix Matrix::RotateY(float angle)
{
	Matrix m;
	m.Values[0] = cosf(angle * DegToRad); m.Values[1] = 0; m.Values[2] = -sinf(angle * DegToRad); m.Values[3] = 0;
	m.Values[4] = 0; m.Values[5] = 1; m.Values[6] = 0; m.Values[7] = 0;
	m.Values[8] = sinf(angle * DegToRad); m.Values[9] = 0; m.Values[10] = cosf(angle * DegToRad); m.Values[11] = 0;
	m.Values[12] = 0; m.Values[13] = 0; m.Values[14] = 0; m.Values[15] = 1;
	m.UpdateRotation();
	return m;
}
Matrix Matrix::RotateZ(float angle)
{
	Matrix m;
	m.Values[0] = cosf(angle * DegToRad); m.Values[1] = sinf(angle * DegToRad); m.Values[2] = 0; m.Values[3] = 0;
	m.Values[4] = -sinf(angle * DegToRad); m.Values[5] = cosf(angle * DegToRad); m.Values[6] = 0; m.Values[7] = 0;
	m.Values[8] = 0; m.Values[9] = 0; m.Values[10] = 1; m.Values[11] = 0;
	m.Values[12] = 0; m.Values[13] = 0; m.Values[14] = 0; m.Values[15] = 1;
	m.UpdateRotation();
	return m;
}
Matrix Matrix::RotateXYZ(Vector3f angles)
{
	return RotateX(angles.X) * RotateY(angles.Y) * RotateZ(angles.Z);
}
Matrix Matrix::Translate(Vector3f offset)
{
	Matrix m;
	m.Values[0] = 1; m.Values[1] = 0; m.Values[2] = 0; m.Values[3] = 0;
	m.Values[4] = 0; m.Values[5] = 1; m.Values[6] = 0; m.Values[7] = 0;
	m.Values[8] = 0; m.Values[9] = 0; m.Values[10] = 1; m.Values[11] = 0;
	m.Values[12] = offset.X; m.Values[13] = offset.Y; m.Values[14] = offset.Z; m.Values[15] = 1;
	m.ResetRotation();
	return m;
}

Matrix Matrix::Multiply(Matrix matrix)
{
	Matrix c;
	c.Values[0] = Values[0] * matrix.Values[0] + Values[1] * matrix.Values[4] + Values[2] * matrix.Values[8] + Values[3] * matrix.Values[12];
	c.Values[1] = Values[0] * matrix.Values[1] + Values[1] * matrix.Values[5] + Values[2] * matrix.Values[9] + Values[3] * matrix.Values[13];
	c.Values[2] = Values[0] * matrix.Values[2] + Values[1] * matrix.Values[6] + Values[2] * matrix.Values[10] + Values[3] * matrix.Values[14];
	c.Values[3] = Values[0] * matrix.Values[3] + Values[1] * matrix.Values[7] + Values[2] * matrix.Values[11] + Values[3] * matrix.Values[15];
	c.Values[4] = Values[4] * matrix.Values[0] + Values[5] * matrix.Values[4] + Values[6] * matrix.Values[8] + Values[7] * matrix.Values[12];
	c.Values[5] = Values[4] * matrix.Values[1] + Values[5] * matrix.Values[5] + Values[6] * matrix.Values[9] + Values[7] * matrix.Values[13];
	c.Values[6] = Values[4] * matrix.Values[2] + Values[5] * matrix.Values[6] + Values[6] * matrix.Values[10] + Values[7] * matrix.Values[14];
	c.Values[7] = Values[4] * matrix.Values[3] + Values[5] * matrix.Values[7] + Values[6] * matrix.Values[11] + Values[7] * matrix.Values[15];
	c.Values[8] = Values[8] * matrix.Values[0] + Values[9] * matrix.Values[4] + Values[10] * matrix.Values[8] + Values[11] * matrix.Values[12];
	c.Values[9] = Values[8] * matrix.Values[1] + Values[9] * matrix.Values[5] + Values[10] * matrix.Values[9] + Values[11] * matrix.Values[13];
	c.Values[10] = Values[8] * matrix.Values[2] + Values[9] * matrix.Values[6] + Values[10] * matrix.Values[10] + Values[11] * matrix.Values[14];
	c.Values[11] = Values[8] * matrix.Values[3] + Values[9] * matrix.Values[7] + Values[10] * matrix.Values[11] + Values[11] * matrix.Values[15];
	c.Values[12] = Values[12] * matrix.Values[0] + Values[13] * matrix.Values[4] + Values[14] * matrix.Values[8] + Values[15] * matrix.Values[12];
	c.Values[13] = Values[12] * matrix.Values[1] + Values[13] * matrix.Values[5] + Values[14] * matrix.Values[9] + Values[15] * matrix.Values[13];
	c.Values[14] = Values[12] * matrix.Values[2] + Values[13] * matrix.Values[6] + Values[14] * matrix.Values[10] + Values[15] * matrix.Values[14];
	c.Values[15] = Values[12] * matrix.Values[3] + Values[13] * matrix.Values[7] + Values[14] * matrix.Values[11] + Values[15] * matrix.Values[15];
	c.ValuesRotation[0] = ValuesRotation[0] * matrix.ValuesRotation[0] + ValuesRotation[1] * matrix.ValuesRotation[4] + ValuesRotation[2] * matrix.ValuesRotation[8] + ValuesRotation[3] * matrix.ValuesRotation[12];
	c.ValuesRotation[1] = ValuesRotation[0] * matrix.ValuesRotation[1] + ValuesRotation[1] * matrix.ValuesRotation[5] + ValuesRotation[2] * matrix.ValuesRotation[9] + ValuesRotation[3] * matrix.ValuesRotation[13];
	c.ValuesRotation[2] = ValuesRotation[0] * matrix.ValuesRotation[2] + ValuesRotation[1] * matrix.ValuesRotation[6] + ValuesRotation[2] * matrix.ValuesRotation[10] + ValuesRotation[3] * matrix.ValuesRotation[14];
	c.ValuesRotation[3] = ValuesRotation[0] * matrix.ValuesRotation[3] + ValuesRotation[1] * matrix.ValuesRotation[7] + ValuesRotation[2] * matrix.ValuesRotation[11] + ValuesRotation[3] * matrix.ValuesRotation[15];
	c.ValuesRotation[4] = ValuesRotation[4] * matrix.ValuesRotation[0] + ValuesRotation[5] * matrix.ValuesRotation[4] + ValuesRotation[6] * matrix.ValuesRotation[8] + ValuesRotation[7] * matrix.ValuesRotation[12];
	c.ValuesRotation[5] = ValuesRotation[4] * matrix.ValuesRotation[1] + ValuesRotation[5] * matrix.ValuesRotation[5] + ValuesRotation[6] * matrix.ValuesRotation[9] + ValuesRotation[7] * matrix.ValuesRotation[13];
	c.ValuesRotation[6] = ValuesRotation[4] * matrix.ValuesRotation[2] + ValuesRotation[5] * matrix.ValuesRotation[6] + ValuesRotation[6] * matrix.ValuesRotation[10] + ValuesRotation[7] * matrix.ValuesRotation[14];
	c.ValuesRotation[7] = ValuesRotation[4] * matrix.ValuesRotation[3] + ValuesRotation[5] * matrix.ValuesRotation[7] + ValuesRotation[6] * matrix.ValuesRotation[11] + ValuesRotation[7] * matrix.ValuesRotation[15];
	c.ValuesRotation[8] = ValuesRotation[8] * matrix.ValuesRotation[0] + ValuesRotation[9] * matrix.ValuesRotation[4] + ValuesRotation[10] * matrix.ValuesRotation[8] + ValuesRotation[11] * matrix.ValuesRotation[12];
	c.ValuesRotation[9] = ValuesRotation[8] * matrix.ValuesRotation[1] + ValuesRotation[9] * matrix.ValuesRotation[5] + ValuesRotation[10] * matrix.ValuesRotation[9] + ValuesRotation[11] * matrix.ValuesRotation[13];
	c.ValuesRotation[10] = ValuesRotation[8] * matrix.ValuesRotation[2] + ValuesRotation[9] * matrix.ValuesRotation[6] + ValuesRotation[10] * matrix.ValuesRotation[10] + ValuesRotation[11] * matrix.ValuesRotation[14];
	c.ValuesRotation[11] = ValuesRotation[8] * matrix.ValuesRotation[3] + ValuesRotation[9] * matrix.ValuesRotation[7] + ValuesRotation[10] * matrix.ValuesRotation[11] + ValuesRotation[11] * matrix.ValuesRotation[15];
	c.ValuesRotation[12] = ValuesRotation[12] * matrix.ValuesRotation[0] + ValuesRotation[13] * matrix.ValuesRotation[4] + ValuesRotation[14] * matrix.ValuesRotation[8] + ValuesRotation[15] * matrix.ValuesRotation[12];
	c.ValuesRotation[13] = ValuesRotation[12] * matrix.ValuesRotation[1] + ValuesRotation[13] * matrix.ValuesRotation[5] + ValuesRotation[14] * matrix.ValuesRotation[9] + ValuesRotation[15] * matrix.ValuesRotation[13];
	c.ValuesRotation[14] = ValuesRotation[12] * matrix.ValuesRotation[2] + ValuesRotation[13] * matrix.ValuesRotation[6] + ValuesRotation[14] * matrix.ValuesRotation[10] + ValuesRotation[15] * matrix.ValuesRotation[14];
	c.ValuesRotation[15] = ValuesRotation[12] * matrix.ValuesRotation[3] + ValuesRotation[13] * matrix.ValuesRotation[7] + ValuesRotation[14] * matrix.ValuesRotation[11] + ValuesRotation[15] * matrix.ValuesRotation[15];
	return c;
}
Vector3f Matrix::Multiply(Vector3f vector)
{
	return Vector3f(
		Values[0] * vector.X + Values[4] * vector.Y + Values[8] * vector.Z + Values[12],
		Values[1] * vector.X + Values[5] * vector.Y + Values[9] * vector.Z + Values[13],
		Values[2] * vector.X + Values[6] * vector.Y + Values[10] * vector.Z + Values[14]
		);
}
Vector3f Matrix::MultiplyRotationOnly(Vector3f vector)
{
	return Vector3f(
		ValuesRotation[0] * vector.X + ValuesRotation[4] * vector.Y + ValuesRotation[8] * vector.Z + ValuesRotation[12],
		ValuesRotation[1] * vector.X + ValuesRotation[5] * vector.Y + ValuesRotation[9] * vector.Z + ValuesRotation[13],
		ValuesRotation[2] * vector.X + ValuesRotation[6] * vector.Y + ValuesRotation[10] * vector.Z + ValuesRotation[14]
		);
}

Matrix Matrix::operator *(Matrix matrix)
{
	return Multiply(matrix);
}
Vector3f Matrix::operator *(Vector3f vector)
{
	return Multiply(vector);
}

void Matrix::ResetRotation()
{
	ValuesRotation[0] = 1; ValuesRotation[1] = 0; ValuesRotation[2] = 0; ValuesRotation[3] = 0;
	ValuesRotation[4] = 0; ValuesRotation[5] = 1; ValuesRotation[6] = 0; ValuesRotation[7] = 0;
	ValuesRotation[8] = 0; ValuesRotation[9] = 0; ValuesRotation[10] = 1; ValuesRotation[11] = 0;
	ValuesRotation[12] = 0; ValuesRotation[13] = 0; ValuesRotation[14] = 0; ValuesRotation[15] = 1;
}
void Matrix::UpdateRotation()
{
	for (int32 i = 0; i < 16; i++) ValuesRotation[i] = Values[i];
}