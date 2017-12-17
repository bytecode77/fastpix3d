#ifdef FASTPIX3D_EXPORTS
#define FASTPIX3D_API __declspec(dllexport)
#else
#define FASTPIX3D_API __declspec(dllimport)
#endif

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(linker, "/subsystem:windows")

typedef signed __int8 sbyte;
typedef signed __int16 int16;
typedef signed __int32 int32;
typedef signed __int64 int64;
typedef unsigned __int8 byte;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <ctime>
#include <direct.h>
#include <Windows.h>
#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
using namespace std;

const float DegToRad = float(M_PI / 180);
const int32 FixedDepthExponent = 16, FixedDepth1 = 1 << FixedDepthExponent;
const int32 SubdivExponent = 4, Subdiv1 = 1 << SubdivExponent, SubdivModulo = Subdiv1 - 1;
const float InvertedSubdiv1 = 1.f / Subdiv1;
const int32 TextureTransparencyKey = 0xff00ff;

#include "SeekOrigin.h"
#include "CullMode.h"
#include "MouseButton.h"
#include "MathUtility.h"
#include "Convert.h"
#include "Directory.h"
#include "File.h"
#include "Path.h"
#include "FPSCounter.h"
#include "SDLUtility.h"
#include "Vector3f.h"
#include "Vector2f.h"
#include "Point.h"
#include "Matrix.h"
#include "Color.h"
#include "LightType.h"
#include "Light.h"
#include "FreelookManager.h"
#include "Vertex.h"
#include "ProcessedVertex.h"
#include "Triangle.h"
#include "Texture.h"
#include "RenderStates.h"
#include "Subset.h"
#include "Drawer.h"
#include "ShadowTriangle.h"
#include "ShadowVolume.h"
#include "Mesh.h"
#include "Input.h"
#include "Font.h"
#include "Graphics.h"
#include "Device.h"