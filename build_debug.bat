:: Engine Executable
clang ^
-std=c17 ^
-DLT_DEBUG ^
-I"./include" ^
-I"./external/log-c" ^
-I"./external" ^
-o "bin/engine.exe" ^
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
-H ^
./external/log-c/log.c ./src/*.c ./src/platform/*.c
