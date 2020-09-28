cd ../..
if [ ! -d ./bin/debug ];
then
mkdir -p ./bin/debug
fi

# Engine Executable
gcc \
-std=c11 \
-DLT_DEBUG \
-D_CRT_SECURE_NO_WARNINGS \
-I"./include" \
-I"./external" \
-I"./external/log-c" \
-I"./external/glad/include" \
-o "bin/debug/engine_linux" \
-m64 \
-pthread \
-fomit-frame-pointer \
-pedantic \
-fdump-tree-optimized \
-static-libgcc \
-ggdb \
-Wall \
-Wno-unused-parameter \
-O0 \
./external/glad/src/glad.c ./external/log-c/log.c ./src/CoreLib/*.c ./src/Engine/*.c ./src/Engine/threading/*.c ./src/Engine/platform/Linux.c \
-ldl \
-lGL \
-lX11 \
