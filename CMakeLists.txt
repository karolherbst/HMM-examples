cmake_minimum_required(VERSION 3.0)
project(HMM-Examples)

find_package(OpenCL 2.0)

include_directories(${CMAKE_CURRENT_DIRECTORY})
add_library(HMMExamplesBase STATIC
	base.cpp
	base.h
	defines.h
)

add_executable(linkedList
	example_list.c
	example_list.h
	linked_list.cpp
)
target_link_libraries(linkedList
	OpenCL::OpenCL
	HMMExamplesBase
)

add_executable(fma
	fma.cpp
)
target_link_libraries(fma
	OpenCL::OpenCL
	HMMExamplesBase
)

add_executable(fma_struct
	fma_struct.cpp
	fma_struct.h
)
target_link_libraries(fma_struct
	OpenCL::OpenCL
	HMMExamplesBase
)

add_executable(fma_struct_ptrs
	fma_struct_ptrs.cpp
	fma_struct_ptrs.h
)
target_link_libraries(fma_struct_ptrs
	OpenCL::OpenCL
	HMMExamplesBase
)
