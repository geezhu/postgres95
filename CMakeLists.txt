cmake_minimum_required(VERSION 3.25)
project(postgres95 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_MODULE_PATH 
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/cmake_modules
        )
include(build_function)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)


#add_executable(postgres95 main.c)
