#pragma once

#include "TGA-Decoder/TGAReader/TgaImage.h"
#include "TGA-Decoder/TGAReader/tgaData.h"
#include "OBJDecoder/OBJDecoder/VectorsMath.h"
#include "OBJDecoder/OBJDecoder/Model.h"

typedef Vector2<int> ScreenPoint;

/// <summary>
/// Bresenham's line algorithm
/// </summary>
void DrawLineBetween(ScreenPoint from, ScreenPoint to, TGAImage& image, TGAColor color);

/// <summary>
/// Draw a triangle based on Bresenham implementation
/// </summary>
void DrawTriangle(ScreenPoint vertex1, ScreenPoint vertex2, ScreenPoint vertex3, TGAImage& image, TGAColor color);


/// <summary>
/// Render the wireframe of a 3D Model onto a 2D image
/// </summary>
void DrawWireframeModel(TGAImage& image, Model*& model, const int width, const int heigth, TGAColor color);
