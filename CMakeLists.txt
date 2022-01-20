cmake_minimum_required(VERSION 3.11)
project(colmapeasy_impl)

add_subdirectory(pybind11)

# add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread")
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(OpenGL_GL_PREFERENCE GLVND)
file(GLOB SOURCES src/*.cpp)

find_package(COLMAP 3.7 REQUIRED)
include_directories(${COLMAP_INCLUDE_DIRS} include)
link_directories(${COLMAP_LINK_DIRS} src)

pybind11_add_module(colmapeasy_impl ${SOURCES})

target_link_libraries(colmapeasy_impl PRIVATE ${COLMAP_LIBRARIES})
