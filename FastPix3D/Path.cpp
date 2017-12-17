#include "FastPix3D.h"

string Path::GetWorkingDirectory()
{
	char *pathChars = new char[4096];
	if (_getcwd(pathChars, 4096) == NULL)
	{
		delete pathChars;
		throw;
	}
	string path = string(pathChars);
	delete pathChars;
	return path;
}
string Path::GetStartupPath()
{
	return GetDirectoryName(GetExecutablePath());
}
string Path::GetExecutablePath()
{
	char *pathChars = new char[4096];
	if (!GetModuleFileName(NULL, pathChars, 4096))
	{
		delete pathChars;
		throw;
	}
	string path = string(pathChars);
	delete pathChars;
	return path;
}

string Path::Combine(string path1, string path2)
{
	if (path1.length() > 0 && path1[path1.length() - 1] != '\\') path1 += '\\';
	if (path2[0] == '\\') path2 = path2.substr(1);
	return path1 + path2;
}
string Path::GetFileName(string path)
{
	int32 pos = int32(path.length()) - 1;
	while (pos >= 0 && path[pos] != '\\') pos--;
	return path.substr(pos + 1);
}
string Path::GetDirectoryName(string path)
{
	int32 pos = int32(path.length()) - 1;
	while (pos >= 0 && path[pos] != '\\') pos--;
	return path.substr(0, pos == -1 ? 0 : pos);
}
string Path::GetExtension(string path)
{
	string fileName = GetFileName(path);
	int32 pos = int32(fileName.length()) - 1;
	while (pos >= 0 && fileName[pos] != '.') pos--;
	return pos == -1 ? "" : fileName.substr(pos + 1);
}
bool Path::HasExtension(string path)
{
	return GetExtension(path) != "";
}
string Path::GetFileNameWithoutExtension(string path)
{
	string fileName = GetFileName(path);
	int32 pos = int32(fileName.length()) - 1;
	while (pos >= 0 && fileName[pos] != '.') pos--;
	return pos == -1 ? fileName : fileName.substr(0, pos);
}