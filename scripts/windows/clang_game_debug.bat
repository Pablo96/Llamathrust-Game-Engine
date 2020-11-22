cls
cd ../..
if not exist ".\bin\debug" mkdir ".\bin\debug"

:: Engine Executable
clang ^
-std=c++17 ^
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
-ggdb ^
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
-shared ^
./src/gameexample/*.cpp