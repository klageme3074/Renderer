#pragma once

class Transform
{
public:
	Transform() = default;

public:
	void SetPosition(Vector3 InPostion) { Position = InPostion; }
	void SetRotation(Vector3 InRotation);
	void SetScale(Vector3 InScale) { Scale = InScale; }

	void AddPostion(Vector3 InPostion) { Position += InPostion; }
	void AddRotation(Vector3 InRotation) { Rotation += InRotation; }
	void AddScale(Vector3 InScale) { Scale += InScale; }

	void AddYawRotation(float InRotation);
	void AddRollRotation(float InRotation);
	void AddPitchRotation(float InRotation);


	Vector3 GetPosition() const { return Position; }
	Vector3 GetRotation() const  { return Rotation; }
	Vector3 GetScale() const  { return Scale; }
	Matrix4x4 GetModelingMatrix() const;
private:

	void CalcLocalAxis();

	// 오른속 좌표계
	// Open GL
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	Vector3 Forward = Vector3::UnitZ;
	Vector3 Right = Vector3::UnitX;
	Vector3 Up = Vector3::UnitY;

	friend class GameObject;
	friend class Camera;
};

