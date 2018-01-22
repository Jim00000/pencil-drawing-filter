cmake_minimum_required (VERSION 3.0.0)
add_subdirectory (lic/)

add_library (pencil-drawing white_noise_gen.cpp lic/lic.cpp)
add_executable (test-pd main.cpp)
target_link_libraries (test-pd pencil-drawing ${OpenCV_LIBS})