
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "WindowsTimer.h"

void SoftRenderer::Initialize()
{
	// Init System Timer
	MilliSecFrequency = WindowsTimer::Init();
	if (MilliSecFrequency == 0)
		return;

	FrameCount = 0;
	ElapsedTime = 0;

	// Init RSI
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::Update()
{
	if (RSI == nullptr) return;


	////static const float PI;
	////static const float TwoPI;
	////static const float HalfPI;
	////static const float InvPI;
	////float sinWave = ElapsedTime , sinf , 

	//float sinWave = (sinf(ElapsedTime * Math::TwoPI) + 1.f) * 0.5f;
	//// Colors to use
	//LinearColor vectorSpaceColor(LinearColor(0.125f, 0.5f, 1.0f, 0.5f));

	//// Basis Vector forms Vector Space
	////Vector3 xBasisVector(1.f, 0.f, 0.f);
	////Vector3 yBasisVector(0.f, 1.f, 0.f);
	////Vector3 zBasisVector(0.f, 1.f, 0.f);

	//float theta = Math::Deg2Rad(30.0f);
	//float sinTheta, cosTheta;
	//Math::SinCos(&sinTheta, &cosTheta, theta);
	//// To Do : Define Transform Matrix 2x2 
	////Matrix2x2 m(Vector2(cosTheta, sinTheta), Vector2(-sinTheta, cosTheta));



	////Matrix3x3 scaleM(
	////	Vector3::UnitX * scalar,
	////	Vector3::UnitY * scalar,
	////	Vector3::UnitZ);

	////Matrix3x3 rotateM(
	////	Vector3(Vector2(cosTheta, sinTheta), false), 
	////	Vector3(Vector2(-sinTheta, cosTheta), false),
	////	Vector3::UnitZ);

	////Matrix3x3 translateM(
	////	Vector3::UnitX, 
	////	Vector3::UnitY, 
	////	Vector3(currentPos, true));

	////Matrix3x3 m = translateM * rotateM * scaleM;


	//VertexData v[4];
	//v[0].Position = (m * Vector3(Vector2::Zero, true)).ToVector2();
	//v[0].Color = vectorSpaceColor;

	//v[1].Position = (m * Vector3(Vector2::UnitX, true)).ToVector2();
	//v[1].Color = vectorSpaceColor;

	//v[2].Position = (m * Vector3(Vector2::UnitY, true)).ToVector2();
	//v[2].Color = vectorSpaceColor;

	//v[3].Position = (m * Vector3(Vector2::UnitX + Vector2::UnitY, true)).ToVector2();
	//v[3].Color = vectorSpaceColor;

	//int i[6];
	//i[0] = 0;
	//i[1] = 3;
	//i[2] = 1;
	//i[3] = 0;
	//i[4] = 2;
	//i[5] = 3;

	//// Draw Call
	//RSI->SetVertexBuffer(v);
	//RSI->SetIndexBuffer(i);
	//RSI->DrawPrimitive(4, 6);

	//// Draw Basis Vector Arrow
	//RSI->DrawArrowLine(v[0].Position, v[1].Position, LinearColor::Red, 4);
	//RSI->DrawArrowLine(v[0].Position, v[2].Position, LinearColor::Green, 4);

	// Draw Translated Vector Arrow ( y = 1 )
	//Vector2 translatedStart = m * Vector2(0, 1) * scalar;
	//Vector2 translatedEnd = m * Vector2(1, 1) * scalar;
	//RSI->DrawArrowLine(translatedStart, translatedEnd, LinearColor::Red, 4);

	// Draw Line2
	//RSI->DrawLine2(ScreenPoint(10, 10), ScreenPoint(120, 50));
	//RSI->DrawLine2(ScreenPoint(10, 10), ScreenPoint(50, 120));
	//RSI->DrawLine2(ScreenPoint(-10, 10), ScreenPoint(-120, 50));
	//RSI->DrawLine2(ScreenPoint(-10, 10), ScreenPoint(-50, 120));
	//RSI->DrawLine2(ScreenPoint(-10, -10), ScreenPoint(-120, -50));
	//RSI->DrawLine2(ScreenPoint(-10, -10), ScreenPoint(-50, -120));
	//RSI->DrawLine2(ScreenPoint(10, -10), ScreenPoint(120, -50));
	//RSI->DrawLine2(ScreenPoint(10, -10), ScreenPoint(50, -120));

	static float moveSpeed = 300;
	static float turnSpeed = 180;
	static float currentZ = 0.f;
	static float currentYaw = 0.f;
	static float currentRoll = 0.f;
	static float currentPitch = 0.f;
	currentYaw += InputManager.GetXAxis() * turnSpeed * FrameSec;
	currentZ += InputManager.GetYAxis() * moveSpeed * FrameSec;
	Vector3 currentPos(0.f, 0.f, currentZ);

	float cy, sy, cr, sr, cp, sp;
	Math::GetSinCos(sy, cy, currentYaw);
	Math::GetSinCos(cr, sr, currentRoll);
	Math::GetSinCos(cp, sp, currentPitch);

	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(currentPos));
	Matrix4x4 rMat(Vector4(cy*cr + sr*sy*sp, cp*sr, sy*cr + cy*-sp*sr, 0.f), Vector4(cy*-sr+sy*sp*cr, cr*cp, -sr*sy+cr*cy*-sp, 0.f), Vector4(-sy*cp, sp, cy*cp, 0.f), Vector4::UnitW);
	Matrix4x4 sMat(Vector4::UnitX * 100, Vector4::UnitY * 100, Vector4::UnitZ * 100, Vector4::UnitW);
	Matrix4x4 mMat = tMat * rMat * sMat;

	Vector3 cameraPosition(Vector3(0.f, 500.f, -500.f));
	Vector3 viewZUnit = (cameraPosition - currentPos).Normalize();
	Vector3 viewXUnit = Vector3::UnitY.Cross(viewZUnit).Normalize();
	if (viewXUnit.IsZero())
	{
		viewXUnit = Vector3::UnitX;
	}
	Vector3 viewYUnit = viewZUnit.Cross(viewXUnit).Normalize();

	Vector4 zAxis(viewZUnit, false);
	Vector4 xAxis(viewXUnit, false);
	Vector4 yAxis(viewYUnit, false);
	Matrix4x4 virMat(xAxis, yAxis, zAxis, Vector4::UnitW);
	Matrix4x4 vitMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-cameraPosition, true));
	Matrix4x4 vMat = virMat * vitMat;

	float nearZ = 5.5f;
	float farZ = 100.f;

	float divisor = 1.f / (nearZ - farZ);
	float fovInDegree = 60.f;
	float fovRadian = Math::Deg2Rad(fovInDegree * 0.5f);
	float d = 1.f / tanf(fovRadian);
	float reciprocalA = (float)DisplaySetting::Inst().GetSize().Y / (float)DisplaySetting::Inst().GetSize().X;

	Vector4 pxAxis(d * reciprocalA, 0.f, 0.f, 0.f);
	Vector4 pyAxis(0.f, d, 0.f, 0.f);
	Vector4 pzAxis(0.f, 0.f, farZ * divisor, -1.f);
	Vector4 pwAxis(0.f, 0.f, nearZ * farZ * divisor, 0.f);

	Matrix4x4 pMat(pxAxis, pyAxis, pzAxis, pwAxis);
	Matrix4x4 finalMat = pMat * vMat * mMat;

	const int vertexCount = 24;
	VertexData v[vertexCount] = {
		// Front 
		VertexData(Vector4(0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(0.5f, -0.5f, -0.5f)),

		// Left
		VertexData(Vector4(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(0.5f, -0.5f, 0.5f)),

		// Right
		VertexData(Vector4(0.5f, -0.5f, -0.5f)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, -0.5f, -0.5f)),

		// Back
		VertexData(Vector4(-0.5f, -0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, -0.5f, 0.5f)),

		// Top
		VertexData(Vector4(0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f)),

		// Bottom
		VertexData(Vector4(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(0.5f, -0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, -0.5f, -0.5f)),
	};

	for (int vi = 0; vi < vertexCount; vi++)
	{
		v[vi].Position = finalMat * v[vi].Position;
	}

	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;
	int i[indexCount] = {
		0, 2, 1, 0, 3, 2,
		4, 6, 5, 4, 7, 6,
		8, 10, 9, 8, 11, 10,
		12, 14, 13, 12, 15, 14,
		16, 18, 17, 16, 19, 18,
		20, 22, 21, 20, 23, 22
	};
	for (int t = 0; t < triangleCount; t++)
	{
		Vector4 tp[3] = { v[i[t * 3]].Position , v[i[t * 3 + 1]].Position, v[i[t * 3 + 2]].Position };

		for (int ti = 0; ti < 3; ti++)
		{
			float reciprocalZ = 1.f / tp[ti].W;
			tp[ti].X = tp[ti].X * reciprocalZ;
			tp[ti].Y = tp[ti].Y * reciprocalZ;
			tp[ti].Z = tp[ti].Z * reciprocalZ;
		}

		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();
		Vector3 faceNormal = edge1.Cross(edge2).Normalize();
		if (faceNormal.Dot(-Vector3::UnitZ) > 0.f)
		{
			continue;
		}

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti].X *= (DisplaySetting::Inst().GetSize().X * 0.5f);
			tp[ti].Y *= (DisplaySetting::Inst().GetSize().Y * 0.5f);
		}

		RSI->DrawLine2(ScreenPoint(tp[0]), ScreenPoint(tp[1]), LinearColor::Blue);
		RSI->DrawLine2(ScreenPoint(tp[0]), ScreenPoint(tp[2]), LinearColor::Blue);
		RSI->DrawLine2(ScreenPoint(tp[1]), ScreenPoint(tp[2]), LinearColor::Blue);
	}

}

void SoftRenderer::PreUpdate()
{
	StartFrameSec = WindowsTimer::CheckMilliSeconds();

	if (RSI == nullptr) return;

	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.5f));

	// Render Start
	static int gridSize = 10;
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenSize = DisplaySetting::Inst().GetSize();

	int startX = -Math::RoundToInt(screenSize.X * 0.5f);
	int endX = -startX;

	int startY = -Math::RoundToInt(screenSize.Y * 0.5f);
	int endY = -startY;

	ScreenPoint xAxisStart(startX, 0);
	ScreenPoint xAxisEnd(endX, 0);
	ScreenPoint yAxisStart(0, startY);
	ScreenPoint yAxisEnd(0, endY);

	for (int x = 0; x <= endX; x += gridSize)
	{
		RSI->DrawVerticalLine(x, gridColor);
		if (x > 0)
		{
			RSI->DrawVerticalLine(-x, gridColor);
		}
	}

	for (int y = 0; y <= endY; y += gridSize)
	{
		RSI->DrawHorizontalLine(y, gridColor);
		if (y > 0)
		{
			RSI->DrawHorizontalLine(-y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawHorizontalLine(0, LinearColor::Red);
	RSI->DrawVerticalLine(0, LinearColor::Green);
}

void SoftRenderer::PostUpdate()
{
	if (RSI != nullptr)
	{
		// Render Finish
		RSI->EndFrame();
	}

	// Check Timer and Increase Frame Stat
	FrameCount++;
	FrameMilliSec = double(WindowsTimer::CheckMilliSeconds() - StartFrameSec) / MilliSecFrequency;
	FrameSec = (float)(FrameMilliSec / 1000);
	ElapsedTime += FrameSec;

	FrameFPS = FrameSec == 0 ? 0 : 1 / FrameSec;
}


