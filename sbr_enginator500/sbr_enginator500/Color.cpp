// INCLUDES
#include "Color.h"

// CONSTRUCTORS

// Generate color. Default is white with alpha = 1
Color::Color(float r, float g, float b, float a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

// PUBLIC METHODS

// Converts color to int32 for frame buffer
uint32_t Color::toInt32() const
{
	uint32_t r = (uint32_t)(R * 255.0f);
	uint32_t g = (uint32_t)(G * 255.0f);
	uint32_t b = (uint32_t)(B * 255.0f);
	uint32_t a = (uint32_t)(A * 255.0f);

	// Conversion: http://www.vcskicks.com/color-uint.php
	return (a << 24) | (r << 16) | (g << 8) | b;
}

// OPERATOR OVERLOADING

// Sum the colors to get a new color
Color Color::operator + (const Color &c) const
{
	return Color(R + c.R, G + c.G, B + c.B, A + c.A);
}

// Substract the colors to get a new color
Color Color::operator - (const Color &c) const
{
	return Color(R - c.R, G - c.G, B - c.B, A - c.A);
}

// Scale color with a constant
Color Color::operator * (float f) const
{
	return Color(R * f, G * f, B * f, A * f);
}