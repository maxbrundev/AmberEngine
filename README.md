## AmberEngine
![EditorImproved](https://github.com/maxbrundev/AmberEngine/assets/32653095/bbeba826-a6e3-4593-92f6-8ddb4a934851)
*WIP Asset Browser, Material Editor, QOL Features (Actor Create menu directly from the Hierarchy Panel, Layouts management, Auto generate and apply Materials from loaded models)*

![AmberEngineDemoscene](https://github.com/maxbrundev/AmberEngine/assets/32653095/08987cc6-7ad1-4b29-bf67-439ceab87946)
*Scene Name + Creation menu directly from Hierarchy Panel*

## Description
Personal Game Engine learning Project based on Overload Engine aiming a simpler architecture Editor UI side and providing QOL Features and Global Improvements.   
AmberEngine will provide Lua scripting like Overload does but also C++ programming:
- Behaviors -> Composed by a Start and Update methods called by the Engine. The goal is to mimic the Lua Behaviors scripts in order to provide a C++ alternative for the users
- CustomComponents -> Data Containers.
- Systems -> Instantiated and Released by the Engine, Systems will query related CustomComponents define the actors behaviors based on the data

## Software
- Visual Studio 2017 (Premake5 soon)

## Dependencies
- OpenGL with GLEW (Graphics API)
- GLFW (Windowing and inputs)
- Assimp (3D model loader)
- GLM (OpenGL Mathematics)
- stb_image (Image Loader)
- ImGui (GUI)

## Requirements
AmberEngine requires:
- GPU: Graphics card supporting OpenGL 4.3

## Licence
AmberEngine is licenced under an MIT Licence.
