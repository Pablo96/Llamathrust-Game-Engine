cls
cd ../..
if not exist ".\bin\tests" mkdir ".\bin\tests"

:: Engine Executable
clang ^
-std=c++17 ^
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
-Weverything ^
-Wno-c++98-compat-pedantic ^
-Wno-microsoft-redeclare-static ^
-Wno-old-style-cast ^
-Wno-gnu-anonymous-struct ^
-Wno-nested-anon-types ^
-Wno-extra-semi-stmt ^
-Wno-unused-parameter ^
-Wno-documentation ^
-Wno-documentation-unknown-command ^
-fshow-source-location ^
-O0 ^
./external/glad/src/glad.cpp ./external/log-c/log.cpp ^
./src/CoreLib/*.cpp ./src/Engine/*.cpp ^
./src/Engine/threading/*.cpp ./src/Engine/platform/Win32.cpp ^
./src/Engine/networking/*.cpp ./src/Tests/*.cpp

.\bin\tests\tests.exe
