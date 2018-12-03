cmake_minimum_required(VERSION 3.0)
project(HMM-Examples)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenCL 2.0)

include_directories(${CMAKE_CURRENT_DIRECTORY})
add_library(HMMExamplesBase STATIC
	base.c
	base.h
	defines.h
	fault.c
	linked_list.h
	linked_list_number.h
)

add_executable(fma
	fma.c
)
target_link_libraries(fma
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(fma_struct
	fma_struct.c
	fma_struct.h
)
target_link_libraries(fma_struct
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(fma_struct_by_value
	fma_struct_by_value.c
	fma_struct_by_value.h
)
target_link_libraries(fma_struct_by_value
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(fma_packed_struct_by_value
	fma_packed_struct_by_value.c
	fma_packed_struct_by_value.h
)
target_link_libraries(fma_packed_struct_by_value
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(fma_struct_ptrs
	fma_struct_ptrs.c
	fma_struct_ptrs.h
)
target_link_libraries(fma_struct_ptrs
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(mat_mul
	mat_mul.c
)
target_link_libraries(mat_mul
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(convert_test
	convert_test.c
)
target_link_libraries(convert_test
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(linked_list_insertion_sort
	linked_list_insertion_sort.c
)
target_link_libraries(linked_list_insertion_sort
	OpenCL::OpenCL
	HMMExamplesBase
	m
)

add_executable(fault
	fault.c
)
target_link_libraries(fault
	OpenCL::OpenCL
	HMMExamplesBase
)

add_executable(normalize
	normalize.c
)
target_link_libraries(normalize
	OpenCL::OpenCL
	HMMExamplesBase
	m
)
