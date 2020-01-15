
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	LoadResource();
	LoadScene();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	auto player = Scene[0].get(); // 해시값이나 맵을 이용한 값을 넣자.
	float halfFov = MainCamera.GetFov() * 0.5f;
	float tanHalfFov = tanf(Math::Deg2Rad(halfFov));
	float invTanHalfFov = 1.f / tanHalfFov;
	float nearZ = MainCamera.GetNearZ();
	float farZ = MainCamera.GetFarZ();

	Vector3 topPlane = Vector3(0.f, -invTanHalfFov, -1.f).Normalize();
	Vector3 bottomPlane = Vector3(0.f, invTanHalfFov, -1.f).Normalize();
	Vector3 rightPlane = Vector3(invTanHalfFov, 0.f, -1.f).Normalize();
	Vector3 leftPlane = Vector3(-invTanHalfFov, 0.f, -1.f).Normalize();
	Vector3 nearPlane = -Vector3::UnitZ;
	Vector3 farPlane = Vector3::UnitZ;

	Plane frusteumPlanes[6] = {
		Plane(nearPlane, -nearZ), // 근평면
		Plane(farPlane, farZ), // 원평면
		Plane(leftPlane, 0), // 왼쪽평면 (+x)
		Plane(rightPlane, 0), // 오른쪽평면 (-x)
		Plane(topPlane, 0), // 상평면 (+y)
		Plane(bottomPlane, 0) // 하평면 (-y)
	};

	for (const auto& g : Scene) {
		Matrix4x4 viewMatrix = MainCamera.GetViewMatrix(*player);
		Mesh* meshToRender = g->Getmesh();
		Vector3 viewPos = viewMatrix * g->GetTransform().GetPosition();
		
		viewPos += meshToRender->GetBoundingSphere().Center;
		float Scale = g->GetTransform().GetScale().X;
		float radius = meshToRender->GetBoundingSphere().Radius * Scale;

		//프러스텀 컬링을 진행해 솎아내기
		bool isInside = true;
		for (int pi = 0; pi < 6; ++pi) {
			// 조건이 있을때
			// 만족하면 끝내기.
			float e = frusteumPlanes[pi].Normal.Dot(viewPos);
			if (e - radius < -frusteumPlanes[pi].GetD()) {
				isInside = false;
				break;
			}
		}
		// 만족하지 않으면 진행
		if (!isInside) {
			continue;
		}
		int vertexCount = meshToRender->GetVertexCount();
		int indexCount = meshToRender->GetIndexCount();

		Vector4* meshVertexBuffer = meshToRender->GetVertices();
		int* indexBuffuer = meshToRender->GetIndices();

		VertexData* v = new VertexData[vertexCount];

		for (int vi = 0; vi < vertexCount; ++vi) {
			v[vi].Position = meshVertexBuffer[vi];
		}

		Matrix4x4 unitforMatrix[3] = {
			g->GetTransform().GetModelingMatrix(),
			MainCamera.GetViewMatrix(*player), // 바라볼 대상
			MainCamera.GetProjectionMatrix(ScreenSize.X, ScreenSize.Y)
		};

		RSI->SetUnitformMatrix(unitforMatrix);
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(indexBuffuer);
		RSI->DrawPrimitive(vertexCount, indexCount);
		delete[] v;
	}


}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	Angle += (180.f * DeltaSeconds * InputManager.GetXAxis());
	const auto player = Scene[0].get();
	const float moveSpeed = 100.f;
	Vector3 deltaPostion = Vector3(
		InputManager.GetXAxis() * moveSpeed * DeltaSeconds, 
		InputManager.GetYAxis() * moveSpeed * DeltaSeconds, 
		0.f );
	

	MainCamera.GetTransform().AddPostion(deltaPostion);
}

void SoftRendererImpl3D::LoadResource()
{

	// Load Mesh Resource
	const int vertexCount = 24;
	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;

	Vector4* v = new Vector4[vertexCount]{
		// Right 
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		// Front
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		// Back
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f),
		// Left
		Vector4(-0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, -0.5f, 0.5f),
		// Top
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		// Bottom
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f)
	};

	int* i = new int[indexCount] {
		0, 2, 1, 0, 3, 2,
			4, 6, 5, 4, 7, 6,
			8, 10, 9, 8, 11, 10,
			12, 14, 13, 12, 15, 14,
			16, 18, 17, 16, 19, 18,
			20, 22, 21, 20, 23, 22
	};

	// 메시에 넘기기
	SharedMesh = std::make_unique<Mesh>();
	SharedMesh->SetMesh(v, vertexCount, i, indexCount);

	delete[] v;
	delete[] i;

	// 위치
	// 이동
	// 스케일
	// 카메라
	// 프로젝션 -> W값으로 나누기 후 크기만큼 늘리기.
}

void SoftRendererImpl3D::LoadScene()
{
	GameObject* player = new GameObject();
	player->SetMesh(SharedMesh.get());
	player->GetTransform().SetPosition(Vector3::Zero);
	player->GetTransform().SetRotation(Vector3::Zero);
	player->GetTransform().SetScale(Vector3::One * 100.f);
	Scene.emplace_back(player);

	GameObject* frustumTestObject1 = new GameObject();
	frustumTestObject1->SetMesh(SharedMesh.get());
	frustumTestObject1->GetTransform().SetPosition(Vector3(0.f, 0.f, -600.f));
	frustumTestObject1->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject2 = new GameObject();
	frustumTestObject2->SetMesh(SharedMesh.get());
	frustumTestObject2->GetTransform().SetPosition(Vector3(-400.f, 0.f, 0.f));
	frustumTestObject2->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject3 = new GameObject();
	frustumTestObject3->SetMesh(SharedMesh.get());
	frustumTestObject3->GetTransform().SetPosition(Vector3(400.f, 0.f, 0.f));
	frustumTestObject3->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject4 = new GameObject();
	frustumTestObject4->SetMesh(SharedMesh.get());
	frustumTestObject4->GetTransform().SetPosition(Vector3(0.f, 0.f, 600.f));
	frustumTestObject4->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject5 = new GameObject();
	frustumTestObject5->SetMesh(SharedMesh.get());
	frustumTestObject5->GetTransform().SetPosition(Vector3(0.f, 400.f, 0.f));
	frustumTestObject5->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject6 = new GameObject();
	frustumTestObject6->SetMesh(SharedMesh.get());
	frustumTestObject6->GetTransform().SetPosition(Vector3(0.f, -400.f, 0.f));
	frustumTestObject6->GetTransform().SetScale(Vector3::One * 100.f);

	Scene.emplace_back(frustumTestObject1);
	Scene.emplace_back(frustumTestObject2);
	Scene.emplace_back(frustumTestObject3);
	Scene.emplace_back(frustumTestObject4);
	Scene.emplace_back(frustumTestObject5);
	Scene.emplace_back(frustumTestObject6);

	MainCamera.GetTransform().SetPosition(Vector3(0.f, 0.f, -500.f));
	
}
