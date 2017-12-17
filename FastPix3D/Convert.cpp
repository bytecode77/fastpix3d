#include "FastPix3D.h"

sbyte Convert::ToSByte(string value)
{
	return sbyte(atol(value.c_str()));
}
int16 Convert::ToInt16(string value)
{
	return int16(atol(value.c_str()));
}
int32 Convert::ToInt32(string value)
{
	return atol(value.c_str());
}
int64 Convert::ToInt64(string value)
{
	return _atoi64((value.c_str()));
}
byte Convert::ToByte(string value)
{
	return byte(atol(value.c_str()));
}
float Convert::ToFloat(string value)
{
	return float(atof(value.c_str()));
}
double Convert::ToDouble(string value)
{
	return atof(value.c_str());
}

string Convert::ToString(sbyte value)
{
	return ToString(int64(value));
}
string Convert::ToString(int16 value)
{
	return ToString(int64(value));
}
string Convert::ToString(int32 value)
{
	return ToString(int64(value));
}
string Convert::ToString(int64 value)
{
	char *str = new char[32];
	_i64toa_s(value, str, 32, 10);
	string ret = string(str);
	delete str;
	return ret;
}
string Convert::ToString(byte value)
{
	return ToString(int64(value));
}
string Convert::ToString(uint16 value)
{
	return ToString(uint64(value));
}
string Convert::ToString(uint32 value)
{
	return ToString(uint64(value));
}
string Convert::ToString(uint64 value)
{
	char *str = new char[32];
	_ui64toa_s(value, str, 32, 10);
	string ret = string(str);
	delete str;
	return ret;
}
string Convert::ToString(wstring value)
{
	return string(value.begin(), value.end());
}