cls
cd ../..
if not exist ".\bin\release" mkdir ".\bin\release"

:: Engine Executable
clang ^
-std=c17 ^
-DLT_RELEASE ^
-DLT_EDITOR ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o "bin/release/editor_clang.exe" ^
-m64 ^
-lOpengl32.lib ^
-lGdi32.lib ^
-lUser32.lib ^
-lKernel32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/release/optimizations.yml" ^
-Weverything ^
-Wno-documentation ^
-O3  ^
./external/glad/src/glad.c ./external/log-c/log.c ./src/engine/*.c ./src/engine/platform/Win32.c
