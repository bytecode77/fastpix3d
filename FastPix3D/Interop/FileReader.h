#pragma once
#include "../FastPix3D.h"

class FASTPIX3D_API FileReader
{
private:
	HANDLE Handle;

public:
	explicit FileReader(const char *path);
	~FileReader();

	sbyte ReadSByte() const;
	int16 ReadInt16() const;
	int32 ReadInt32() const;
	int64 ReadInt64() const;
	byte ReadByte() const;
	uint16 ReadUInt16() const;
	uint32 ReadUInt32() const;
	uint64 ReadUInt64() const;
	float ReadFloat() const;
	void ReadString(char *buffer, int32 bufferLength) const;
	void ReadLine(char *buffer, int32 bufferLength) const;
};