cmake_minimum_required(VERSION 3.10)

project(glut-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

include(FetchContent)
FetchContent_Declare(
    FreeGLUT
    URL https://github.com/freeglut/freeglut/releases/download/v3.6.0/freeglut-3.6.0.tar.gz
)
FetchContent_MakeAvailable(FreeGLUT)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL OpenGL::GLU freeglut)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freeglut_SOURCE_DIR}/include)

add_custom_target(copy-resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)

add_dependencies(${PROJECT_NAME} copy-resources)
