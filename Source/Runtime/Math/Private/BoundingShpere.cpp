#include "Precompiled.h"
#include "BoundingShpere.h"

void BoundingShpere::CalcSphere(Vector4 * Invertices, int VertexCount)
{
	// 가장 긴 거리.
	for (int i = 0; i < VertexCount; i++) {
		Center += Invertices[i].ToVector3();
	}

	Center /= VertexCount;

	float maxSize = 0.f;
	for (int vi = 0; vi < VertexCount; ++vi) {
		float squaredSize = (Center - Invertices[vi].ToVector3()).SizeSquared();
		if (squaredSize > maxSize) {
			maxSize = squaredSize;
		}
	}
	Radius = sqrtf(maxSize);

}
