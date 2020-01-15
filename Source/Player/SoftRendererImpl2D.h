#pragma once

#include "RenderingSoftwareInterface.h"
#include "InputManager.h"
#include "EngineHeaders.h"
#include <memory>
class SoftRendererImpl2D
{
public:
	SoftRendererImpl2D(class SoftRenderer* InOwner);
	~SoftRendererImpl2D();

public:
	void RenderFrameImpl();
	void UpdateImpl(float DeltaSeconds);

	void LoadResource();
	void LoadScene();

private:
	FORCEINLINE void DrawGrid2D();

private:
	RenderingSoftwareInterface* RSI = nullptr;

	// 2D Grid Options
	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;
	float Angle = 0.5f;
	// Input Manager
	InputManager InputManager;

	// SmartPointer Mesh Class
	std::unique_ptr<Mesh> SharedMesh;
	std::vector<std::unique_ptr<GameObject> > Scene;

	Camera Camera2D;
};
