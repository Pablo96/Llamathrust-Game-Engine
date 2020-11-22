cls
if not exist "..\..\project_files" (
    call "cmake_vs_proj.bat"
    cd ..
) else (
    cd ../..
)
if not exist ".\bin\debug" mkdir ".\bin\debug"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat";

:: Engine Executable
msbuild ./project_files/llamathrust.vcxproj -property:Configuration=Debug

del .\bin\debug\*.ilk
del .\bin\debug\*.pdb
