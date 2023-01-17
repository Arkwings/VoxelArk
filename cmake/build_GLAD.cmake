project(
	GLAD
	VERSION 0.1.0
	LANGUAGES C
)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "$ENV{GLAD_PATH}/lib")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "$ENV{GLAD_PATH}/lib")

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "$ENV{GLAD_PATH}/src/*.c")
message(STATUS "SOURCES = ${PROJECT_SOURCES}")

add_library(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "$ENV{GLAD_PATH}/include")
