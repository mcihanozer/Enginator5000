#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "Color.h"

struct Edge;
struct Span;

class Rasterizer
{
public:
	// CONSTRUCTORS
	Rasterizer()	{}

	Rasterizer(uint32_t* frameBuffer, unsigned int width = 1024, unsigned int height = 768);	// Set frame buffer and related members

	// METHODS
	void drawLine(float x1, float y1, const Color& color1, float x2, float y2, const Color& color2);	// Draw a line with given information. TODO	USE DDA

	// Raster the triangle with given 3 points
	void rasterTriangle(int x1, int y1, const Color& color1, int const x2, int const y2, const Color& color2, int x3, int y3, const Color& color3);

	void clear();	// Clear frame buffer

	unsigned int mWidth, mHeight;	// Width and height of the frame buffer (window surface)

private:
	// PRIVATE METHODS
	void Bresenham(int x1, int y1, const Color& color1, int const x2, int const y2, const Color& color2);	// The from the web site
	void oldBresenham(int x1, int y1, const Color& color1, int const x2, int const y2, const Color& color2);	// The one from https://github.com/joshb/linedrawing/blob/master/Rasterizer.cpp

	void raster(const Edge& edge1, const Edge& edge2);	// Raster the given edges. It is used for triangle rasterization

	void drawSpan(const Span& span, int y);	// Draw spans (X coordinate values) during triangle rasterization

	void setPixel(float x, float y, const Color& color = Color());	// Set the specified pixel with the color (takes float)
	void setPixel(unsigned int x, unsigned y, const Color& color = Color());	// Set the specified pixel with the color (takes int)

	

	// MEMEBRS
	uint32_t* mFrameBuffer;	// Frame buffer keeping the pixels
	
};

#endif