cmake_minimum_required(VERSION 3.17)
project(HoleFilling)

set(CMAKE_CXX_STANDARD 14)

#find_library(OpenCV)
find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 11)

add_executable(HoleFilling main.cpp HoleFiller.cpp ImageMasker.cpp MyWeightFunction.cpp)

target_link_libraries(HoleFilling ${OpenCV_LIBS})

