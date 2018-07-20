cmake_minimum_required (VERSION 3.0.0)

add_custom_target (copyPaperTexture ALL)
add_custom_command (
    TARGET copyPaperTexture PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/texture/ ${CMAKE_BINARY_DIR}/texture
    COMMENT "---Copy texture directory to build directory---"
)