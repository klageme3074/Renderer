
#pragma once

#include <memory>
#include "WindowsGDI.h"
#include "RenderingSoftwareInterface.h"

class WindowsGDI;
class WindowsRSI : public WindowsGDI, public RenderingSoftwareInterface
{
public:
	WindowsRSI() = default;
	~WindowsRSI();

public:
	virtual bool Init(const ScreenPoint& InScreenSize) override;
	virtual void Shutdown() override;
	virtual bool IsInitialized() const { return IsGDIInitialized; }
	virtual void SetBlendingMode(BlendingModes InNewBlendingMode) override;

	virtual void Clear(const LinearColor& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor) override;

	virtual void DrawFullVerticalLine(int InX, const LinearColor& InColor) override;
	virtual void DrawFullHorizontalLine(int InY, const LinearColor& InColor) override;


	virtual void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor) override;
	virtual void SetUnitformMatrix(Matrix4x4* InMatrixData) override;
	virtual void SetVertexBuffer(VertexData* InVertexData) override;
	virtual void SetIndexBuffer(const int* InIndexData) override;
	virtual void DrawPrimitive(UINT InVertexSize, UINT InIndexSize) override;
private:
	void SetPixel(const ScreenPoint& InPos, const LinearColor& InColor);

	BlendingModes BlendingMode = BlendingModes::Opaque;

	VertexData* VertexBuffer = nullptr;
	const int* IndexBuffer = nullptr;

	Matrix4x4 ModelingMatrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;
};
