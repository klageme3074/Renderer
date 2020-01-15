#pragma once

class Mesh
{
public:
	Mesh() = default;
	~Mesh() { Release();  }
public:
	//VertexBuffer, VertexCount, IndexBuffer, IndexCount
	void SetMesh(Vector4*, int, int*, int);
	void Release();

	int GetVertexCount() const { return VertexCount; }
	int GetIndexCount() const { return IndexCount; }

	Vector4* GetVertices() const { return Vertices; }
	int* GetIndices() const { return Indices; }

	BoundingShpere& GetBoundingSphere() { return Sphere; }
private:

	Vector4* Vertices = nullptr;
	int* Indices = nullptr;

	int VertexCount = 0;
	int IndexCount = 0;
	BoundingShpere Sphere;
};

