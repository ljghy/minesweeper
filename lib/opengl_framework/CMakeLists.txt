add_library(opengl_framework STATIC)

target_sources(opengl_framework
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameBufferObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VertexArrayObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VertexBufferObject.cpp
)

target_include_directories(opengl_framework
PRIVATE 
    ${PROJECT_SOURCE_DIR}/lib/glad/include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
    ${PROJECT_SOURCE_DIR}/lib/opengl_framework/include
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
