cmake_minimum_required(VERSION 3.20)
project(raytracing)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(raytracing main.cpp vec3.h color.h ray.h hittable.h sphere.h hittableList.h raytracing.h camera.h material.h)
