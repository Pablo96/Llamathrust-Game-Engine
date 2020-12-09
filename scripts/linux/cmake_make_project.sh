cd ../..
if [ ! -d ./project_files ];
then
mkdir -p ./project_files
fi

cd "project_files"
cmake ..