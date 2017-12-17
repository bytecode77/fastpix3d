#include "FastPix3D.h"

File::File(FILE *filePtr)
{
	FilePtr = filePtr;
}

bool File::Exists(string path)
{
	int32 attr = GetFileAttributes(path.c_str());
	return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}
void File::Delete(string path)
{
	if (Exists(path) && !DeleteFile(path.c_str())) throw;
}
File File::OpenRead(string path)
{
	FILE *filePtr;
	if (fopen_s(&filePtr, path.c_str(), "rb") != 0) throw;
	return File(filePtr);
}
File File::Create(string path)
{
	FILE *filePtr;
	if (fopen_s(&filePtr, path.c_str(), "wb") != 0) throw;
	if (filePtr == NULL) throw;
	return File(filePtr);
}
File File::Open(string path)
{
	FILE *filePtr;
	if (fopen_s(&filePtr, path.c_str(), "rb+") != 0) throw;
	return File(filePtr);
}

int64 File::getPosition()
{
	return _ftelli64(FilePtr);
}

int32 File::Read(void *dest, int32 elementSize, int32 elementCount)
{
	return int32(fread(dest, elementSize, elementCount, FilePtr));
}
sbyte File::ReadSByte()
{
	sbyte ret = fgetc(FilePtr);
	if (ret == -1) throw;
	return ret;
}
int16 File::ReadInt16()
{
	int16 ret;
	if (Read(&ret, 2, 1) != 1) throw;
	return ret;
}
int32 File::ReadInt32()
{
	int32 ret;
	if (Read(&ret, 4, 1) != 1) throw;
	return ret;
}
int64 File::ReadInt64()
{
	int64 ret;
	if (Read(&ret, 8, 1) != 1) throw;
	return ret;
}
float File::ReadFloat()
{
	float ret;
	if (Read(&ret, 4, 1) != 1) throw;
	return ret;
}
double File::ReadDouble()
{
	double ret;
	if (Read(&ret, 8, 1) != 1) throw;
	return ret;
}
byte File::ReadByte()
{
	byte ret = fgetc(FilePtr);
	if (ret == -1) throw;
	return ret;
}
char File::ReadChar()
{
	char ret = fgetc(FilePtr);
	if (ret == -1) throw;
	return ret;
}
uint16 File::ReadUInt16()
{
	uint16 ret;
	if (Read(&ret, 2, 1) != 1) throw;
	return ret;
}
uint32 File::ReadUInt32()
{
	uint32 ret;
	if (Read(&ret, 4, 1) != 1) throw;
	return ret;
}
uint64 File::ReadUInt64()
{
	uint64 ret;
	if (Read(&ret, 8, 1) != 1) throw;
	return ret;
}
string File::ReadString()
{
	char ch;
	string ret = "";
	while ((ch = fgetc(FilePtr)) > -1)
	{
		if (ch == '\0') break;
		ret += ch;
	}
	return ret;
}
string File::ReadLine()
{
	char *lineChars = new char[4096];
	char *linePtr = fgets(lineChars, 4096, FilePtr);
	if (linePtr == NULL)
	{
		delete lineChars, linePtr;
		throw;
	}
	string line = string(linePtr);
	delete lineChars, linePtr;

	int32 length = int32(line.length());
	if (length > 0 && line[length - 1] == '\n')
	{
		line = line.substr(0, length - 1);
		length = int32(line.length());
		if (length > 0 && line[length - 1] == '\r') line = line.substr(0, length - 1);
	}

	return line;
}

void File::Write(void *data, int32 elementSize, int32 elementCount)
{
	fwrite(data, elementSize, elementCount, FilePtr);
}
void File::WriteSByte(sbyte value)
{
	if (fputc(value, FilePtr) == -1) throw;
}
void File::WriteInt16(int16 value)
{
	if (fwrite(&value, 2, 1, FilePtr) != 1) throw;
}
void File::WriteInt32(int32 value)
{
	if (fwrite(&value, 4, 1, FilePtr) != 1) throw;
}
void File::WriteInt64(int64 value)
{
	if (fwrite(&value, 8, 1, FilePtr) != 1) throw;
}
void File::WriteFloat(float value)
{
	if (fwrite(&value, 4, 1, FilePtr) != 1) throw;
}
void File::WriteDouble(double value)
{
	if (fwrite(&value, 8, 1, FilePtr) != 1) throw;
}
void File::WriteByte(byte value)
{
	if (fputc(value, FilePtr) == -1) throw;
}
void File::WriteChar(char value)
{
	if (fputc(value, FilePtr) == -1) throw;
}
void File::WriteUInt16(uint16 value)
{
	if (fwrite(&value, 2, 1, FilePtr) != 1) throw;
}
void File::WriteUInt32(uint32 value)
{
	if (fwrite(&value, 4, 1, FilePtr) != 1) throw;
}
void File::WriteUInt64(uint64 value)
{
	if (fwrite(&value, 8, 1, FilePtr) != 1) throw;
}
void File::WriteString(string value)
{
	if (fputs(value.c_str(), FilePtr) < 0) throw;
	WriteChar('\0');
}
void File::WriteLine(string value)
{
	WriteString(value);
	WriteChar('\n');
}

void File::Seek(int64 position, SeekOrigin origin)
{
	_fseeki64(FilePtr, position, int32(origin));
}
void File::Close()
{
	fclose(FilePtr);
}