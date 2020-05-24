cmake_minimum_required(VERSION 3.1)

cmake_policy(SET "CMP0079"  NEW)

# set configuration types and make them advanced option on cmake.
mark_as_advanced(CMAKE_INSTALL_PREFIX)
set(CMAKE_CONFIGURATION_TYPES Release Debug)
set(CMAKE_CXX_FLAGS_RELEASE_ASSERTS ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE_ASSERTS ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

# set c standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# set the project/solution name
project("Llamathrust Engine"
        VERSION 1.0
        DESCRIPTION "Game engine Win32"
        LANGUAGES C)
# use folders for ZERO_CHECK and BUILD_ALL
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# remove ...
remove_definitions(/CMAKE_INTDIR)

set(TARGET_NAME "llamathrust")
# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

# set sources
set(Llamathrust_SRC
external/glad/src/glad.c
external/log-c/log.c
src/Engine.c
src/Input.c
src/GraphicsAPI.c
src/platform/Win32.c
)

# executable name
add_executable(${TARGET_NAME} ${Llamathrust_SRC})

target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/external/" "${CMAKE_SOURCE_DIR}/external/log-c" "${CMAKE_SOURCE_DIR}/external/glad/include")

# set filters
foreach(_source IN ITEMS ${Llamathrust_SRC})
# Get the directory of the source file
get_filename_component(_source_path "${_source}" PATH)

# Make sure we are using windows slashes
string(REPLACE "/" "\\" _group_path "${_source_path}")

source_group("${_group_path}" FILES "${_source}")
endforeach()

# find and link to opengl
find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARY})
target_include_directories(${TARGET_NAME} PRIVATE "${OPENGL_INCLUDE_DIR}")

# target name label
set_target_properties(${TARGET_NAME} PROPERTIES PROJECT_LABEL ${TARGET_NAME})

# DEBUG
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY
  COMPILE_DEFINITIONS $<$<CONFIG:Debug>:LT_DEBUG>)
#RELEASE
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY
  COMPILE_DEFINITIONS $<$<CONFIG:Release>:LT_RELEASE>)