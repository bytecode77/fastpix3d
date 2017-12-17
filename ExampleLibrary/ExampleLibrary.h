#ifdef EXAMPLELIBRARY_EXPORTS
#define EXAMPLELIBRARY_API __declspec(dllexport)
#else
#define EXAMPLELIBRARY_API __declspec(dllimport)
#endif

#pragma comment(lib, "FastPix3D.lib")

#include <FastPix3D.h>

#include "ExampleType.h"
#include "Examples.h"