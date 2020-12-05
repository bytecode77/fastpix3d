class EXAMPLELIBRARY_API Examples
{
public:
	static const int32 DefaultWidth;
	static const int32 DefaultHeight;
	static const int32 DefaultRectangularResolution;

	static void DrawRenderingInformation(Font *font, FPSCounter *fpsCounter, int triangleCount, bool fillBackGround);
	static void DrawInputHint(ExampleType exampleType, Font *font);
	static void DrawPosition(Font *font, Vector3f position);
};