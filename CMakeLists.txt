cmake_minimum_required(VERSION 3.1)
project(GatorEngine)

set(CMAKE_CXX_STANDARD 17)

# Add SFML
set(SFML_DIR ${CMAKE_SOURCE_DIR}/SFML)
add_subdirectory(${SFML_DIR})

# Add core ImGui library
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

# Add ImGui-SFML integration
set(IMGUI_SFML_DIR ${CMAKE_SOURCE_DIR}/imgui-sfml)
add_subdirectory(${IMGUI_SFML_DIR})

# Add Box2D without building its examples and tests
set(BOX2D_DIR ${CMAKE_SOURCE_DIR}/box2d)
set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "Do not build Box2D examples" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Do not build Box2D testbed" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build Box2D unit tests" FORCE)
add_subdirectory(${BOX2D_DIR})

# Main executable
add_executable(GatorEngine main.cpp)
target_link_libraries(GatorEngine PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML::ImGui-SFML
    box2d
)

# Set the include directories for the GatorEngine target
target_include_directories(GatorEngine PRIVATE ${SFML_INCLUDE_DIR} ${IMGUI_DIR} ${IMGUI_SFML_DIR})