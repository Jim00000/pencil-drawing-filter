cmake_minimum_required (VERSION 3.0.0)

add_executable (test-seg test.cpp)
add_executable (test-fourier test_fourier.cpp)
target_link_libraries (test-seg ${OpenCV_LIBS})
target_link_libraries (test-fourier ${OpenCV_LIBS})