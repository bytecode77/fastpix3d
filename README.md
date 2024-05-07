# FastPix3D

## Software Rasterizer

FastPix3D is a high performance software rasterizer written in C++ with a wide variety of features.

[![](/.github/screenshots/001.thumb.jpg)](/.github/screenshots/001.jpg)
[![](/.github/screenshots/002.thumb.jpg)](/.github/screenshots/002.jpg)
[![](/.github/screenshots/003.thumb.jpg)](/.github/screenshots/003.jpg)
[![](/.github/screenshots/004.thumb.jpg)](/.github/screenshots/004.jpg)
[![](/.github/screenshots/005.thumb.jpg)](/.github/screenshots/005.jpg)
[![](/.github/screenshots/006.thumb.jpg)](/.github/screenshots/006.jpg)

## Features

- Depth shadows (point lights)
- Mip mapping
- Sphere mapping
- Texture blend modes
- Vertex lighting
- Fog
- Bitmap fonts

## Performance

- Branchless render states by leveraging C++ template parameters
- Multithreaded scanline rasterization
- SIMD optimized
- Rendering of scene and shadow map in parallel
- Perspective correction only once per 16 pixels

## Potential for improvement

- Replace the scanline algorithm with something more parallelizable
- Fixed point maths
- More visual effects, such as bump mapping, refraction, or SSAO

## Downloads

[![](http://bytecode77.com/public/fileicons/zip.png) FastPix3D 4.0.0.zip](/.github/downloads/FastPix3D%204.0.0.zip)