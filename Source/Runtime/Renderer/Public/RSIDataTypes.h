#pragma once

enum class BlendingModes
{
	Opaque,
	AlphaBlending
};

struct VertexData {
public:
	VertexData() { }
	explicit VertexData(Vector4 InPosition) : Position(InPosition) { }
	explicit VertexData(Vector4 InPosition, LinearColor InColor) : Position(InPosition), Color(InColor) { }
	explicit VertexData(Vector4 InPosition, LinearColor InColor, Vector2 InUV) : Position(InPosition), Color(InColor), UV(InUV) { }

	Vector4 Position;
	LinearColor Color = LinearColor::Error;
	Vector3 Normal;
	Vector4 Tangent;
	Vector2 UV;
};