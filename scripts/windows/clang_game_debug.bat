cls
cd ../..
if not exist ".\bin\debug" mkdir ".\bin\debug"

:: Engine Executable
clang ^
-std=c17 ^
-DLT_DEBUG ^
-I"./include" ^
-I"./external" ^
-I"./external/log-c" ^
-I"./external/glad/include" ^
-o "bin/debug/game.dll" ^
-m64 ^
-lUser32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/debug/optimizations.yml" ^
-Weverything ^
-Wno-unused-parameter ^
-fshow-source-location ^
-O0 ^
-shared ^
./src/gameexample/*.c