cmake_minimum_required (VERSION 3.0.0)
project (pencil-drawing-filter LANGUAGES CXX)

set (${CMAKE_CXX_FLAGS_DEBUG} "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set (CMAKE_BUILD_TYPE DEBUG)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package (OpenCV REQUIRED)
if (OpenCV_FOUND)
    include_directories (${OpenCV_INCLUDE_DIRS})
endif ()

find_package (OpenMP)
if (OpenMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

include_directories (include/)
add_subdirectory (src/)
add_subdirectory (test/)