cmake_minimum_required(VERSION 3.1)
project(demo)

# if not specified, build a release build
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# C++11
set(CMAKE_CXX_STANDARD 11)

# compiler flags, build decides sync speed (beats per sec * rows per beat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROW_RATE='((160./60.)*8)'")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O2 -DSYNC_PLAYER")

# static stdc++ for max compatibility.
# Target is GNU/Linux; libgcc and libc.so.6 should be present.
# C++ versions are a mismatch.
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-static-libstdc++")

# Glob src files
file(GLOB SRC_FILES
  "src/*.cpp"
  "lib/rocket/*.c" # Just compile rocket in, static linking sucks
  "lib/glew/*.c"
)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
# rocket
include_directories("lib/rocket")
# stb
include_directories("lib/stb")
# glew
include_directories("lib/glew")

# the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
