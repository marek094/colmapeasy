cmake_minimum_required(VERSION 3.11)
project(colmapeasy_impl)

set(COLMAP_ROOT_DIR /home/meu/git/colmapeasy/colmap/build/installed)
find_package(COLMAP 3.7 REQUIRED PATHS ${COLMAP_ROOT_DIR} NO_DEFAULT_PATH)

add_subdirectory(pybind11)

# add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread")
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(OpenGL_GL_PREFERENCE GLVND)
file(GLOB SOURCES src/*.cpp)

# find_package(COLMAP 3.7 REQUIRED)
include_directories(${COLMAP_INCLUDE_DIRS} include)
link_directories(${COLMAP_LINK_DIRS} src)

message(WARNING ${COLMAP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
message(WARNING ${COLMAP_INCLUDE_DIRS} Hello_World! )


pybind11_add_module(colmapeasy_impl ${SOURCES})

target_link_libraries(colmapeasy_impl PRIVATE ${COLMAP_LIBRARIES})
