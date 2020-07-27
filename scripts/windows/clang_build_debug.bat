cd ../..
if not exist ".\bin\debug" mkdir ".\bin\debug"
cls

:: Engine Executable
clang ^
-std=c17 ^
-D_CRT_SECURE_NO_WARNINGS ^
-DLT_DEBUG ^
-D_CRT_SECURE_NO_WARNINGS ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o "bin/debug/engine_clang.exe" ^
-m64 ^
-lOpengl32.lib ^
-lGdi32.lib ^
-lUser32.lib ^
-lKernel32.lib ^
-lWs2_32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/debug/optimizations.yml" ^
-static-libgcc ^
-ggdb ^
-Weverything ^
-Wno-unused-parameter ^
-Wno-documentation ^
-Wno-documentation-unknown-command ^
-fshow-source-location ^
-O0 ^
./external/glad/src/glad.c ./external/log-c/log.c ./src/CoreLib/*.c ./src/engine/*.c ./src/engine/networking/*.c ./src/engine/threading/*.c ./src/engine/platform/*.c
