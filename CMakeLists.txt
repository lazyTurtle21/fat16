cmake_minimum_required(VERSION 3.15)
project(fat)

set(CMAKE_CXX_STANDARD 14)

add_executable(fat main.cpp headers/custom_types.h sources/fat16.cpp headers/fat16.h)