class FASTPIX3D_API Font
{
private:
	int32 Width, Height, CharacterSpacing;
	byte *Buffer;
	Point *CharacterDimensions;
public:
	Font(string path);
	~Font();

	int32 getCharacterSpacing();

	void setCharacterSpacing(int32 characterSpacing);

	friend class Graphics;
};