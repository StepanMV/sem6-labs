cmake_minimum_required(VERSION 3.10)
project(glut-test)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
    FreeGLUT
    URL https://github.com/freeglut/freeglut/releases/download/v3.6.0/freeglut-3.6.0.tar.gz
)
FetchContent_MakeAvailable(FreeGLUT)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/nigels-com/glew.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glew)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freeglut_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glew_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL freeglut glu32)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${glew_SOURCE_DIR}/lib/Release/x64/glew32s.lib)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE glew)
endif()

add_custom_target(copy-resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)

add_dependencies(${PROJECT_NAME} copy-resources)
