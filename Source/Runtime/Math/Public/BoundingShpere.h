#pragma once
#include "Vector4.h"
struct BoundingShpere {
public:
	BoundingShpere() {}
	BoundingShpere(float InRadius) : Radius(InRadius){}
	void CalcSphere(Vector4* Invertices, int VertexCount);
public:
	float Radius = 0.f;
	Vector3 Center = Vector3::Zero;
};