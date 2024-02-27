cmake_minimum_required(VERSION 3.8)

project(breakout VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(AddDependency)

find_package(OpenGL REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(deps/glad)

add_github_dependency(freetype freetype/freetype VER-2-13-2)
add_github_dependency(glm g-truc/glm bf71a834948186f4097caa076cd2663c69a10e1e)
add_github_dependency(glfw glfw/glfw 3.3.2)
add_github_dependency(spdlog gabime/spdlog v1.13.0)

add_subdirectory(assets)
add_subdirectory(src)