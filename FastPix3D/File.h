class FASTPIX3D_API File
{
private:
	FILE *FilePtr;

	File(FILE *filePtr);
public:
	static bool Exists(string path);
	static void Delete(string path);
	static File OpenRead(string path);
	static File Create(string path);
	static File Open(string path);

	int64 getPosition();

	int32 Read(void *dest, int32 elementSize, int32 elementCount);
	sbyte ReadSByte();
	int16 ReadInt16();
	int32 ReadInt32();
	int64 ReadInt64();
	float ReadFloat();
	double ReadDouble();
	byte ReadByte();
	char ReadChar();
	uint16 ReadUInt16();
	uint32 ReadUInt32();
	uint64 ReadUInt64();
	string ReadString();
	string ReadLine();

	void Write(void *data, int32 elementSize, int32 elementCount);
	void WriteSByte(sbyte value);
	void WriteInt16(int16 value);
	void WriteInt32(int32 value);
	void WriteInt64(int64 value);
	void WriteFloat(float value);
	void WriteDouble(double value);
	void WriteByte(byte value);
	void WriteChar(char value);
	void WriteUInt16(uint16 value);
	void WriteUInt32(uint32 value);
	void WriteUInt64(uint64 value);
	void WriteString(string value);
	void WriteLine(string value);

	void Seek(int64 position, SeekOrigin origin);
	void Close();
};