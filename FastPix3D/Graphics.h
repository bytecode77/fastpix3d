class FASTPIX3D_API Graphics
{
public:
	static void FillRectangle(int32 x, int32 y, int32 width, int32 height, Color color);
	static void FillRectangle(int32 x, int32 y, int32 width, int32 height, Color color, float opacity);
	static void DrawString(int32 x, int32 y, string text, Font *font);
	static void DrawString(int32 x, int32 y, string text, Font *font, Color color);
};