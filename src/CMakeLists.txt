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
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/main.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/Minesweeper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/MineMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/MineMapRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/DigitRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/RecordManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/PreferencesManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/minesweeper/ResourceManager.cpp

)

target_include_directories(minesweeper
PRIVATE 
    ${PROJECT_SOURCE_DIR}/lib/glad/include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
    ${PROJECT_SOURCE_DIR}/lib/imgui/include
    ${PROJECT_SOURCE_DIR}/lib/opengl_framework/include
    ${PROJECT_SOURCE_DIR}/lib/stb/include
)

find_package(OpenGL REQUIRED)

target_link_libraries(minesweeper
PRIVATE 
    glad
    glfw
    imgui
    OpenGL::GL
    opengl_framework
)
