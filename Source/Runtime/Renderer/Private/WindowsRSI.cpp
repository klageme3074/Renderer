
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "TriangleRasterizer.h"

void WindowsRSI::Init(const bool InbSRGBColorSpace)
{
	InitializeGDI(DisplaySetting::Inst().GetSize(), InbSRGBColorSpace);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{

}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawScreenPoint(const ScreenPoint & InPoint, const LinearColor & InColor)
{
	SetPixelWithAlpha(InPoint, InColor);
}

void WindowsRSI::DrawLine(const Vector2 & InStartPos, const Vector2 & InEndPos, const LinearColor & InColor, int InThickness)
{
	if (InThickness < 1)
	{
		return;
	}

	if (InThickness == 1)
	{
		ScreenPoint startPos(InStartPos);
		ScreenPoint endPos(InEndPos);

		int dx = Math::Abs(endPos.X - startPos.X);
		int sx = (startPos.X < endPos.X) ? 1 : -1;
		int dy = -Math::Abs(endPos.Y - startPos.Y);
		int sy = (startPos.Y < endPos.Y) ? 1 : -1;
		int err = dx + dy;

		int currentX = startPos.X;
		int currentY = startPos.Y;

		while (true)
		{
			SetPixelWithAlpha(ScreenPoint(currentX, currentY), InColor);

			int e2 = err * 2;
			if (e2 >= dy)
			{
				if (currentX == endPos.X)
				{
					break;
				}

				err += dy;
				currentX += sx;
			}

			if (e2 <= dx)
			{
				if (currentY == endPos.Y)
				{
					break;
				}

				err += dx;
				currentY += sy;
			}

		}

		//int dx = Math::CeilToInt(Math::Abs(InEndPos.X - InStartPos.X));
		//int sx = (InStartPos.X < InEndPos.X) ? 1 : -1;
		//int dy = Math::FloorToInt(-Math::Abs(InEndPos.Y - InStartPos.Y));
		//int sy = (InStartPos.Y < InEndPos.Y) ? 1 : -1;
		//int err = dx + dy;

		//int currentX = Math::CeilToInt(InStartPos.X);
		//int currentY = Math::FloorToInt(InStartPos.Y);
		//while (true)
		//{
		//	SetPixelWithAlpha(ScreenPoint(currentX, currentY), InColor);

		//	if (currentX == InEndPos.X && currentY == InEndPos.Y)
		//	{
		//		break;
		//	}

		//	int e2 = err * 2;
		//	if (e2 >= dy)
		//	{
		//		err += dy;
		//		currentX += sx;
		//	}

		//	if (e2 <= dx)
		//	{
		//		err += dx;
		//		currentY += sy;
		//	}
		//}

		return;
	}
	
	if (InThickness > 1)
	{
		// Draw Quad Mesh
		float thickness = (float)InThickness;
		Vector2 unitV = (InEndPos - InStartPos).Normalize();
		Vector2 unitVTurn90(-unitV.Y, unitV.X);
		Vector2 unitVTurnR90(unitV.Y, -unitV.X);

		VertexData v[4];
		v[0].Position = unitVTurn90 * thickness * 0.5f + InStartPos;
		v[0].Color = InColor;
		v[1].Position = unitVTurn90 * thickness * 0.5f + InEndPos;
		v[1].Color = InColor;
		v[2].Position = unitVTurnR90 * thickness * 0.5f + InEndPos;
		v[2].Color = InColor;
		v[3].Position = unitVTurnR90 * thickness * 0.5f + InStartPos;
		v[3].Color = InColor;

		int i[6];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 2;
		i[5] = 3;

		SetVertexBuffer(v);
		SetIndexBuffer(i);
		DrawPrimitive(4, 6);
	}

}

void WindowsRSI::DrawLine2(const ScreenPoint & InStartPos, const ScreenPoint & InEndPos,const LinearColor & InColor)
{
	int w = InEndPos.X - InStartPos.X;
	int h = InEndPos.Y - InStartPos.Y;
	//if (w == 0 || h == 0)
	//{
	//	if (h == 0)
	//	{
	//		DrawHorizontalLine(InStartPos, InEndPos, LinearColor(1.f, 0.f, 1.f).ToColor32());
	//	}
	//	else
	//	{

	//	}

	//	return;
	//}

	bool slope = (Math::Abs(w) >= Math::Abs(h));
	int dirx = w > 0 ? 1 : -1;
	int diry = h > 0 ? 1 : -1;
	int fw = dirx * w;
	int fh = diry * h;

	int f = slope ? fh * 2 - fw : 2 * fw - fh;
	int f1 = slope ? 2 * fh : 2 * fw;
	int f2 = slope ? 2 * (fh - fw) : 2 * (fw - fh);
	int x = InStartPos.X;
	int y = InStartPos.Y;

	if (slope)
	{
		while (x != InEndPos.X)
		{
			SetPixel(ScreenPoint(x, y), InColor.ToColor32());

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += diry;
			}

			x += dirx;
		}
	}
	else
	{
		while (y != InEndPos.Y)
		{
			SetPixel(ScreenPoint(x, y), InColor.ToColor32());

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dirx;
			}

			y += diry;
		}
	}

}

void WindowsRSI::DrawArrowLine(const Vector2 & InStartPos, const Vector2 & InEndPos, const LinearColor & InColor, int InThickness)
{
	static float arrowLength = 15.0f;
	DrawLine(InStartPos, InEndPos, InColor, InThickness);
	Vector2 reversedCenterLine = (InStartPos - InEndPos).Normalize() * arrowLength;

	// To Do : Draw Arrow Head!
	static float arrowAngle = Math::Deg2Rad(20.0f);
	static float sinAngle = sinf(arrowAngle);
	static float cosAngle = cosf(arrowAngle);
	static Matrix2x2 rotMat1(Vector2(cosAngle, -sinAngle), Vector2(sinAngle, cosAngle));
	static Matrix2x2 rotMat2(Vector2(cosAngle, sinAngle), Vector2(-sinAngle, cosAngle));

	Vector2 head1 = rotMat1 * reversedCenterLine + InEndPos;
	Vector2 head2 = rotMat2 * reversedCenterLine + InEndPos;
	DrawLine(head1, InEndPos, InColor, InThickness);
	DrawLine(head2, InEndPos, InColor, InThickness);
}

void WindowsRSI::DrawVerticalLine(int InX, const LinearColor & InColor)
{
	if (InX < TopLeftPoint.X || InX > BottomRightPoint.X)
	{
		return;
	}

	Color32* dest = ScreenBuffer + InX - TopLeftPoint.X;

	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		// Alpha Blending
		LinearColor bufferColor = LinearColor(*dest);
		*dest = (InColor * InColor.A + bufferColor * (1.f - InColor.A)).ToColor32();
		dest += ScreenSize.X;
	}

	return;
}

void WindowsRSI::DrawHorizontalLine(int InY, const LinearColor & InColor)
{
	if (InY > TopLeftPoint.Y || InY < BottomRightPoint.Y)
	{
		return;
	}

	Color32* dest = ScreenBuffer + (TopLeftPoint.Y - InY) * ScreenSize.X;
	for (int x = 0; x < ScreenSize.X; x++)
	{
		// Alpha Blending
		LinearColor bufferColor = LinearColor(*dest);
		*dest = (InColor * InColor.A + bufferColor * (1.f - InColor.A)).ToColor32();
		*dest++;
	}
}

void WindowsRSI::DrawHorizontalLine(const ScreenPoint & InStartPos, const ScreenPoint & InEndPos, const LinearColor & InColor)
{
	int y = InStartPos.Y;
	if (y > TopLeftPoint.Y || y < BottomRightPoint.Y)
	{
		return;
	}

	if (InStartPos.X > InEndPos.X)
	{
		return;
	}

	int diff = InEndPos.X - InStartPos.X;

	Color32* dest = ScreenBuffer + (TopLeftPoint.Y - y) * ScreenSize.X + InStartPos.X;
	for (int x = 0; x <= diff; x++)
	{
		// Alpha Blending
		LinearColor bufferColor = LinearColor(*dest);
		*dest = (InColor * InColor.A + bufferColor * (1.f - InColor.A)).ToColor32();
		*dest++;
	}
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		// Draw Each Triangle List 
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]);

		//t.RecalcBounds();

		int* contourBuffer = t.GetContourBuffer();
		int bufferSize = t.GetContourBufferSize();

		for (int y = 0; y < bufferSize; ++y)
		{
			int screenY = t.BottomLeft.Y + y;
			int startXValue = contourBuffer[y * 2 + 1];
			int endXValue = contourBuffer[y * 2];

			for (int screenX = startXValue; screenX <= endXValue; ++screenX)
			{
				ScreenPoint currentPixel(screenX, screenY);
				Vector2 currentPos = currentPixel.ToVector2();
				if (t.IsInside(currentPos))
				{
					SetPixelWithAlpha(currentPixel, t.GetColor(currentPixel.ToVector2()));
				}
			}
		}

		//for (int x = t.BottomLeft.X; x < t.TopRight.X; ++x)
		//{
		//	for (int y = t.BottomLeft.Y; y < t.TopRight.Y; ++y)
		//	{
		//		ScreenPoint currentPixel(x, y);
		//		Vector2 currentPos = currentPixel.ToVector2();
		//		if (t.IsInside(currentPos))
		//		{
		//			SetPixelWithAlpha(currentPixel, t.GetColor(currentPos));
		//		}
		//	}
		//}

	}
}
