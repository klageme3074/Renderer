#pragma once
#include "Vector3.h"
struct BoundingBox {
public:
	BoundingBox() {}
	BoundingBox(Vector3 InCenter, Vector3 InExtent): Center(InCenter), Extent(InExtent) {}

public:
	Vector3 Center;
	Vector3 Extent;
};

