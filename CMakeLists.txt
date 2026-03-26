cmake_minimum_required(VERSION 3.20)
project(Project2)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Project2 src/main.cpp src/DataLoader.cpp src/MergeSort.cpp src/QuickSort.cpp)