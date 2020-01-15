
#include "Precompiled.h"
#include "Transform.h"

void Transform::SetRotation(Vector3 InRotation)
{
	Rotation = InRotation;
	CalcLocalAxis();
}

void Transform::AddYawRotation(float InRotation)
{
	Rotation.Y += InRotation;
	CalcLocalAxis();
}

void Transform::AddRollRotation(float InRotation)
{
	Rotation.X += InRotation;
	CalcLocalAxis();
}

void Transform::AddPitchRotation(float InRotation)
{
	Rotation.Z += InRotation;
	CalcLocalAxis();
}

Matrix4x4 Transform::GetModelingMatrix() const
{
	// 4¿ø¼Ò?
	Matrix4x4 TransMatrix(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(Position));
	Matrix4x4 RotationMatrix(Vector4(Right, false), Vector4(Up, false), Vector4(Forward, false), Vector4::UnitW);
	Matrix4x4 ScaleMatix(Vector4::UnitX * Scale.X, Vector4::UnitY * Scale.Y, Vector4::UnitZ * Scale.Z, Vector4::UnitW);
	return TransMatrix * RotationMatrix * ScaleMatix;
}

void Transform::CalcLocalAxis()
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(sy, cy, Rotation.Y);
	Math::GetSinCos(sp, cp, Rotation.X);
	Math::GetSinCos(sr, cr, Rotation.Z);

	Right = Vector3(cy * cr + sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
	Up = Vector3(-cy * sr + sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
	Forward = Vector3(sy * cp, -sp, cy * cp);
	
}
