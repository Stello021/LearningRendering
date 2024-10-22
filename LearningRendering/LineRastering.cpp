#include "LineRastering.h"
#include "OBJDecoder/OBJDecoder/Model.h"
#include "vector"
#include "OBJDecoder/OBJDecoder/VectorsMath.h"
#include "TGA-Decoder/TGAReader/tgaData.h"

void DrawLineBetween(ScreenPoint from, ScreenPoint to, TGAImage& image, TGAColor color)
{
	//Check for steepness
	bool steep = false;
	if (std::abs(from.X - to.X) < std::abs(from.Y - to.Y)) //steep if the change in Y is greater than the change in x	
	{
		//if line is steep, transpose the image (X become Y and vice versa)
		std::swap(from.X, from.Y);
		std::swap(to.X, to.Y);
		steep = true; //tracking transposition
	}

	if (from.X > to.X) //ensure left-to-right
	{
		std::swap(from.X, to.X);
		std::swap(from.Y, to.Y);
	}
	//OPTIMIZATION: Extract divisior from the loop, giving that is always the same value
	Vector2<int> delta = to - from;
	int deltaError = std::abs(delta.Y) * 2;

	int error = 0;
	int y = from.Y;
	const int yIncrement = to.Y > from.Y ? 1 : -1;

	//OPTIMIZATION: Eliminate branching inside for loops
	if (steep)
	{
		for (int x = from.X; x <= to.X; x++)
		{
			image.SetColor(y, x, color);
			error += deltaError; //return the distance from the current (x,y) pixel to the best straight line.
			if (error > delta.X) //Each time error is greater than one pixel 
			{
				y += yIncrement;
				error -= delta.X * 2;
			}
		}
	}
	else
	{
		for (int x = from.X; x <= to.X; x++)
		{
			image.SetColor(x, y, color);
			error += deltaError; //return the distance from the current (x,y) pixel to the best straight line.
			if (error > delta.X) //Each time error is greater than one pixel 
			{
				y += yIncrement;
				error -= delta.X * 2;
			}
		}
	}
}

void DrawTriangle(ScreenPoint p1, ScreenPoint p2, ScreenPoint p3, TGAImage& image, TGAColor color)
{
	DrawLineBetween(p1, p2, image, color);
	DrawLineBetween(p1, p3, image, color);
	DrawLineBetween(p2, p3, image, color);
}

void DrawWireframeModel(TGAImage& image, Model*& model, const int width, const int heigth, TGAColor color)
{
	for (int i = 0; i < model->FacesAmount(); i++)
	{
		std::vector<int> currentFace = model->Face(i);
		for (int j = 0; j < 3; j++)
		{
			Vector3<float> currentVertex = model->Vertex(currentFace[j]);
			Vector3<float> nextVertex = model->Vertex(currentFace[(j + 1) % 3]);

			//Projecting 3D Vertices to 2D 
			//Vertices are normalized (-1, 1), so they need to be mapped in screen space (or image space in this case)
			//Adding 1 shifts range from (-1, 1) to (0, 2)
			//Multiply for half-width or half-height maps the coordinate to the screen's dimensions
			ScreenPoint from((currentVertex.X + 1.) * width / 2., (currentVertex.Y + 1.) * heigth / 2.);
			ScreenPoint to((nextVertex.X + 1.) * width / 2., (nextVertex.Y + 1.) * heigth / 2.);

			DrawLineBetween(from, to, image, color);
		}
	}
}


