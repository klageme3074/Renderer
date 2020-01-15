
#include "Precompiled.h"
#include "Mesh.h"

void Mesh::SetMesh(Vector4 * Vertices, int VertexCount, int * Indices, int IndexCount)
{
	this->Vertices = new Vector4[VertexCount];
	this->VertexCount = VertexCount;

	this->Indices = new int[IndexCount];
	this->IndexCount = IndexCount;


	for (int vi = 0; vi < VertexCount; vi++) {
		this->Vertices[vi] = Vertices[vi];
	}
	for (int in = 0; in < IndexCount; in++) {
		this->Indices[in] = Indices[in];
	}

	Sphere.CalcSphere(Vertices, VertexCount);
}

void Mesh::Release()
{
	if (nullptr != Vertices)
	{
		delete[] Vertices;
		Vertices = nullptr;
		VertexCount = 0;
	}

	if (nullptr != Indices)
	{
		delete[] Indices;
		Indices = nullptr;
		IndexCount = 0;
	}

}
