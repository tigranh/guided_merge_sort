
cmake_minimum_required (VERSION 3.23)

project (guided_merge_sort_demo)

set (CMAKE_CXX_STANDARD 17)

set (HEADER_FILES
	include/insertion_sort.hpp
	include/stl_sort_strategy.hpp
	include/stl_heap_sort_strategy.hpp 
	include/stl_merge_strategy.hpp 
	include/merge.hpp 
	include/guided_merge.hpp 
	include/merge_sort.hpp 
	)
	
set (SOURCE_FILES
	src/main.cpp
	)

add_executable ( guided_merge_sort_demo ${HEADER_FILES} ${SOURCE_FILES} )

