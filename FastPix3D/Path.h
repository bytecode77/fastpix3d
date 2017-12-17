class FASTPIX3D_API Path
{
public:
	static string GetWorkingDirectory();
	static string GetStartupPath();
	static string GetExecutablePath();

	static string Combine(string path1, string path2);
	static string GetFileName(string path);
	static string GetDirectoryName(string path);
	static string GetExtension(string path);
	static bool HasExtension(string path);
	static string GetFileNameWithoutExtension(string path);
};