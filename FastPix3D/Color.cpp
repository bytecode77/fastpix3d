#include "FastPix3D.h"

Color::Color()
{
	R = 0;
	G = 0;
	B = 0;
}
Color::Color(int32 r, int32 g, int32 b)
{
	R = r;
	G = g;
	B = b;
}
Color::Color(int32 rgb)
{
	R = (rgb & 0xff0000) >> 16;
	G = (rgb & 0xff00) >> 8;
	B = rgb & 0xff;
}

int32 Color::getRGB()
{
	return R << 16 | G << 8 | B;
}