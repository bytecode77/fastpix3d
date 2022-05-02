# FastPix3D

## Software Rasterizer

FastPix3D is a software rasterizer written in C++. Software rendering is based on software graphics processing without utilizing hardware acceleration, such as Direct3D or OpenGL. CPU rendering is slow, yet FastPix3D is performing well enough. It even supports vertex lights and stencil shadows at a quite acceptable frame rate. Many optimizations are implemented in order to achieve such performance.

[![](https://bytecode77.com/images/pages/fastpix3d/001.thumb.jpg)](https://bytecode77.com/images/pages/fastpix3d/001.jpg)
[![](https://bytecode77.com/images/pages/fastpix3d/002.thumb.jpg)](https://bytecode77.com/images/pages/fastpix3d/002.jpg)
[![](https://bytecode77.com/images/pages/fastpix3d/003.thumb.jpg)](https://bytecode77.com/images/pages/fastpix3d/003.jpg)
[![](https://bytecode77.com/images/pages/fastpix3d/004.thumb.jpg)](https://bytecode77.com/images/pages/fastpix3d/004.jpg)
[![](https://bytecode77.com/images/pages/fastpix3d/005.thumb.jpg)](https://bytecode77.com/images/pages/fastpix3d/005.jpg)
[![](https://bytecode77.com/images/pages/fastpix3d/006.thumb.jpg)](https://bytecode77.com/images/pages/fastpix3d/006.jpg)

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

[![](http://bytecode77.com/public/fileicons/zip.png) FastPix3D 3.1.1.zip](https://downloads.bytecode77.com/FastPix3D%203.1.1.zip)

## Project Page

[![](https://bytecode77.com/public/favicon16.png) bytecode77.com/fastpix3d](https://bytecode77.com/fastpix3d)