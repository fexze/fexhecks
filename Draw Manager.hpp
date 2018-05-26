#pragma once

#include "SDK.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Vector.hpp"
#include "CColor.hpp"

namespace RENDER
{
	RECT GetViewport();
	bool WorldToScreen3(Vector& in, Vector& out);
	bool WorldToScreen(const Vector& in, QAngle& out);
	bool WorldToScreen(const Vector& in, Vector& out);
	int GetWidth(const char* input, unsigned int font);
	std::wstring stringToWide(const std::string& text);
	bool WorldToScreenESP(Vector world, Vector& screen);
	bool ScreenTransform(const Vector& in, QAngle& out);
	bool TransformScreen(const Vector& in, Vector& out);
	void Line(int x, int y, int x2, int y2, CColor color);
	void DrawRect(int x, int y, int w, int h, CColor col);
	bool WorldToScreen2(const Vector& in, Vector& position);
	Vector2D GetTextSize(unsigned int Font, std::string Input);
	void DrawLine(int x1, int y1, int x2, int y2, CColor color);
	void DrawOutlinedRect(int x, int y, int w, int h, CColor col);
	void DrawFilledRect(int x1, int y1, int x2, int y2, CColor color);
	void DrawCircle(int x, int y, int radius, int segments, CColor color);
	void FillRectangle(int x1, int y2, int width, int height, CColor color);
	void DrawFilledRectArray(IntRect* rects, int rect_amount, CColor color);
	void DrawFilledRectOutline(int x1, int y1, int x2, int y2, CColor color);
	void TexturedPolygon(int n, std::vector<Vertex_t> vertice, CColor color);
	void DrawFilledCircle(int x, int y, int radius, int segments, CColor color);
	void GetTextSize(unsigned long font, const char* txt, int& width, int& height);
	void DrawWF(int X, int Y, unsigned int Font, CColor Color, const wchar_t* Input);
	void Draw3DCube(float scalar, QAngle angles, Vector middle_origin, CColor outline);
	void DrawEmptyRect(int x1, int y1, int x2, int y2, CColor color, unsigned char = 0);
	void DrawEdges(float topX, float topY, float bottomX, float bottomY, float length, CColor color);
	void StringA(unsigned long Font, bool center, int x, int y, CColor color, const char* input, ...);
	unsigned int CreateF(std::string font_name, int size, int weight, int blur, int scanlines, int flags);
	void DrawString(unsigned long font, int x, int y, CColor Color, unsigned long alignment, const char* msg, ...);
	void StringA(unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char* input, ...);
	void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, CColor Color, std::string Input);
	void DrawCornerRect(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const bool outlined, const CColor& color, const CColor& outlined_color);
}