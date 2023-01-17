cmake_minimum_required(VERSION 3.1...3.22)
project(GL_Global VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_GENERATOR "Visual Studio 17 2022" CACHE INTERNAL "" FORCE)
set (CMAKE_VERBOSE_MAKEFILE ON)
#set (MAKE_RULE_MESSAGES OFF)

set (BASE_DIR "..")
get_filename_component(
    BASE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/${BASE_DIR}"
    ABSOLUTE)
#set (BASE_DIR "D:/dev/personnel/GL_Sandbox")
set (ENV{GLM_PATH} "${BASE_DIR}/lib/GLM")
set (ENV{GLFW_PATH} "${BASE_DIR}/lib/GLFW")
set (ENV{GLAD_PATH} "${BASE_DIR}/lib/GLAD")
set (ENV{EXPRTK_PATH} "${BASE_DIR}/lib/exprtk")
set (ENV{FREEIMG_PATH} "${BASE_DIR}/lib/FreeImage")
set (ENV{JSON_PATH} "${BASE_DIR}/lib/json")
set (ENV{ASAN_PATH} "${BASE_DIR}/lib/asan")
set (glm_DIR $ENV{GLM_PATH}/cmake/glm)

message(STATUS "GLM_PATH = $ENV{GLM_PATH}")
message(STATUS "GLFW_PATH = $ENV{GLFW_PATH}")
message(STATUS "GLAD_PATH = $ENV{GLAD_PATH}")
message(STATUS "EXPRTK_PATH = $ENV{EXPRTK_PATH}")
message(STATUS "FREEIMG_PATH = $ENV{FREEIMG_PATH}")
message(STATUS "JSON_PATH = $ENV{JSON_PATH}")
message(STATUS "ASAN_PATH = $ENV{ASAN_PATH}")

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP8>)

include("build_GLAD.cmake")
include("build_app.cmake")