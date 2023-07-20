# FastPix3D

## Software Rasterizer

FastPix3D is a software rasterizer written in C++. Software rendering is based on software graphics processing without utilizing hardware acceleration, such as Direct3D or OpenGL. CPU rendering is slow, yet FastPix3D is performing well enough. It even supports vertex lights and stencil shadows at a quite acceptable frame rate. Many optimizations are implemented in order to achieve such performance.

[![](/.github/screenshots/001.thumb.jpg)](/.github/screenshots/001.jpg)
[![](/.github/screenshots/002.thumb.jpg)](/.github/screenshots/002.jpg)
[![](/.github/screenshots/003.thumb.jpg)](/.github/screenshots/003.jpg)
[![](/.github/screenshots/004.thumb.jpg)](/.github/screenshots/004.jpg)
[![](/.github/screenshots/005.thumb.jpg)](/.github/screenshots/005.jpg)
[![](/.github/screenshots/006.thumb.jpg)](/.github/screenshots/006.jpg)

In times of Direct3D and OpenGL, writing a software rasterizer is a mathematical challenge rather than a graphics engine for actual game development. You will not achieve high quality graphics with acceptable performance using software rendering. But you will definitely get a better understanding for polygon-based 3D graphics when writing one yourself.

## Features

- Textured & colored polygons
- Z-buffering
- Vertex lights
- Stencil buffer, shadow volume creation & stencil shadows using Z-Fail algorithm
- Meshes
- Highly optimized, native C++ code
- HUD & bitmap fonts

## Downloads

[![](http://bytecode77.com/public/fileicons/zip.png) FastPix3D 3.1.1.zip](/.github/downloads/FastPix3D%203.1.1.zip)