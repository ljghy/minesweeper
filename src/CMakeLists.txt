add_subdirectory(imgui)
add_subdirectory(opengl_framework)

add_executable(minesweeper)

target_sources(minesweeper
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Minesweeper.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/MineMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/MineMapRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/DigitRenderer.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/ResourceManager.cpp

)

target_include_directories(minesweeper
PRIVATE 
    ${PROJECT_SOURCE_DIR}/deps/glad/include
    ${PROJECT_SOURCE_DIR}/deps/glfw/include
    ${PROJECT_SOURCE_DIR}/include/imgui
    ${PROJECT_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)

target_link_libraries(minesweeper
PRIVATE 
    glfw
    imgui
    OpenGL::GL
    opengl_framework
)