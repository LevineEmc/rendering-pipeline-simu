cmake_minimum_required(VERSION 3.20)

project(SoftwareRenderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(SoftwareRenderer
        main.cpp
        framebuffer.cpp
        math.h
        objloader.h
        objloader.cpp
        mat4.h
)