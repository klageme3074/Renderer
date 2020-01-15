
#include "Precompiled.h"
#include "SoftRendererImpl2D.h"
#include "SoftRenderer.h"

SoftRendererImpl2D::SoftRendererImpl2D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	LoadResource();
	LoadScene();
}

SoftRendererImpl2D::~SoftRendererImpl2D()
{
}

void SoftRendererImpl2D::DrawGrid2D()
{
	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// Render Start
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenHalfSize = ScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= ScreenSize.X; x += Grid2DUnit)
	{
		RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			RSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= ScreenSize.Y; y += Grid2DUnit)
	{
		RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			RSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	RSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}


void SoftRendererImpl2D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	DrawGrid2D();

	for (const auto& g : Scene) {
		Mesh* meshToRender = g->Getmesh();
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
			Matrix4x4(),
			Matrix4x4()
		};

		RSI->SetUnitformMatrix(unitforMatrix);
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(indexBuffuer);
		RSI->DrawPrimitive(4, 6);
	}


	/*float sa, ca;
	Math::GetSinCos(sa, ca, Angle); 

	Matrix4x4 rotMat(Vector3(ca, sa, 0.f), Vector3(-sa, ca, 0.f), Vector4::UnitZ, Vector4::UnitW);

	const int vertexCount = 4;
	VertexData v[vertexCount] = {
	 VertexData(Vector4(-0.5f, -0.5f, 0.f) * 100.f, LinearColor::Black),
	 VertexData(Vector4(-0.5f, 0.5f, 0.f) * 100.f, LinearColor::Black),
	 VertexData(Vector4(0.5f, 0.5f, 0.f) * 100.f, LinearColor::Black),
	 VertexData(Vector4(0.6f, -1.2f, 0.f) * 100.f, LinearColor::Black)
	};

	const int triangleCount = 2;
	const int indexCount = triangleCount * 3;
	int i[indexCount] = {
	 0, 2, 1, 0, 3, 2
	};

	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawPrimitive(3, 3);*/
}

void SoftRendererImpl2D::UpdateImpl(float DeltaSeconds)
{
	Angle += (180.f * DeltaSeconds * InputManager.GetXAxis());
	const auto player = Scene[0].get();
	const float moveSpeed = 100.f;
	Vector3 deltaPostion = Vector3(
		InputManager.GetXAxis() * moveSpeed * DeltaSeconds, 
		InputManager.GetYAxis() * moveSpeed * DeltaSeconds, 
		0.f );
	

	Camera2D.GetTransform().AddPostion(deltaPostion);
}

void SoftRendererImpl2D::LoadResource()
{

	// FBX 데이터 로딩으로 가정.
	const int vertexCount = 4;
	const int triangleCount = 2;
	const int indexCount = triangleCount * 3;

	Vector4 v[vertexCount] = {
	 Vector4(-0.5f, -0.5f, 0.f),
	 Vector4(-0.5f, 0.5f, 0.f),
	 Vector4(0.5f, 0.5f, 0.f),
	 Vector4(0.5f, -0.5f, 0.f)
	};

	int i[indexCount] = {
	 0, 2, 1, 0, 3, 2
	};

	// 메시에 넘기기
	SharedMesh = std::make_unique<Mesh>();
	SharedMesh->SetMesh(v, vertexCount, i, indexCount);
}

void SoftRendererImpl2D::LoadScene()
{
	GameObject* player = new GameObject();
	Scene.emplace_back(player);

	player->SetMesh(SharedMesh.get());

	player->GetTransform().SetPosition(Vector3::Zero);
	player->GetTransform().SetScale(Vector3::One * 100);
	Camera2D.GetTransform().SetPosition(Vector3::Zero);
	
}
