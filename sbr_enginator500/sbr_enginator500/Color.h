#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>

class Color
{
public:
	// CONSTRUCTORS
	//Color()	{}

	Color(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f);	// Generate color. Default is white with alpha = 1

	// METHODS
	uint32_t toInt32() const;	// Converts color to int32 for frame buffer

	// OPERATOR OVERLOADING
	Color operator + (const Color &c) const;	// Sum the colors to get a new color
	Color operator - (const Color &c) const;	// Substract the colors to get a new color
	Color operator * (float f) const;			// Scale color with a constant

	// MEMBERS
	float R, G, B, A;	// Color components
};

#endif