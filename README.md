## AmberEngine
![AmberEngivev1 0 0](https://github.com/user-attachments/assets/c0ee49e3-aed2-4a64-bfdb-3e1053721bf0)
*WIP AmberEngine v1.0.0 OverloadEngine features integration 80% + QOL Features*

![EditorImproved](https://github.com/maxbrundev/AmberEngine/assets/32653095/bbeba826-a6e3-4593-92f6-8ddb4a934851)
*WIP Asset Browser, Material Editor, QOL Features (Actor Create menu directly from the Hierarchy Panel, Layouts management, Auto generate and apply Materials from loaded models)*

![AmberEngineDemoscene](https://github.com/maxbrundev/AmberEngine/assets/32653095/59c19e3e-2b0e-4cdb-92fa-0024cf54c113)
*Scene Name + Creation menu directly from Hierarchy Panel*

## Description
Personal Game Engine Project based on Overload Engine aiming a simpler architecture Editor UI side and providing QOL Features and Global Improvements.   
AmberEngine will provide Lua scripting like Overload does but also C++ programming:
- Behaviors -> Composed by a Start and Update methods called by the Engine. The goal is to mimic the Lua Behaviors scripts in order to provide a C++ alternative for the users
- CustomComponents -> Data Containers.
- Systems -> Instantiated and Released by the Engine, Systems will query related CustomComponents define the actors behaviors based on the data

## Software
- Visual Studio 2022

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

## Compiling sources
Premake5 is used to generate project files.
To generate the project, execute `GenerateProject.bat`. By default, `GenerateProject.bat` will generate project files for Visual Studio 2022. If you want to use another version of Visual Studio you can execute `GenerateProject.bat` from the command line with the Visual Studio version as argument. (ex: `.\GeneratedProject.bat vs2019`)

## Licence
AmberEngine is licenced under an MIT Licence.
