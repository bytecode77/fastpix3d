# FastPix3D

## Software Rasterizer

FastPix3D is a **high-performance** software rasterizer written in C++.

With features akin to modern 3D engines - such as depth shadows, anisotropic mip mapping, cubemaps, and more - this project stands distinctly superior to other software rasterizers.

## Performance

The **block-based half-space** algorithm is aggressively optimized using **AVX2** and **multithreading**. Pixel shading is performed on 8 pixels at once using SIMD intrinsics.

C++ **template parameters** are used to implement **branchless** render states that would otherwise impose branches in the tight inner loops. As a result, render states add **zero overhead**.

## Screenshots

[![](/.github/screenshots/001.thumb.jpg)](/.github/screenshots/001.jpg)
[![](/.github/screenshots/002.thumb.jpg)](/.github/screenshots/002.jpg)
[![](/.github/screenshots/003.thumb.jpg)](/.github/screenshots/003.jpg)
[![](/.github/screenshots/004.thumb.jpg)](/.github/screenshots/004.jpg)
[![](/.github/screenshots/005.thumb.jpg)](/.github/screenshots/005.jpg)
[![](/.github/screenshots/006.thumb.jpg)](/.github/screenshots/006.jpg)
[![](/.github/screenshots/007.thumb.jpg)](/.github/screenshots/007.jpg)
[![](/.github/screenshots/008.thumb.jpg)](/.github/screenshots/008.jpg)

## Features

*Cool stuff you rarely see in software renderers — all in one place.*

### Depth shadows with PCF & cubemapping

FastPix3D renders **depth maps** from point light sources. **PCF** (percentage closer filtering) produces soft shadows.

[![](/.github/screenshots/pcf.jpg)](/.github/screenshots/pcf.jpg)

With **cubemap shadow map projection**, it is possible to implement true indoor shadows with full 360° coverage. This feature requires peak performance rasterization to be feasible.

### Anisotropic mip mapping & Texture filtering

**Anisotropic mip mapping** significantly reduces aliasing, at negligible performance cost. **Bilinear texture filtering**, combined with high frame rates, provides smooth and artifact-free visuals.

[![](/.github/screenshots/texture-filtering.jpg)](/.github/screenshots/texture-filtering.jpg)

## Full list of features

- Depth shadows
  - Point lights
  - Cubemap (6 shadow maps, full 360°)
  - PCF (percentage closer filtering)
- Textures
  - Anisotropic mip mapping
  - Bilinear filtering
  - Sphere mapping
  - Blend modes (alpha, multiply, add)
  - Per-pixel transparency key
- Vertex lighting with specular highlights
- Fog
- Wireframe
- Bitmap fonts
- Primitive generator (cube, sphere, cylinder, torus, etc.)
- Render to texture

## Performance optimizations

- Block-based half-space algorithm
  - Rendering 8 pixels at once using AVX2
  - Multithreading
  - Rasterization is almost fully branchless
- Branchless render states by leveraging C++ template parameters
- Perspective correction and mip mapping performed only once per 8x8 pixel block
- Optimizations from the architectural level down to micro-optimizations

## Downloads

All examples as compiled executables:

[![](http://bytecode77.com/public/fileicons/zip.png) FastPix3D 4.1.0.zip](/.github/downloads/FastPix3D%204.1.0.zip)