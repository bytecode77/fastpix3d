class FASTPIX3D_API Texture
{
private:
	string FileName;
	int32 Width, Height, WidthExponent;
	int32 *Buffer;

	static int32 GetLogarithmicCeiling(int32 number);
	static int32 GetExponent(int32 number);
public:
	Texture(string path);
	~Texture();

	int32 getWidth();
	int32 getHeight();
	int32* getBuffer();
	string getFileName();

	friend class Drawer;
};