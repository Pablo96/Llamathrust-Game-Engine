clang ^
-std=c17 ^
-DLT_RELEASE ^
-I"./include" ^
-o "bin/engine.exe" ^
-m64 ^
-lUser32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/optimizations.yml" ^
-Weverything ^
-O3  ^
./src/Engine.c ./src/platform/Win32.c
