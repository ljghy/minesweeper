add_library(glad STATIC)

target_sources(glad
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
)

target_include_directories(glad
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
