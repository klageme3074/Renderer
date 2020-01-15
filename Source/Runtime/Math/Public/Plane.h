#pragma once
#include "Vector3.h"

class Plane
{
public:
	Plane(){}
	Plane(const Vector3& InNormal, float InD) : Normal(InNormal), D(InD) {}
	Plane(const Vector3& InP1, const Vector3& InP2, const Vector3& InP3) {}

	Plane Normalize();
	~Plane() {};

	Vector3 Normal = Vector3::UnitY;
	float GetD () const { return D; }

private:
	float D = 0.f;

};