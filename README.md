# Llamathrust Game Engine (LtGE)
LtGE developed in C++17 using LLVM CLANG/GCC/VS2019.

## Motivation
LtGE is a personal project to learn how game engines work.

It aims to cover:
- Custom platform API abstraction (linux and windows)
- Multiple Grpahics API's (OpenGL and Vulkan)
- GUI library from scratch
- PBR rendering
- ECS based
- Binary files for assets (includes a cooker and editor)
- Plug-in based physics API
- Someday mobile support for Android

You can see tutorials and progress in facebook.com/LlamathrustGE

# IMPORTANT
If you decide to contribute to the project you need to document
the implementation and why you made some decisions in the code.
Right now all is documented (poorly) in this page:
https://pablonarvaja.wixsite.com/llamathrustengine

# Implemented Features
## Supported Builds
- Windows 10 x64: clang | vs 2019
- Linux 4+ x64: gcc
- Android 4.4+ x64: Not implemented

## Render API Engine
- OpenGL 3.3+
- Vulkan (WIP)


# Build Instructions
## Windows
- Clone the repository
- Install visual studio 2019
- Run the script ```./scripts/windows/vs_build_debug.bat```
- Run ```./bin/{debug/editor_release/release}/engine.exe```

## Linux
- Clone the repository
- Install gcc and libgl1-mesa-dev
- Run the script ```./scripts/linux/build_engine_debug.sh```
- Run ```./bin/llamathrust```
