#include "Examples/PrimitivesExample.h"
#include "Examples/MeshViewExample.h"
#include "Examples/FreeLookExample.h"
#include "Examples/ShadowMapExample.h"
#include "Examples/HighPolyBenchmarkExample.h"
#include "Examples/SphereMapExample.h"

int main(int argc, char *argv[])
{
	int32 width = 1280;
	int32 height = 720;
	Example *example;

	switch (3)
	{
		case 1: example = new PrimitivesExample(width, height); break;
		case 2: example = new MeshViewExample(width, height); break;
		case 3: example = new FreeLookExample(width, height); break;
		case 4: example = new ShadowMapExample(width, height); break;
		case 5: example = new HighPolyBenchmarkExample(width, height); break;
		case 6: example = new SphereMapExample(width, height); break;
		default: throw;
	}

	example->Run();
	delete example;

	return 0;
}



//TODO:
//   * Proper scanline convention
//   * Wireframe
//   * Vertical mip mapping
//   * Dot3 mapping
//   * Displacement mapping
//   * SSAO
//   * Screen Space Refraction
//   * Shadow Mapping PCF
// Performance:
//   * Mipmapping in shadow map projection
//   * "bool perspectiveCorrection" template parameter
//   * Fixed point