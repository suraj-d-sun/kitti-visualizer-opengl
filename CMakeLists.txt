cmake_minimum_required(VERSION 3.10)
project(kitti_visualizer_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# OpenGL configuration for modern systems
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# For legacy OpenGL (if needed, uncomment the following line)
# set(OpenGL_GL_PREFERENCE LEGACY)



# Add include directories
include_directories(
    include
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
    external/imgui
)

# GLFW: if installed system-wide, fallback to that
if (EXISTS "${CMAKE_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(STATUS "Using external GLFW...")
    add_subdirectory(external/glfw)
    set(GLFW_LIB glfw)
else()
    message(STATUS "Using system GLFW...")
    find_package(glfw3 REQUIRED)
    set(GLFW_LIB glfw3)
endif()

# GLAD
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# glm (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# stb_image (header-only)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)

file(GLOB IMGUI_SOURCES
    external/imgui/*.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends)

# Collect all source files automatically (src/**)
file(GLOB_RECURSE SOURCES
    src/core/*.cpp
    src/data/*.cpp
    src/input/*.cpp
    src/rendering/*.cpp
    src/utils/*.cpp
    src/main.cpp
)

# Create executable
add_executable(kitti_visualizer ${SOURCES})

# Link libraries
target_link_libraries(kitti_visualizer
    glfw
    glad
    glm
    stb
    imgui
    OpenGL::GL
)

# OpenGL
find_package(OpenGL REQUIRED)

# Warnings (GCC/Clang)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(kitti_visualizer PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Build ready. Run from: build/bin/kitti_visualizer")

