class FASTPIX3D_API Drawer
{
private:
	static bool IsStencilTriangle;
	static bool IsLastTriangleCulled;
	static float Dz;
	static float Du;
	static float Dv;
	static float Dr;
	static float Dg;
	static float Db;
	static float Dz16;
	static float Du16;
	static float Dv16;
	static float Dr16;
	static float Dg16;
	static float Db16;

	static void Initialize();

	static void DrawClippedTriangle(ProcessedVertex vertex1, ProcessedVertex vertex2, ProcessedVertex vertex3);
	static void DrawScanline(int32 y, int32 v1x, float v1z, float v1u, float v1v, float v1r, float v1g, float v1b, int32 v2x, float v2z, float v2u, float v2v, float v2r, float v2g, float v2b);
	static void DrawStencilScanline(int32 y, int32 v1x, float v1z, int32 v2x, float v2z);
public:
	static void DrawTriangle(Matrix modelSpace, Vertex vertex1, Vertex vertex2, Vertex vertex3);
	static void DrawStencilTriangle(Matrix modelSpace, Vector3f vertex1Position, Vector3f vertex2Position, Vector3f vertex3Position);

	static bool getIsLastTriangleCulled();

	friend class Device;
};