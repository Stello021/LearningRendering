#include "TGA-Decoder/TGAReader/TgaImage.h"
#include "LineRastering.h"
#include "TriangleRastering.h"
#include "OBJDecoder/OBJDecoder/Model.h"

const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);


const int width = 800;
const int height = 800;


int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
	//Model* model = new Model("suzanne.obj");
	//DrawWireframeModel(image, model, width, height, white);
	DrawFilledTriangle_MonoThread(ScreenPoint(10, 70), ScreenPoint(50, 160), ScreenPoint(70, 80), image, red);
	DrawFilledTriangle_MonoThread(ScreenPoint(180, 50), ScreenPoint(150, 1), ScreenPoint(70, 180), image, white);
	image.FlipVertically(); // i want to have the origin at the left bottom corner of the image
	image.WriteTGAFile("example.tga");
	//delete model;
	return 0;
}