cmake_minimum_required(VERSION 3.22)
project(MultiThreadedRayTracer)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror -pthread -O3 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)

find_package(GLEW REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

add_executable(Main
        src/simpleTexture.cpp
        src/SceneConstants.cpp
        src/Geometry/MeshLoader.cpp
        src/Core/Utils.cpp
        )

target_link_libraries(Main
    glfw
    GLEW
    GL
)