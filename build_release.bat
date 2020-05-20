clang ^
-std=c17 ^
-DLT_RELEASE ^
-I"./include" ^
-I"./external/log-c" ^
-o "bin/engine.exe" ^
-m64 ^
-lUser32.lib ^
-lGdi32.lib ^
-fcolor-diagnostics ^
-fomit-frame-pointer ^
-pedantic ^
-fsave-optimization-record=yaml ^
-foptimization-record-file="bin/optimizations.yml" ^
-Weverything ^
-O3  ^
./external/log-c/log.c ./src/GraphicsAPI.c ./src/Engine.c ./src/platform/Win32.c
