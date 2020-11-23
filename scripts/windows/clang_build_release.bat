cd ../..
if not exist ".\bin\release" mkdir ".\bin\release"
cls

:: Engine Executable
clang ^
-std=c++17 ^
-D_CRT_SECURE_NO_WARNINGS ^
-DLT_RELEASE ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o"bin/release/engine_clang.exe" ^
-m64 ^
-lOpengl32.lib ^
-lGdi32.lib ^
-lUser32.lib ^
-lKernel32.lib ^
-lwsock32.lib ^
-lws2_32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/release/optimizations.yml" ^
-static-libgcc ^
-ggdb ^
-Weverything ^
-Wno-c++98-compat-pedantic ^
-Wno-microsoft-redeclare-static ^
-Wno-gnu-anonymous-struct ^
-Wno-nested-anon-types ^
-Wno-extra-semi-stmt ^
-Wno-documentation ^
-Wno-documentation-unknown-command ^
-fshow-source-location ^
-O3 ^
./external/glad/src/glad.cpp ./external/log-c/log.cpp ./src/CoreLib/*.cpp ./src/engine/*.cpp ./src/engine/threading/*.cpp ./src/engine/platform/Win32.cpp
