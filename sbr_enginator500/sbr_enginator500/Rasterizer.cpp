// INCLUDES
#include "Rasterizer.h"

#include <cstring>		// For memset() in clear()
#include <cmath>		// For fabs() in drawLine()
#include <algorithm>	// For swap() in drawLine()

// DATA STRUCTURES

// Helper structure for triangle rasterization. It is used to fill the pixels in the Y direction
struct Edge
{
	Edge(int px1, int py1, const Color& clr1, int const px2, int const py2, const Color& clr2)
	{
		// The point having the lower Y value is 1st one. Because algorithm works from the lowest Y value to highest one.
		if (py1 < py2)
		{
			x1 = px1;
			y1 = py1;
			color1 = clr1;

			x2 = px2;
			y2 = py2;
			color2 = clr2;
		}
		else
		{
			x1 = px2;
			y1 = py2;
			color1 = clr2;

			x2 = px1;
			y2 = py1;
			color2 = clr1;

		}
	}	// End of constructor

	int x1, y1, x2, y2;
	Color color1;
	Color color2;
	//const Color* color1;
	//const Color* color2;
};

// Helper structure to fill the pixels in the X direction
struct Span
{
	Span(int px1, const Color& clr1, int const px2, const Color& clr2)
	{
		// The point having the lower X value is 1st one. Because algorithm works from the lowest X value to highest one.
		if (px1 < px2)
		{
			x1 = px1;
			color1 = clr1;

			x2 = px2;
			color2 = clr2;
		}
		else
		{
			x1 = px2;
			color1 = clr2;

			x2 = px1;
			color2 = clr2;
		}
	}

	int x1, x2;
	Color color1;
	Color color2;
	/*const Color* color1;
	const Color* color2;*/
};


// CONSTRUCTORS

// Set frame buffer and related members
Rasterizer::Rasterizer(uint32_t* frameBuffer, unsigned int width, unsigned int height)
{
	mFrameBuffer = frameBuffer;

	mWidth = width;
	mHeight = height;

}

// PUBLIC METHODS

// Draw a line with given information. TODO	USE DDA
void Rasterizer::drawLine(float x1, float y1, const Color& color1, float x2, float y2, const Color& color2)
{

	Bresenham((int)(x1 + 0.5f), (int)(y1 + 0.5f), color1, (int)(x2 + 0.5f), (int)(y2 + 0.5f), color2);

	//oldBresenham((int)(x1 + 0.5f), (int)(y1 + 0.5f), color1, (int)(x2 + 0.5f), (int)(y2 + 0.5f), color2);
}

// Raster the triangle with given 3 points
void Rasterizer::rasterTriangle(int x1, int y1, const Color& color1, int const x2, int const y2, const Color& color2, int x3, int y3, const Color& color3)
{
	// Create edges of the triangle
	Edge edges[3] = {
						Edge(x1, y1, color1, x2, y2, color2),
						Edge(x2, y2, color2, x3, y3, color3),
						Edge(x3, y3, color3, x1, y1, color1)
					};

	// Find the longest edge in Y direction
	int maxLength = 0, longEdgeId = 0;

	for (int ei = 0; ei < 3; ei++)
	{
		int length = edges[ei].y2 - edges[ei].y1;	// Since, we keep the Y values sorted, we guarantee that the result'll be positive

		if (length > maxLength)
		{
			maxLength = length;
			longEdgeId = ei;
		}
	}

	// Set other two short edges
	int shotEdgeId1 = (longEdgeId + 1) % 3;	// mod(x,3), for keeping the indices in [0, 2] range
	int shotEdgeId2 = (longEdgeId + 2) % 3;	// mod(x,3), for keeping the indices in [0, 2] range

	// Start rasterization process
	raster(edges[longEdgeId], edges[shotEdgeId1]);
	raster(edges[longEdgeId], edges[shotEdgeId2]);
}

// PRIVATE METHODS

void Rasterizer::Bresenham(int x1, int y1, const Color& color1, int const x2, int const y2, const Color& color2)
{
	int delta_x(x2 - x1);
	// if x1 == x2, then it does not matter what we set here
	signed char const ix((delta_x > 0) - (delta_x < 0));
	delta_x = std::abs(delta_x) << 1;

	int delta_y(y2 - y1);
	// if y1 == y2, then it does not matter what we set here
	signed char const iy((delta_y > 0) - (delta_y < 0));
	delta_y = std::abs(delta_y) << 1;

	setPixel((unsigned int)x1, (unsigned int)y1, color1);

	//plot(x1, y1);

	if (delta_x >= delta_y)
	{
		// error may go below zero
		int error(delta_y - (delta_x >> 1));

		while (x1 != x2)
		{
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= delta_x;
				y1 += iy;
			}
			// else do nothing

			error += delta_y;
			x1 += ix;

			setPixel((unsigned int)x1, (unsigned int)y1, color1); //plot(x1, y1);
		}
	}
	else
	{
		// error may go below zero
		int error(delta_x - (delta_y >> 1));

		while (y1 != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= delta_y;
				x1 += ix;
			}
			// else do nothing

			error += delta_x;
			y1 += iy;

			setPixel((unsigned int)x1, (unsigned int)y1, color1); //plot(x1, y1);
		}
	}
}

// The one from https://github.com/joshb/linedrawing/blob/master/Rasterizer.cpp
void Rasterizer::oldBresenham(int x1, int y1, const Color& color1, int const x2, int const y2, const Color& color2)
{
	float xdiff = (x2 - x1);
	float ydiff = (y2 - y1);

	if (xdiff == 0.0f && ydiff == 0.0f) {
		setPixel((unsigned int)x1, (unsigned int)y1, color1);
		return;
	}

	if (fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

		// set xmin to the lower x value given
		// and xmax to the higher value
		if (x1 < x2) {
			xmin = x1;
			xmax = x2;
		}
		else {
			xmin = x2;
			xmax = x1;
		}

		// draw line in terms of y slope
		float slope = ydiff / xdiff;
		for (float x = xmin; x <= xmax; x += 1.0f) {
			float y = y1 + ((x - x1) * slope);
			Color color = color1 + ((color2 - color1) * ((x - x1) / xdiff));
			setPixel((unsigned int)x, (unsigned int)y, color);
		}
	}
	else {
		float ymin, ymax;

		// set ymin to the lower y value given
		// and ymax to the higher value
		if (y1 < y2) {
			ymin = y1;
			ymax = y2;
		}
		else {
			ymin = y2;
			ymax = y1;
		}

		// draw line in terms of x slope
		float slope = xdiff / ydiff;
		for (float y = ymin; y <= ymax; y += 1.0f) {
			float x = x1 + ((y - y1) * slope);
			Color color = color1 + ((color2 - color1) * ((y - y1) / ydiff));
			setPixel((unsigned int)x, (unsigned int)y, color);
		}
	}
}	// End of oldBresenham

// Raster the given edges. It is used for triangle rasterization
void Rasterizer::raster(const Edge& edge1, const Edge& edge2)
{
	// calculate difference between the y coordinates
	// of the first edge and return if 0
	float e1ydiff = (float)(edge1.y2 - edge1.y1);
	if (e1ydiff == 0.0f)
		return;

	// calculate difference between the y coordinates
	// of the second edge and return if 0
	float e2ydiff = (float)(edge2.y2 - edge2.y1);
	if (e2ydiff == 0.0f)
		return;

	// calculate differences between the x coordinates
	// and colors of the points of the edges
	float e1xdiff = (float)(edge1.x2 - edge1.x1);
	float e2xdiff = (float)(edge2.x2 - edge2.x1);
	Color e1colordiff = (edge1.color2 - edge1.color1);
	Color e2colordiff = (edge2.color2 - edge2.color1);

	// calculate factors to use for interpolation
	// with the edges and the step values to increase
	// them by after drawing each span
	float factor1 = (float)(edge2.y1 - edge1.y1) / e1ydiff;
	float factorStep1 = 1.0f / e1ydiff;
	float factor2 = 0.0f;
	float factorStep2 = 1.0f / e2ydiff;

	// loop through the lines between the edges and draw spans
	for (int y = edge2.y1; y < edge2.y2; y++) {
		// create and draw span
		Span span(edge1.x1 + (int)(e1xdiff * factor1),
			edge1.color1 + (e1colordiff * factor1),
			edge2.x1 + (int)(e2xdiff * factor2),
			edge2.color1 + (e2colordiff * factor2));
		drawSpan(span, y);

		// increase factors
		factor1 += factorStep1;
		factor2 += factorStep2;
	}



	//// Calculate the difference between Y coordinates of the edge
	//float yDifE1 = (float)edge1.y2 - edge1.y1;
	//if (yDifE1 == 0.f)	// Return if the diffeence is zero
	//{
	//	return;
	//}

	//float yDifE2 = (float)edge2.y2 - edge2.y1;
	//if (yDifE2 == 0.f)	// Return if the diffeence is zero
	//{
	//	return;
	//}

	//// Get the differences between X coordinates
	//float xDifE1 = (float)edge1.x2 - edge1.x1;
	//float xDifE2 = (float)edge2.x2 - edge2.x2;

	//// Get the differences between the colors
	//Color clrDifE1 = edge1.color2 - edge1.color1;
	//Color clrDifE2 = edge2.color2 - edge2.color2;

	//// Calculate interpolation factors, and steps
	//float factor1 = (float)(edge2.y1 - edge1.y1) / yDifE1;
	//float factorStep1 = 1.0f / yDifE1;

	//float factor2 = 0.0f;
	//float factorStep2 = 1.0f / yDifE2;

	//// Loop through the lines between the edges and draw spans
	//for (int y = edge2.y1; y < edge2.y2; y++)
	//{
	//	// Create span
	//	Span span(	edge1.x1 + (int)(xDifE1 * factor1), edge1.color1 + (clrDifE1 * factor1),
	//				edge2.x1 + (int)(xDifE2 * factor2), edge2.color1 + (clrDifE2 * factor2));

	//	// Draw span
	//	drawSpan(span, y);

	//	// increase factors
	//	factor1 += factorStep1;
	//	factor2 += factorStep2;
	//}


}	// End of raster()

// Draw spans (X coordinate values) during triangle rasterization
void Rasterizer::drawSpan(const Span& span, int y)
{
	int xDif = span.x2 - span.x1;
	if (xDif == 0)
	{
		return;
	}

	Color colordif = span.color2 - span.color1;

	float factor = 0.0f;
	float factorStep = 1.0f / (float)xDif;

	// Draw each pixel in the span
	for (int x = span.x1; x < span.x2; x++)
	{
		setPixel((unsigned int)x, (unsigned int)y, span.color1 + (colordif * factor));
		factor += factorStep;
	}

}	// End of drawSpan()

// Set the specified pixel with the color (takes float)
void Rasterizer::setPixel(float x, float y, const Color& color)
{
	if (x < 0.0f || y < 0.0f)	// >= width/height will be check at setPixel(unsigned int). No need to check twice
	{
		return;
	}

	setPixel((unsigned int)x, (unsigned int)y, color);
}

// Set the specified pixel with the color (takes int)
void Rasterizer::setPixel(unsigned int x, unsigned y, const Color& color)
{
	if (x >= mWidth || y >= mHeight)	// Since x and y are unsigned int, we do not need to check with < 0 condition
	{
		return;
	}

	mFrameBuffer[y * mWidth + x] = color.toInt32();
}

// Clear frame buffer
void Rasterizer::clear()
{
	memset(mFrameBuffer, 0, sizeof(uint32_t) * mWidth * mHeight);
}