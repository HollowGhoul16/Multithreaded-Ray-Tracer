cmake_minimum_required(VERSION 3.22)
project(MultiThreadedRayTracer)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror -pthread -ffast-math -O3 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

add_executable(Main
    src/simpleTexture.cpp
    src/stb_image_impl.cpp
    src/Window/Window.cpp
    src/Resources/ResourceManager.cpp
    src/Resources/TextureLoader.cpp
    src/Resources/TextureData.cpp
    src/Resources/MeshLoader.cpp
    src/Resources/MeshData.cpp
    src/Parsers/ObjParser.cpp
    src/Core/Screenshot.cpp
    src/Core/Utils.cpp
)

target_link_libraries(Main
    glfw
    GLEW
    GL
)