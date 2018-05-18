cmake_minimum_required (VERSION 3.0.0)
project (pencil-drawing-filter LANGUAGES CXX)

# set (CMAKE_BUILD_TYPE DEBUG)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
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

include (ExternalProject)

ExternalProject_Add(sketch-filter
    GIT_REPOSITORY https://github.com/Jim00000/sketch-filter
    PREFIX ${CMAKE_BINARY_DIR}/external
    INSTALL_COMMAND ""
    CMAKE_ARGS -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-9.1 # Potential bugs. Fix it
    -DCMAKE_BUILD_TYPE=RELEASE
)

include_directories (${CMAKE_BINARY_DIR}/external/src/sketch-filter/include)
include_directories (include/)
link_directories (${CMAKE_BINARY_DIR}/external/src/sketch-filter-build/lib)
add_subdirectory (src/)
add_subdirectory (test/)


# include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
# link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)