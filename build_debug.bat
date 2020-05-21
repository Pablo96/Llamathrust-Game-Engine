:: Engine Executable
::cl /Z7 /EHsc /Od /Fo".\bin\obj\\" /DLT_DEBUG ^
::/I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\include" ^
::/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt" ^
::/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um" ^
::/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\shared" ^
::/I".\include" ^
::/I".\external" ^
::/I".\external\log-c" ^
::/I".\external\glad\include" ^
::/c ./external/glad/src/glad.c ./external/log-c/log.c ./src/Engine.c ./src/GraphicsAPI.c ./src/platform/Win32.c ^
::/link User32.lib Gdi32.lib OpenGL32.lib ^
::/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x64\" ^

:: Engine Executable
clang ^
-std=c17 ^
-DLT_DEBUG ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o "bin/engine_clang.exe" ^
-m64 ^
-lUser32.lib ^
-lGdi32.lib ^
-lOpengl32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/optimizations.yml" ^
-Weverything ^
-Wno-unused-parameter ^
-fshow-source-location ^
-Og ^
./external/glad/src/glad.c ./external/log-c/log.c ./src/*.c ./src/platform/*.c
