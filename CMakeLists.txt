cmake_minimum_required(VERSION 3.1)
cmake_policy(SET "CMP0079"  NEW)

# set configuration types and make them advanced option on cmake.
mark_as_advanced(CMAKE_INSTALL_PREFIX)
set(CMAKE_CONFIGURATION_TYPES Release Debug)
set(CMAKE_CXX_FLAGS_RELEASE_ASSERTS ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE_ASSERTS ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

# set c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project/solution name
project("Llamathrust Engine"
        VERSION 1.0
        DESCRIPTION "Game engine and Tools"
        LANGUAGES CXX)
# use folders for ZERO_CHECK and BUILD_ALL
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# remove ...
remove_definitions(/CMAKE_INTDIR)


######################################
# CMAKE OPTIONS
######################################


######################################
# DIRECTORIES AND NAMES
######################################

set(LTSTD_LIB "ltstd")
set(ENGINE_EXEC "llamathrust")
set(TEST_EXEC "tests")

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

#####################################
# Llamathrust Standard Library
#####################################

# set LTSTD sources
set(LTSTD_SRC
external/log-c/log.cpp
include/CoreLib/Array.hpp
include/CoreLib/Stack.hpp
include/CoreLib/Queue.hpp
include/CoreLib/String.hpp
src/CoreLib/Array.cpp
src/CoreLib/Stack.cpp
src/CoreLib/Queue.cpp
src/CoreLib/String.cpp
)

# LTSTD library
add_library(${LTSTD_LIB} STATIC ${LTSTD_SRC})
target_include_directories(${LTSTD_LIB} PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/external/log-c")

#####################################
# Llamathrust Game Engine
#####################################

# set LT sources
set(Llamathrust_SRC
external/glad/src/glad.cpp
external/log-c/log.cpp

include/Common.hpp
include/EngineConfig.hpp
include/ErrorCodes.hpp
include/Game.hpp
include/GameManager.hpp
include/Networking.hpp
include/Platform.hpp
include/System.hpp
include/Thread.hpp
include/ThreadPool.hpp

src/Engine/Engine.cpp
src/Engine/Engine.hpp
src/Engine/Input.cpp
src/Engine/Input.hpp
src/Engine/GraphicsAPI.cpp
src/Engine/GraphicsAPI.hpp
src/Engine/OpenGL.cpp
src/Engine/GameManager.cpp
src/Engine/Performance.hpp
src/Engine/ArgsParsing.hpp
src/Engine/ArgsParsing.cpp
src/Engine/System.cpp
src/Engine/networking/Socket.hpp
src/Engine/networking/Socket.cpp
src/Engine/threading/Thread.cpp
src/Engine/threading/ThreadPool.cpp
)

if (WIN32)
  list(APPEND Llamathrust_SRC src/Engine/platform/Win32.cpp)
endif()

if (UNIX)
  list(APPEND Llamathrust_SRC src/Engine/platform/Linux.cpp)
endif()

# Engine executable name
add_executable(${ENGINE_EXEC} ${Llamathrust_SRC})

# target name label
set_target_properties(${ENGINE_EXEC} PROPERTIES PROJECT_LABEL ${ENGINE_EXEC})

# set include dirs
target_include_directories(${ENGINE_EXEC} PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/external/" "${CMAKE_SOURCE_DIR}/external/log-c" "${CMAKE_SOURCE_DIR}/external/glad/include")

# set filters
foreach(_source IN ITEMS ${Llamathrust_SRC})
# Get the directory of the source file
get_filename_component(_source_path "${_source}" PATH)

# Make sure we are using windows slashes
string(REPLACE "/" "\\" _group_path "${_source_path}")

source_group("${_group_path}" FILES "${_source}")
endforeach()

#link to ltstd
target_link_libraries(${ENGINE_EXEC} ${LTSTD_LIB})

# find and link to opengl
if (UNIX)
  set(OpenGL_GL_PREFERENCE GLVND)
endif()

find_package(OpenGL COMPONENTS GL OpenGL GLX)

if (NOT OpenGL_FOUND AND UNIX)
  message(STATUS "OpenGL not found. Make sure to have installed libgl1-mesa-dev.")
  return()
endif()

target_link_libraries(${ENGINE_EXEC} ${OPENGL_LIBRARY})
target_include_directories(${ENGINE_EXEC} PRIVATE "${OPENGL_INCLUDE_DIR}")

# link to platform specific libs
if(WIN32)
  target_link_libraries(${ENGINE_EXEC} wsock32 ws2_32)
endif()

# some code is from GLFW
if (UNIX)
  find_package(X11 REQUIRED)
  
  # Check for Xkb (X keyboard extension)
  if (NOT X11_Xkb_INCLUDE_PATH)
    message(FATAL_ERROR "XKB headers not found; install X11 development package")
  endif()
  
  target_include_directories(${ENGINE_EXEC} PRIVATE "${X11_X11_INCLUDE_PATH}")
  target_link_libraries(${ENGINE_EXEC} ${CMAKE_DL_LIBS})
endif()



#####################################
# SET BUILD TARGETS
#####################################

# DEBUG
set_property(TARGET ${ENGINE_EXEC} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:LT_DEBUG>)
#RELEASE
set_property(TARGET ${ENGINE_EXEC} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>:LT_RELEASE>)


#####################################
# TEST PROJECT
#####################################

set(Test_SRC
src/Tests/main.cpp
src/Tests/test_framework.hpp
src/Tests/CoreLib.hpp
src/Tests/Threading.hpp
)

if (WIN32)
  list(APPEND Llamathrust_SRC src/Tests/test_framework_win32.cpp)
endif()

if (UNIX)
  list(APPEND Llamathrust_SRC src/Tests/test_framework_linux.cpp)
endif()

# Test executable name
add_executable(${TEST_EXEC} ${Test_SRC} ${Llamathrust_SRC})
target_include_directories(${TEST_EXEC} PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/external/" "${CMAKE_SOURCE_DIR}/external/log-c" "${CMAKE_SOURCE_DIR}/external/glad/include")

set_property(TARGET ${TEST_EXEC} APPEND PROPERTY COMPILE_DEFINITIONS LT_TEST_FRAMEWORK LT_NO_MAIN LT_DEBUG)
target_link_libraries(${TEST_EXEC} ${OPENGL_LIBRARY})
target_link_libraries(${TEST_EXEC} ${LTSTD_LIB})
