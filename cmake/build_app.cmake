project(
	GL_Sandbox
	VERSION 0.1.0
	LANGUAGES CXX
)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${BASE_DIR}/src/*.cpp")
message(STATUS "SOURCES = ${PROJECT_SOURCES}")

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

find_package(glm REQUIRED)
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC
"$ENV{GLFW_PATH}" 
"$ENV{GLAD_PATH}/lib"
"$ENV{FREEIMG_PATH}/lib")
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC 
"/std:c++20" 
"/openmp" 
"/Zc:__cplusplus" 
"/bigobj"
# "$<$<CONFIG:DEBUG>:/fsanitize=address>" 
"$<$<CONFIG:DEBUG>:/Od>"
"$<$<CONFIG:DEBUG>:/sdl>"
"$<$<CONFIG:DEBUG>:/W2>"
"$<$<CONFIG:DEBUG>:/wd5264>"
"$<$<CONFIG:RELEASE>:/O2>")
target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
"/NODEFAULTLIB:LIBCMTD"
"$<$<CONFIG:DEBUG>:/INCREMENTAL:NO>")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
"${BASE_DIR}/inc" 
"$ENV{GLM_PATH}" 
"$ENV{GLFW_PATH}/include" 
"$ENV{GLAD_PATH}/include"
"$ENV{EXPRTK_PATH}/include"
"$ENV{FREEIMG_PATH}/include"
"$ENV{JSON_PATH}/include")
target_link_libraries(${CMAKE_PROJECT_NAME} glfw3 GLAD FreeImage)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:DEBUG=1>)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Release>,copy_directory,true>
		${BASE_DIR}/src/shaders
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Release>,copy_directory,true>
		${BASE_DIR}/extern/tex
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/tex
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
		$ENV{FREEIMG_PATH}/run/FreeImage.dll
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
		$ENV{ASAN_PATH}/clang_rt.asan_dbg_dynamic-x86_64.dll
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
		$ENV{ASAN_PATH}/clang_rt.asan_dynamic-x86_64.dll
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
		$ENV{ASAN_PATH}/llvm-symbolizer.exe
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)