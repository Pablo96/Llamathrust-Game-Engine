cls
cd ../..
if not exist ".\bin\tests" mkdir ".\bin\tests"

:: Engine Executable
clang ^
-std=c17 ^
-DLT_DEBUG ^
-D_CRT_SECURE_NO_WARNINGS ^
-DLT_NO_MAIN ^
-DLT_TEST_FRAMEWORK ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o "bin/tests/tests.exe" ^
-m64 ^
-lOpengl32.lib ^
-lGdi32.lib ^
-lUser32.lib ^
-lKernel32.lib ^
-lWs2_32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/debug/optimizations.yml" ^
-w ^
-fshow-source-location ^
-O0 ^
./external/glad/src/glad.c ./external/log-c/log.c ^
./src/CoreLib/*.c ./src/Engine/*.c ^
./src/Engine/threading/*.c ./src/Engine/platform/*.c ^
./src/Engine/networking/*.c ./src/Tests/*.c

.\bin\tests\tests.exe
