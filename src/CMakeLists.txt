cmake_minimum_required (VERSION 3.0.0)
add_subdirectory (lic/)

add_library (pencil-drawing white_noise_gen.cpp vector_field_gen.cpp border.cpp combiner.cpp lic/lic.cpp)
add_dependencies (pencil-drawing sketch-filter)