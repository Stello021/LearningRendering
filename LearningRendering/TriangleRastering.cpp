#include "TriangleRastering.h"
#include "TGA-Decoder/TGAReader/TgaImage.h"
#include "TGA-Decoder/TGAReader/tgaData.h"
#include "OBJDecoder/OBJDecoder/VectorsMath.h"
#include <vector>

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

Vector3<float> Barycentric(Vector2<int>* vertices, Vector2<int> p)
{
	//Check BARYCENTRIC.md for details
	Vector3<float> CrossProduct =
		Vector3<float>(vertices[2].X - vertices[0].X, vertices[1].X - vertices[0].X, vertices[0].X - p.X).Cross(
			Vector3<float>(vertices[2].Y - vertices[0].Y, vertices[1].Y - vertices[0].Y, vertices[0].Y - p.Y));

	//Degenerate Triangle Check
	//if the z-component of cross product is close to 0, triangle is degenerate
	//It means that it's collapsed into a line or P is not inside a valid triangle
	if (std::abs(CrossProduct.Z) < 1)
	{
		return Vector3<float>(-1, 1, 1); //Invalid point
	}

	//CrossProduct.Z is the determinant of the transformation matrix formed by the triangle, used to normalize coordinates
	return Vector3<float>(1.f - (CrossProduct.X + CrossProduct.Y) / CrossProduct.Z, CrossProduct.Y / CrossProduct.Z, CrossProduct.X / CrossProduct.Z);
}

Vector3<float> Barycentric(Vector3<float>* vertices, Vector3<float> p)
{
	//Check BARYCENTRIC.md for details
	Vector3<float> CrossProduct =
		Vector3<float>(vertices[2].Y - vertices[0].Y, vertices[1].Y - vertices[0].Y, vertices[0].Y - p.Y).Cross(
			Vector3<float>(vertices[2].X - vertices[0].X, vertices[1].X - vertices[0].X, vertices[0].X - p.X));

	//Degenerate Triangle Check
	//if the z-component of cross product is close to 0, triangle is degenerate
	//It means that it's collapsed into a line or P is not inside a valid triangle
	if (std::abs(CrossProduct.Z) > 1e-2)
	{
		//CrossProduct.Z is the determinant of the transformation matrix formed by the triangle, used to normalize coordinates
		return Vector3<float>(1.f - (CrossProduct.X + CrossProduct.Y) / CrossProduct.Z, CrossProduct.Y / CrossProduct.Z, CrossProduct.X / CrossProduct.Z);
	}
	return Vector3<float>(-1, 1, 1); //Invalid point


}

void DrawFilledTriangle_MultiThread(Vector2<int>* vertices, TGAImage& image, TGAColor color)
{
	//Init Bounding Box 
	Vector2<int> b_boxMin(image.GetWidth() - 1, image.GetHeigth() - 1);
	Vector2<int> b_boxMax(0, 0);
	Vector2<int> clamp = b_boxMin; //clamping to image dimensions

	//Bounding Box Calculation 
	for (int i = 0; i < 3; i++)
	{
		b_boxMin.X = std::max(0, std::min(b_boxMin.X, vertices[i].X));
		b_boxMin.Y = std::max(0, std::min(b_boxMin.Y, vertices[i].Y));

		b_boxMax.X = std::min(clamp.X, std::max(b_boxMax.X, vertices[i].X));
		b_boxMax.Y = std::min(clamp.Y, std::max(b_boxMax.Y, vertices[i].Y));
	}

	//Rasterization
	Vector2<int> P;
	for (P.X = b_boxMin.X; P.X <= b_boxMax.X; P.X++)
	{
		for (P.Y = b_boxMin.Y; P.Y <= b_boxMax.Y; P.Y++)
		{
			Vector3<float> bc_screen = Barycentric(vertices, P);
			if (bc_screen.X < 0 || bc_screen.Y < 0 || bc_screen.Z < 0)
			{
				continue;
			}

			image.SetColor(P.X, P.Y, color);
		}
	}


}

void DrawTrianglesFromModel(Model*& model, TGAImage& image, const int width, const int height)
{
	//Loop through faces of the model
	for (int i = 0; i < model->FacesAmount(); i++)
	{
		std::vector<int> face = model->Face(i);

		//store the 2D screen coordinates for each vertices of the face
		Vector2<int> screenCoords[3];

		//iterate for each vertices of the face
		for (int j = 0; j < 3; j++)
		{
			//retrives the 3D world coordinate of the current vertex of the face
			Vector3<float> worldCoords = model->Vertex(face[j]);

			//world coordinates are converted in screen coordinates
			//- adding +1 to coordinates to normalizes them (shift range from [-1, 1] to [0, 2]
			//- scales the coordinates by the screen dimensions, mapping them to the screen coordinates
			screenCoords[j] = Vector2<int>((worldCoords.X + 1.) * width / 2., (worldCoords.Y + 1.) * height / 2.);
		}
		DrawFilledTriangle_MultiThread(screenCoords, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
}

void DrawFlatShadedModel(Model*& model, TGAImage& image, const int width, const int height)
{
	Vector3<float> lightDirection(0, 0, -1); //define the direction of the light source, pointing towards the viewer along the z-axis

	for (int i = 0; i < model->FacesAmount(); i++)
	{
		std::vector<int> face = model->Face(i);

		Vector2<int> screenCoords[3];
		Vector3<float> worldCoords[3];
		for (int j = 0; j < 3; j++) {
			Vector3<float> v = model->Vertex(face[j]);
			screenCoords[j] = Vector2<int>((v.X + 1.) * width / 2., (v.Y + 1.) * height / 2.);
			worldCoords[j] = v;
		}

		//Cross product between two edges of the triangle in 3D Space to calculate normal vector
		Vector3<float> normal = (worldCoords[2] - worldCoords[0]).Cross(worldCoords[1] - worldCoords[0]);
		normal.Normalize();

		float intensity = normal.Dot(lightDirection); //dot product measures how aligned the surface is with the light
		// close to 1 -> surface is directly facing the light (max brightness)
		// close to 0 -> surface is perpendicular to the light
		// negative -> surface is facing away from the light

		if (intensity > 0) //Back-face culling 
		{
			DrawFilledTriangle_MultiThread(screenCoords, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}

	}
}

void DrawDeepShadedTriangle(Vector3<float>* vertices, float* zBuffer, TGAImage& image, TGAColor color)
{
	Vector2<float> b_boxMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector2<float> b_boxMax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

	Vector2<float> clamp(image.GetWidth() - 1, image.GetHeigth() - 1);

	for (int i = 0; i < 3; i++) //iterate for each vertex
	{
		for (int j = 0; j < 2; j++) //iterate for each vertex's coordinate
		{
			b_boxMin.Raw[j] = std::max(0.f, std::min(b_boxMin.Raw[j], vertices[i].Raw[j]));
			b_boxMax.Raw[j] = std::min(clamp.Raw[j], std::max(b_boxMax.Raw[j], vertices[i].Raw[j]));
		}
	}

	Vector3<float> P;

	for (P.X = b_boxMin.X; P.X <= b_boxMax.X; P.X++)
	{
		for (P.Y = b_boxMin.Y; P.Y <= b_boxMax.Y; P.Y++)
		{
			Vector3<float> bc = Barycentric(vertices, P);
			if (bc.X < 0 || bc.Y < 0 || bc.Z < 0)
			{
				continue;
			}

			P.Z = 0;

			for (int i = 0; i < 3; i++)
			{
				P.Z += vertices[i].Raw[2] * bc.Raw[i]; //P.Z is calculated as the depth of point P within the triangle,
				//using the z-coordinates of each vertex weighted by their respective barycentric
			}
			if (zBuffer[static_cast<int>(P.X + P.Y * image.GetWidth())] < P.Z) //P is more distant to the viewer than any previously drawn point at this pixel
			{
				zBuffer[static_cast<int>(P.X + P.Y * image.GetWidth())] = P.Z;
				image.SetColor(P.X, P.Y, color);
			}
		}
	}
}

void DrawDeepShadedModel(Model*& model, TGAImage& image, const int width, const int heigth)
{
	Vector3<float> lightDirection(0, 0, -1); //define the direction of the light source, pointing towards the viewer along the z-axis

	float* zBuffer = new float[image.GetWidth() * image.GetHeigth()];
	for (int i = image.GetWidth() * image.GetHeigth(); i--; zBuffer[i] = -std::numeric_limits<float>::max());

	for (int i = 0; i < model->FacesAmount(); i++)
	{
		std::vector<int> face = model->Face(i);

		Vector3<float> screenCoords[3];
		Vector3<float> worldCoords[3];
		for (int j = 0; j < 3; j++) {
			Vector3<float> v = model->Vertex(face[j]);
			screenCoords[j] = Vector3<float>(static_cast<int>((v.X + 1.) * width / 2. + .5), static_cast<int>((v.Y + 1.) * heigth / 2. + .5), static_cast<int>(v.Z));
			worldCoords[j] = v;
		}

		//Cross product between two edges of the triangle in 3D Space to calculate normal vector
		Vector3<float> normal = (worldCoords[2] - worldCoords[0]).Cross(worldCoords[1] - worldCoords[0]);
		normal.Normalize();

		float intensity = normal.Dot(lightDirection); //dot product measures how aligned the surface is with the light
		// close to 1 -> surface is directly facing the light (max brightness)
		// close to 0 -> surface is perpendicular to the light
		// negative -> surface is facing away from the light

		if (intensity > 0) //Back-face culling 
		{
			DrawDeepShadedTriangle(screenCoords, zBuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}
}
