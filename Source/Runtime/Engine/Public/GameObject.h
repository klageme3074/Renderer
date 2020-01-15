#pragma once
#include "Transform.h"
#include "Mesh.h"
class GameObject
{
public:
	GameObject() = default;

public:
	Transform& GetTransform() { return TransformGameObject; }
	Mesh* Getmesh() { return MeshPtr; }

	void SetMesh(Mesh* InMeshPtr) { MeshPtr = InMeshPtr; };
private:
	Transform TransformGameObject;
	Mesh* MeshPtr = nullptr;
};

