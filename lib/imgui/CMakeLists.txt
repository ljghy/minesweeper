add_library(imgui STATIC)

target_sources(imgui
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_widgets.cpp
)

target_include_directories(imgui
PRIVATE 
    ${PROJECT_SOURCE_DIR}/lib/glad/include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
    ${PROJECT_SOURCE_DIR}/lib/imgui/include
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
