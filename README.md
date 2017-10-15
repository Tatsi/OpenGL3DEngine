# OpenGL 3D Engine prototype

This is a simple 3D engine for Windows that uses WIN32 API to create a window, rendering context and handle
user input. It uses OpenGL 3.3 API to render 3D models and also includes simple collision detection.

I did this program in learning purposes. At first I was using GLUT to do some graphics programming, but soon found 
it very limited and decided to build my own engine as I found great intrest in computer graphics and knew that this would 
be a very educational project. It is programmed in C++ and for development I used Visual studio IDE. I use GLSL
for shaders and they are read from my shader files from `Shaders` directory.

The engine uses a custom model format which is a slightly modified version of common `ply` format. The models can be done 
and textured in Autodesk 3DS Max and then exported to the used format with a customized script.

## Technologies

Technologies and libraries used in this project:

* C++
* OpenGL 3.3 API
* GLSL 3.3 (in shaders)
* WIN32 API
* GLEW
* GLM math library
* 3D Studio Max (for modeling, texturing and exporting to my format)
* Visual Studio IDE

## TODO tasks

* Fix all warnings
* Move to `GLM` instead of simple `vec3` struct in all math calculations
* Add light position to renderer and pass it to vertex shader in eye coordinates
* Fix normal loading from file. Now normals are always recalculated. Bad for large models.
* Load some other models
* Re-do UI (text log) with modern pipeline
* Implement basic animation