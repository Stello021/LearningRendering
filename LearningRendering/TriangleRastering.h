#pragma once

#include "TGA-Decoder/TGAReader/TgaImage.h"
#include "TGA-Decoder/TGAReader/tgaData.h"
#include "OBJDecoder/OBJDecoder/VectorsMath.h"
#include "OBJDecoder/OBJDecoder/Model.h"

typedef Vector2<int> ScreenPoint;

//A Good method of drawing a triangle must be:
//- Simple and Fast;
//- Symmetrical: Vertices are sorted, so rastering not depend on the order of vertices passed to the drawing function
//- Allows Intersections

/// <summary>
/// Scanline Rasterization algorithm
/// </summary>
void DrawFilledTriangle_MonoThread(ScreenPoint vertex1, ScreenPoint vertex2, ScreenPoint vertex3, TGAImage& image, TGAColor color);

/// <summary>
/// Calculates barycentric coordinates for a point P with respect to vertices of a triangle 
/// </summary>
Vector3<float> Barycentric(Vector2<int>* vertices, Vector2<int> p);

/// <summary>
/// Bounding Box Rasterization Algorithm
/// </summary>
void DrawFilledTriangle_MultiThread(Vector2<int>* vertices, TGAImage& image, TGAColor color);

/// <summary>
/// Render 3D model's faces as 2D triangles on a screen using random colors for each triangle
/// </summary>
void DrawTrianglesFromModel(Model*& model, TGAImage& image, const int width, const int heigth);

/// <summary>
/// Renders 3D model by lighting its faces based on the angle of the light source
/// </summary>
void DrawFlatShadedModel(Model*& model, TGAImage& image, const int width, const int heigth);