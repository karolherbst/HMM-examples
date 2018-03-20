cmake_minimum_required(VERSION 3.0)
project(HMM-Examples)

find_package(OpenCL 2.0)

include_directories(${CMAKE_CURRENT_DIRECTORY})
add_executable(example
	example_list.c
	example_list.h
	main.cpp
)
target_link_libraries(example OpenCL::OpenCL)
