cmake_minimum_required(VERSION 3.1) # CXX_STANDARD
project(demo)

# if not specified, build a release build
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 11)

# debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DDEBUG")
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wundef -Wshadow")
endif(CMAKE_COMPILER_IS_GNUCXX)

# release flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3 -DSYNC_PLAYER")

# build time definitions
add_definitions("-DROW_RATE='((60./60.)*8)'")

# static stdc++ for max compatibility.
# Target is GNU/Linux; libgcc and libc.so.6 should be present.
# C++ versions are a mismatch.
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-static-libstdc++")

# Glob src files
file(GLOB SRC_FILES
  "src/*.cpp"
)

# Copy shader files
file(GLOB SHADER_FILES "src/*.glsl" "src/*.frag" "src/*.vert")
foreach(SHADER_FILE ${SHADER_FILES})
  configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Build libraries config-time because they're supposed to be untouched
# and I couldn't get make to work with add_custom_command

# rocket
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(ROCKET_LIBRARY_TARGET lib/librocket-player.a)
else()
  set(ROCKET_LIBRARY_TARGET lib/librocket.a)
endif()
execute_process(COMMAND make ${ROCKET_LIBRARY_TARGET}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/rocket
)
set(ROCKET_LIBRARY_FILE
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/rocket/${ROCKET_LIBRARY_TARGET}
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/rocket/lib)

# glew
set(GLEW_LIBRARY_FILE
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/lib/libGLEW.a
)
execute_process(COMMAND make extensions
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew
)
execute_process(COMMAND make glew.lib.static
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/include)

# stb
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/stb)

# the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ROCKET_LIBRARY_FILE})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY_FILE})
