cls
if not exist ".\bin\release" mkdir ".\bin\release"

:: Engine Executable
clang ^
-std=c17 ^
-DLT_RELEASE ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o "bin/release/engine_clang.exe" ^
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
-O3  ^
./external/glad/src/glad.c ./external/log-c/log.c ./src/*.c ./src/platform/*.c
