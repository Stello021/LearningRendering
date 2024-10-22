#include "TriangleRastering.h"
 
//Scanline implementation is compoed by 3 steps
//1. Sort vertices of the triangle by their y-coordinates
//2. Rasterize left and right sides of triangle
//3. Draw horizontal segments between the left and right boundary points
void DrawFilledTriangle_MonoThread(ScreenPoint vertex1, ScreenPoint vertex2, ScreenPoint vertex3, TGAImage& image, TGAColor color)
{
	//Sort the vertices upper-to-lower
	if (vertex2.Y > vertex1.Y)
	{
		std::swap(vertex1, vertex2);
	}
	if (vertex3.Y > vertex1.Y)
	{
		std::swap(vertex1, vertex3);
	}
	if (vertex3.Y > vertex2.Y)
	{
		std::swap(vertex2, vertex3);
	}

	//First side (left or right) = 1->2 & 2->3
	//Other side = 1 -> 3

	//First side is made of two segments, so drawing will be divided from bottom half and upper half

	const int totalHeight = vertex1.Y - vertex3.Y;
	const int FirstSegmentHeight = vertex2.Y - vertex3.Y + 1; // +1 ensures that the top pixel  of this segment is included

	//Bottom
	for (int y = vertex3.Y; y <= vertex2.Y; y++)
	{
		//Fractional values representing how far the current y value is between the start and end points of the triangle side
		float alpha = static_cast<float>(y - vertex3.Y) / totalHeight; //interpolate along the full height of the triangle from vertex3 to vertex1
		float beta = static_cast<float>(y - vertex3.Y) / FirstSegmentHeight; //interpolate along the lower segment of the triangle from vertex3 to vertex2

		ScreenPoint A = vertex3 + (vertex1 - vertex3) * alpha; //interpolated point along the full triangle height
		ScreenPoint B = vertex3 + (vertex2 - vertex3) * beta; //interpolated point along the first segment

		if (A.X > B.X)
		{
			std::swap(A, B); //So that A represent left point and B right point
		}

		for (int j = A.X; j <= B.X; j++)
		{
			//Fills the horizontal span between two edges of the triangle
			image.SetColor(j, y, color);
		}
	}

	//Upper
	const int SecondSegmentHeight = vertex1.Y - vertex2.Y + 1;

	for (int y = vertex2.Y; y <= vertex1.Y; y++)
	{
		//Fractional values representing how far the current y value is between the start and end points of the triangle side
		float alpha = static_cast<float>(y - vertex3.Y) / totalHeight; //interpolate along the full height of the triangle from vertex3 to vertex1
		float beta = static_cast<float>(y - vertex2.Y) / SecondSegmentHeight; //interpolate along the lower segment of the triangle from vertex2 to vertex1

		ScreenPoint A = vertex3 + (vertex1 - vertex3) * alpha;
		ScreenPoint B = vertex2 + (vertex1 - vertex2) * beta; 

		if (A.X > B.X)
		{
			std::swap(A, B); 
		}

		for (int j = A.X; j <= B.X; j++)
		{
			image.SetColor(j, y, color);
		}
	}
}
