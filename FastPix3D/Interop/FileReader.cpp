#include "FileReader.h"

FileReader::FileReader(const char *path)
{
	Handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (Handle == INVALID_HANDLE_VALUE) throw;
}
FileReader::~FileReader()
{
	CloseHandle(Handle);
}

sbyte FileReader::ReadSByte() const
{
	sbyte value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(sbyte), &bytesRead, NULL) || bytesRead != sizeof(sbyte)) throw;

	return value;
}
int16 FileReader::ReadInt16() const
{
	int16 value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(int16), &bytesRead, NULL) || bytesRead != sizeof(int16)) throw;

	return value;
}
int32 FileReader::ReadInt32() const
{
	int32 value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(int32), &bytesRead, NULL) || bytesRead != sizeof(int32)) throw;

	return value;
}
int64 FileReader::ReadInt64() const
{
	int64 value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(int64), &bytesRead, NULL) || bytesRead != sizeof(int64)) throw;

	return value;
}
byte FileReader::ReadByte() const
{
	byte value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(byte), &bytesRead, NULL) || bytesRead != sizeof(byte)) throw;

	return value;
}
uint16 FileReader::ReadUInt16() const
{
	uint16 value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(uint16), &bytesRead, NULL) || bytesRead != sizeof(uint16)) throw;

	return value;
}
uint32 FileReader::ReadUInt32() const
{
	uint32 value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(uint32), &bytesRead, NULL) || bytesRead != sizeof(uint32)) throw;

	return value;
}
uint64 FileReader::ReadUInt64() const
{
	uint64 value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(uint64), &bytesRead, NULL) || bytesRead != sizeof(uint64)) throw;

	return value;
}
float FileReader::ReadFloat() const
{
	float value;
	DWORD bytesRead;
	if (!ReadFile(Handle, &value, sizeof(float), &bytesRead, NULL) || bytesRead != sizeof(float)) throw;

	return value;
}
void FileReader::ReadString(char *buffer, int32 bufferLength) const
{
	for (DWORD bytesRead; bufferLength-- > 0 && ReadFile(Handle, buffer, 1, &bytesRead, NULL); buffer++)
	{
		if (bytesRead == 0 || *buffer == '\0')
		{
			break;
		}
	}
}
void FileReader::ReadLine(char *buffer, int32 bufferLength) const
{
	DWORD bytesRead;
	for (char *ptr = buffer; bufferLength-- > 0 && ReadFile(Handle, ptr, 1, &bytesRead, NULL); ptr++)
	{
		if (bytesRead == 0 || *ptr == '\n')
		{
			*ptr = '\0';
			break;
		}
	}

	int32 length = lstrlenA(buffer);
	if (buffer[length - 1] == '\r')
	{
		buffer[length - 1] = '\0';
	}
}