cmake_minimum_required(VERSION 4.2)
project(ocio_opengl C)

set(CMAKE_C_STANDARD 11)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.c
        src/shader.h
        src/utils.h)

target_link_libraries(${PROJECT_NAME} glad::glad glfw)
