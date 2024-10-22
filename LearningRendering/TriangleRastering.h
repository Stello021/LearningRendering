#pragma once

#include "TGA-Decoder/TGAReader/TgaImage.h"
#include "TGA-Decoder/TGAReader/tgaData.h"
#include "OBJDecoder/OBJDecoder/VectorsMath.h"

typedef Vector2<int> ScreenPoint;

//A Good method of drawing a triangle must be:
//- Simple and Fast;
//- Symmetrical: Vertices are sorted, so rastering not depend on the order of vertices passed to the drawing function
//- Allows Intersections

/// <summary>
/// Scanline Rasterization algorithm
/// </summary>
void DrawFilledTriangle_MonoThread(ScreenPoint vertex1, ScreenPoint vertex2, ScreenPoint vertex3, TGAImage& image, TGAColor color);