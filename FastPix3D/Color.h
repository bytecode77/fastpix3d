struct FASTPIX3D_API Color
{
public:
	int32 R, G, B;

	Color();
	Color(int32 r, int32 g, int32 b);
	Color(int32 rgb);

	int32 getRGB();
};