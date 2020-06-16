# LlamathrustC
Llamathrust GameEngine test in C18 using LLVM CLANG

## Build
- Clone the repository
- Execute the script in /{repoDir}/scripts/windows/clang_build_{debug/editor_release/release}.bat
- Run /repoDir/bin/{debug/editor_release/release}/*.exe

# IMPORTANT
If you decide to contribute to the project you need to document
the implementation and why you made some decisions in the code.
Right now all is documented (poorly) in this page:
https://pablonarvaja.wixsite.com/llamathrustengine

# Features
## Game
- OpenGL 3.3+

## Editor
- Same as game

## Supported Builds
- Windows 10 x64: WIP
- Linux 4+ x64: Not implemented
- Android 4.4+ x64: Not implemented

## Errors list
####| code | description
####| 0    | No errors
####| 1    | Couldn't create the window
####| 48   | Instance already running
####| 50   | Graphic API initialization
