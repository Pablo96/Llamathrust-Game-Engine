cls
cd ../..
if exist ".vs\" (
    rmdir /S /Q ".vs\"
)
if exist "bin\" (
    rmdir /S /Q "bin\"
)
if exist "x64\" (
    rmdir /S /Q "x64\"
)
if exist "CMakeFiles\" (
    rmdir /S /Q "CMakeFiles\"
)
if exist "project_files\" (
    rmdir /S /Q "project_files\"
)
