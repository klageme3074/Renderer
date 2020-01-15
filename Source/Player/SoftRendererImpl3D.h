#pragma once

#include "RenderingSoftwareInterface.h"
#include "InputManager.h"
#include "EngineHeaders.h"
#include <memory>
class SoftRendererImpl3D
{
public:
	SoftRendererImpl3D(class SoftRenderer* InOwner);
	~SoftRendererImpl3D();

public:
	void RenderFrameImpl();
	void UpdateImpl(float DeltaSeconds);

	void LoadResource();
	void LoadScene();

private:
	// Gizmo
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

	Camera MainCamera;
};
