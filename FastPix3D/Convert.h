class FASTPIX3D_API Convert
{
public:
	static sbyte ToSByte(string value);
	static int16 ToInt16(string value);
	static int32 ToInt32(string value);
	static int64 ToInt64(string value);
	static byte ToByte(string value);
	static float ToFloat(string value);
	static double ToDouble(string value);

	static string ToString(sbyte value);
	static string ToString(int16 value);
	static string ToString(int32 value);
	static string ToString(int64 value);
	static string ToString(byte value);
	static string ToString(uint16 value);
	static string ToString(uint32 value);
	static string ToString(uint64 value);
	static string ToString(wstring value);
};