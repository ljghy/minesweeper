add_subdirectory(imgui)
add_subdirectory(opengl_framework)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
add_executable(minesweeper WIN32)
else()
add_executable(minesweeper)
endif()

set_property(TARGET minesweeper PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<1:${PROJECT_SOURCE_DIR}/bin>)

if (MSVC)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT minesweeper)
endif (MSVC)

if ((CMAKE_SYSTEM_NAME MATCHES "Windows") AND ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")))
target_link_options(minesweeper PRIVATE -mwindows)
endif()

target_sources(minesweeper
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/Minesweeper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/MineMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/MineMapRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/DigitRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/RecordManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/PreferencesManager.cpp
    
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
