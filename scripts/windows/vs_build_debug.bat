cls
cd ../..
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat";
if not exist ".\bin\debug" mkdir ".\bin\debug"

:: Engine Executable
cl /nologo /std:c++17 /GS  /W3 /Gd /MDd /fp:precise ^
/D "WIN32" /D "_WINDOWS" /D "LT_DEBUG" /D "_MBCS" ^
/I".\include" ^
/I".\external" ^
/I".\external\log-c" ^
/I".\external\glad\include" ^
/Z7 /Od /Ob0 ^
/TC ./external/glad/src/glad.c ./external/log-c/log.c ./src/*.c ./src/platform/*.c ^
/link ^
/DEBUG ^
/SUBSYSTEM:CONSOLE ^
/OUT:"D:\Workspace\Engines\LlamathrustC_V0.1\bin\debug\engine_vs.exe" ^
/DYNAMICBASE "opengl32.lib" "kernel32.lib" "user32.lib" "gdi32.lib"

del *.obj
del .\bin\debug\*.ilk
del .\bin\debug\*.pdb
