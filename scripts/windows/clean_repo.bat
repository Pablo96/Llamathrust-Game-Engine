cls
cd ../..
if exist ".vs\" (
    rmdir ".vs\"
)
if exist "bin\" (
    rmdir "bin\ "
)
if exist "x64\" (
    rmdir "x64\"
)
if exist "CMakeFiles\" (
    rmdir "CMakeFiles\"
)
if exist "project_files\" (
    rmdir "project_files\"
)
