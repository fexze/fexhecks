#pragma once

#include <cstdint>

struct Color
{
	int red, green, blue, alpha;

	Color()
	{
		this->red = 0;
		this->green = 0;
		this->blue = 0;
		this->alpha = 255;
	}

	Color(int r, int g, int b, int a = 255) : red{ r }, green{ g }, blue{ b }, alpha{ a } { }

	Color& operator *=(float coeff)
	{
		this->red = static_cast<int>(this->red * coeff);
		this->green = static_cast<int>(this->green * coeff);
		this->blue = static_cast<int>(this->blue * coeff);
		return *this;
	}

	Color& operator /=(float div)
	{
		const float flDiv = 1.f / div;
		*this *= flDiv;
		return *this;
	}

	Color operator* (float coeff) const
	{
		Color color = *this;
		return color *= coeff;
	}

	unsigned char _CColor[4];

	void GetColor(int &_r, int &_g, int &_b, int &_a) const
	{
		_r = _CColor[0];
		_g = _CColor[1];
		_b = _CColor[2];
		_a = _CColor[3];
	}

	static Color Black()
	{
		return { 0, 0, 0 };
	}

	static Color Grey()
	{
		return { 127, 127, 127 };
	}

	static Color White()
	{
		return { 255, 255, 255 };
	}

	static Color Red()
	{
		return { 255, 0, 0 };
	}

	static Color Green()
	{
		return { 0, 255, 0 };
	}

	static Color Blue()
	{
		return { 0, 0, 255 };
	};

	int GetRawColor() const
	{
		return *((int *)this);
	}
};