#include "Draw Manager.hpp"

namespace RENDER
{
	unsigned int CreateF(std::string font_name, int size, int weight, int blur, int scanlines, int flags)
	{
		auto font = g_VGuiSurface->SCreateFont();
		g_VGuiSurface->SetFontGlyphSet(font, font_name.c_str(), size, weight, blur, scanlines, flags);
		return font;
	}

	void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, CColor Color, std::string Input)
	{
		size_t size = Input.size() + 1;
		auto wide_buffer = std::make_unique<wchar_t[]>(size);
		mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);
		int width = 0, height = 0;
		g_VGuiSurface->GetTextSize(Font, wide_buffer.get(), width, height);

		if (!center_width)
			width = 0;

		if (!center_height)
			height = 0;

		g_VGuiSurface->DrawSetTextColor(Color);
		g_VGuiSurface->DrawSetTextFont(Font);
		g_VGuiSurface->DrawSetTextPos(X - (width * .5), Y - (height * .5));
		g_VGuiSurface->DrawPrintText(wide_buffer.get(), wcslen(wide_buffer.get()), 0);
	}

	void DrawWF(int X, int Y, unsigned int Font, CColor Color, const wchar_t* Input)
	{
		g_VGuiSurface->DrawSetTextColor(Color);
		g_VGuiSurface->DrawSetTextFont(Font);
		g_VGuiSurface->DrawSetTextPos(X, Y);
		g_VGuiSurface->DrawPrintText(Input, wcslen(Input), 0);
	}

	Vector2D GetTextSize(unsigned int Font, std::string Input)
	{
		size_t size = Input.size() + 1;
		auto wide_buffer = std::make_unique<wchar_t[]>(size);
		mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);
		int width, height;
		g_VGuiSurface->GetTextSize(Font, wide_buffer.get(), width, height);
		return Vector2D(width, height);
	}

	void GetTextSize(unsigned long font, const char* txt, int& width, int& height)
	{
		FUNCTION_GUARD;
		size_t origsize = strlen(txt) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		int x, y;
		mbstowcs_s(&convertedChars, wcstring, origsize, txt, _TRUNCATE);
		g_VGuiSurface->GetTextSize(font, wcstring, x, y);
		width = x;
		height = y;
	}

	void FillRectangle(int x1, int y2, int width, int height, CColor color)
	{
		g_VGuiSurface->DrawSetTextColor(color);
		g_VGuiSurface->DrawFilledRect(x1, y2, x1 + width, y2 + height);
	}

	void DrawLine(int x1, int y1, int x2, int y2, CColor color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawLine(x1, y1, x2, y2);
	}

	void DrawEmptyRect(int x1, int y1, int x2, int y2, CColor color, unsigned char ignore_flags)
	{
		g_VGuiSurface->DrawSetColor(color);

		if (!(ignore_flags & 0b1))
			g_VGuiSurface->DrawLine(x1, y1, x2, y1);

		if (!(ignore_flags & 0b10))
			g_VGuiSurface->DrawLine(x2, y1, x2, y2);

		if (!(ignore_flags & 0b100))
			g_VGuiSurface->DrawLine(x2, y2, x1, y2);

		if (!(ignore_flags & 0b1000))
			g_VGuiSurface->DrawLine(x1, y2, x1, y1);
	}

	void DrawCornerRect(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const bool outlined, const CColor& color, const CColor& outlined_color)
	{
		auto corner = [&](const int32_t _x, const int32_t _y, const int32_t width, const int32_t height, const bool right_side, const bool down, const CColor& _color, const bool _outlined, const CColor& _outlined_color) -> void
		{
			const auto corner_x = right_side ? _x - width : _x;
			const auto corner_y = down ? _y - height : _y;
			const auto corner_w = down && right_side ? width + 1 : width;
			DrawEmptyRect(corner_x, _y, corner_w, 1, _color);
			DrawEmptyRect(_x, corner_y, 1, height, _color);

			if (_outlined)
			{
				DrawEmptyRect(corner_x, down ? _y + 1 : _y - 1, !down && right_side ? corner_w + 1 : corner_w, 1, _outlined_color);
				DrawEmptyRect(right_side ? _x + 1 : _x - 1, down ? corner_y : corner_y - 1, 1, down ? height + 2 : height + 1, _outlined_color);
			}
		};

		corner(x - w, y, w / 2, w / 2, false, false, color, outlined, outlined_color);
		corner(x - w, y + h, w / 2, w / 2, false, true, color, outlined, outlined_color);
		corner(x + w, y, w / 2, w / 2, true, false, color, outlined, outlined_color);
		corner(x + w, y + h, w / 2, w / 2, true, true, color, outlined, outlined_color);
	}

	void DrawEdges(float topX, float topY, float bottomX, float bottomY, float length, CColor color)
	{
		float scale = (bottomY - topY) / 5.0f;
		DrawLine(topX - scale, topX - scale + length, topY, topY, color);
		DrawLine(topX - scale, topX - scale, topY, topY + length, color);
		DrawLine(topX + scale, topX + scale + length, topY, topY, color);
		DrawLine(topX + scale + length, topX + scale + length, topY, topY + length, color);
		DrawLine(bottomX - scale, topX - scale + length, bottomY, bottomY, color);
		DrawLine(bottomX - scale, topX - scale, bottomY, bottomY - length, color);
		DrawLine(bottomX + scale, topX + scale + length, bottomY, bottomY, color);
		DrawLine(bottomX + scale + length, topX + scale + length, bottomY, bottomY - length, color);
	}

	void DrawFilledRect(int x1, int y1, int x2, int y2, CColor color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawFilledRect(x1, y1, x2, y2);
	}

	void DrawFilledRectOutline(int x1, int y1, int x2, int y2, CColor color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawFilledRect(x1, y1, x2, y2);
		DrawEmptyRect(x1 - 1, y1 - 1, x2, y2, CColor(0, 0, 0, 100));
	}

	void DrawFilledRectArray(IntRect* rects, int rect_amount, CColor color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawFilledRectArray(rects, rect_amount);
	}

	void DrawCircle(int x, int y, int radius, int segments, CColor color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawOutlinedCircle(x, y, radius, segments);
	}

	void TexturedPolygon(int n, std::vector<Vertex_t> vertice, CColor color)
	{
		static int texture_id = g_VGuiSurface->CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		g_VGuiSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawSetTexture(texture_id);
		g_VGuiSurface->DrawTexturedPolygon(n, vertice.data());
	}

	void DrawFilledCircle(int x, int y, int radius, int segments, CColor color)
	{
		std::vector<Vertex_t> vertices;
		float step = M_PI * 2.0f / segments;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + x, radius * sinf(a) + y)));

		TexturedPolygon(vertices.size(), vertices, color);
	}

	bool ScreenTransform(const Vector& in, QAngle& out)
	{
		static ptrdiff_t ptrViewMatrix;

		if (!ptrViewMatrix)
		{
			ptrViewMatrix = static_cast<ptrdiff_t>(Utils::FindSignature("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			ptrViewMatrix += 0x3;
			ptrViewMatrix = *reinterpret_cast<uintptr_t*>(ptrViewMatrix);
			ptrViewMatrix += 176;
		}

		int ScrW, ScrH;
		const VMatrix& worldToScreen = g_EngineClient->WorldToScreenMatrix();
		g_EngineClient->GetScreenSize(ScrW, ScrH);
		float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
		out.roll = 0;

		if (w > 0.01)
		{
			float inverseWidth = 1 / w;
			out.pitch = (ScrW / 2) + (0.5 * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * inverseWidth) * ScrW + 0.5);
			out.yaw = (ScrH / 2) - (0.5 * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * inverseWidth) * ScrH + 0.5);
			return true;
		}

		return false;
	}

	bool WorldToScreen(const Vector& in, QAngle& out)
	{
		if (ScreenTransform(in, out))
		{
			int w, h;
			g_EngineClient->GetScreenSize(w, h);
			out.pitch = (w / 2.0f) + (out.pitch * w) / 2.0f;
			out.yaw = (h / 2.0f) - (out.yaw * h) / 2.0f;
			return true;
		}

		return false;
	}

	bool WorldToScreen(const Vector& in, Vector& out)
	{
		if (TransformScreen(in, out))
		{
			int w, h;
			g_EngineClient->GetScreenSize(w, h);
			out.x = (w / 2.0f) + (out.x * w) / 2.0f;
			out.y = (h / 2.0f) - (out.y * h) / 2.0f;
			return true;
		}

		return false;
	}

	bool WorldToScreen2(const Vector& in, Vector& position)
	{
		return (g_DebugOverlay->ScreenPosition(in, position) != 1);
	}

	bool WorldToScreen3(Vector& in, Vector& out)
	{
		const VMatrix& worldToScreen = g_EngineClient->WorldToScreenMatrix();
		float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
		out.z = 0;

		if (w > 0.001)
		{
			RECT ScreenSize = GetViewport();
			float fl1DBw = 1 / w;
			out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f);
			out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f);
			return true;
		}

		return false;
	}

	bool WorldToScreenESP(Vector world, Vector& screen)
	{
		return (g_DebugOverlay->ScreenPosition(world, screen) != 1);
	}

	RECT GetViewport()
	{
		RECT Viewport = { 0, 0, 0, 0 };
		int w, h;
		g_EngineClient->GetScreenSize(w, h);
		Viewport.right = w; Viewport.bottom = h;
		return Viewport;
	}

	bool TransformScreen(const Vector& in, Vector& out)
	{
		static ptrdiff_t ptrViewMatrix;

		if (!ptrViewMatrix)
		{
			ptrViewMatrix = static_cast<ptrdiff_t>(Utils::FindSignature("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			ptrViewMatrix += 0x3;
			ptrViewMatrix = *reinterpret_cast<uintptr_t*>(ptrViewMatrix);
			ptrViewMatrix += 176;
		}

		int ScrW, ScrH;
		const VMatrix& worldToScreen = g_EngineClient->WorldToScreenMatrix();
		g_EngineClient->GetScreenSize(ScrW, ScrH);
		float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
		out.z = 0;

		if (w > 0.01)
		{
			float inverseWidth = 1 / w;
			out.x = (ScrW / 2) + (0.5 * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * inverseWidth) * ScrW + 0.5);
			out.y = (ScrH / 2) - (0.5 * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * inverseWidth) * ScrH + 0.5);
			return true;
		}

		return false;
	}

	void Line(int x, int y, int x2, int y2, CColor color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawLine(x, y, x2, y2);
	}

	void DrawOutlinedRect(int x, int y, int w, int h, CColor col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	std::wstring stringToWide(const std::string& text)
	{
		std::wstring wide(text.length(), L' ');
		std::copy(text.begin(), text.end(), wide.begin());

		return wide;
	}

	int GetWidth(const char* input, unsigned int font)
	{
		int iWide = 0;
		int iTall = 0;
		int iBufSize = MultiByteToWideChar(CP_UTF8, 0x0, input, -1, NULL, 0);
		wchar_t* pszUnicode = new wchar_t[iBufSize];
		MultiByteToWideChar(CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize);
		g_VGuiSurface->GetTextSize(font, pszUnicode, iWide, iTall);
		delete[] pszUnicode;
		return iWide;
	}

	void StringA(unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char* input, ...)
	{
		CHAR szBuffer[MAX_PATH];

		if (!input)
			return;

		vsprintf_s(szBuffer, input, (char*)&input + _INTSIZEOF(input));

		if (center)
			x -= GetWidth(szBuffer, Font) / 2;

		g_VGuiSurface->DrawSetTextColor(r, g, b, a);
		g_VGuiSurface->DrawSetTextFont(Font);
		g_VGuiSurface->DrawSetTextPos(x, y);
		std::wstring wide = stringToWide(std::string(szBuffer));
		g_VGuiSurface->DrawPrintText(wide.c_str(), wide.length());
	}

	void StringA(unsigned long Font, bool center, int x, int y, CColor color, const char* input, ...)
	{
		CHAR szBuffer[MAX_PATH];

		if (!input)
			return;

		vsprintf_s(szBuffer, input, (char*)&input + _INTSIZEOF(input));

		if (center)
			x -= GetWidth(szBuffer, Font) / 2;

		g_VGuiSurface->DrawSetTextColor(color);
		g_VGuiSurface->DrawSetTextFont(Font);
		g_VGuiSurface->DrawSetTextPos(x, y);
		std::wstring wide = stringToWide(std::string(szBuffer));
		g_VGuiSurface->DrawPrintText(wide.c_str(), wide.length());
	}

	void DrawRect(int x, int y, int w, int h, CColor col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
	}

	void Draw3DCube(float scalar, QAngle angles, Vector middle_origin, CColor outline)
	{
		Vector Points[8];
		Vector PointsScreen[8];
		Vector forward, right, up;
		Math::AngleVectors(angles, forward, right, up);

		Points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar);
		Points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar);
		Points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar);
		Points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar);

		Points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar);
		Points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar);
		Points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar);
		Points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar);

		for (int i = 0; i < 8; i++)
			if (!Math::WorldToScreen(Points[i], PointsScreen[i]))
				return;

		g_VGuiSurface->DrawSetColor(outline);

		g_VGuiSurface->DrawLine(PointsScreen[0].x, PointsScreen[0].y, PointsScreen[1].x, PointsScreen[1].y);
		g_VGuiSurface->DrawLine(PointsScreen[0].x, PointsScreen[0].y, PointsScreen[2].x, PointsScreen[2].y);
		g_VGuiSurface->DrawLine(PointsScreen[3].x, PointsScreen[3].y, PointsScreen[1].x, PointsScreen[1].y);
		g_VGuiSurface->DrawLine(PointsScreen[3].x, PointsScreen[3].y, PointsScreen[2].x, PointsScreen[2].y);

		g_VGuiSurface->DrawLine(PointsScreen[0].x, PointsScreen[0].y, PointsScreen[4].x, PointsScreen[4].y);
		g_VGuiSurface->DrawLine(PointsScreen[1].x, PointsScreen[1].y, PointsScreen[5].x, PointsScreen[5].y);
		g_VGuiSurface->DrawLine(PointsScreen[2].x, PointsScreen[2].y, PointsScreen[6].x, PointsScreen[6].y);
		g_VGuiSurface->DrawLine(PointsScreen[3].x, PointsScreen[3].y, PointsScreen[7].x, PointsScreen[7].y);

		g_VGuiSurface->DrawLine(PointsScreen[4].x, PointsScreen[4].y, PointsScreen[5].x, PointsScreen[5].y);
		g_VGuiSurface->DrawLine(PointsScreen[4].x, PointsScreen[4].y, PointsScreen[6].x, PointsScreen[6].y);
		g_VGuiSurface->DrawLine(PointsScreen[7].x, PointsScreen[7].y, PointsScreen[5].x, PointsScreen[5].y);
		g_VGuiSurface->DrawLine(PointsScreen[7].x, PointsScreen[7].y, PointsScreen[6].x, PointsScreen[6].y);
	}

	void DrawString(unsigned long font, int x, int y, CColor Color, unsigned long alignment, const char* msg, ...)
	{
		FUNCTION_GUARD;
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, msg);
		_vsnprintf(buf, sizeof(buf), msg, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);
		int r = 255, g = 255, b = 255, a = 255;
		int width, height;
		g_VGuiSurface->GetTextSize(font, wbuf, width, height);

		if (alignment & FONT_RIGHT)
			x -= width;

		if (alignment & FONT_CENTER)
			x -= width / 2;

		g_VGuiSurface->DrawSetTextFont(font);
		g_VGuiSurface->DrawSetTextColor(Color);
		g_VGuiSurface->DrawSetTextPos(x, y - height / 2);
		g_VGuiSurface->DrawPrintText(wbuf, wcslen(wbuf));
	}
}