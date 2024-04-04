cmake_minimum_required(VERSION 3.1)
project(GatorEngine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(luajit
    GIT_REPOSITORY https://github.com/WohlSoft/LuaJIT.git
    GIT_TAG        879f3976f3657c11464206b2d49e21accdfb62bc
)
FetchContent_MakeAvailable(luajit)

FetchContent_Declare(
    nativefiledialog-extended
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG        17b6e8ce219c0677f94b63636abb9296b28841ca
)
FetchContent_MakeAvailable(nativefiledialog-extended)

# Add SFML
set(SFML_DIR ${CMAKE_SOURCE_DIR}/extern/SFML)
add_subdirectory(${SFML_DIR})

# Add core ImGui library
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/extern/imgui)

# Add ImGui-SFML integration
set(IMGUI_SFML_DIR ${CMAKE_SOURCE_DIR}/extern/imgui-sfml)
add_subdirectory(${IMGUI_SFML_DIR})

# Add Box2D without building its examples and tests
set(BOX2D_DIR ${CMAKE_SOURCE_DIR}/extern/box2d)
set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "Do not build Box2D examples" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Do not build Box2D testbed" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build Box2D unit tests" FORCE)
add_subdirectory(${BOX2D_DIR})

#Add nativefiledialog-extended
# set(NATIVE_FILE_DIALOG_DIR ${CMAKE_SOURCE_DIR}/extern/nativefiledialog-extended)
# add_subdirectory(${NATIVE_FILE_DIALOG_DIR})


# Main executable
add_executable(
    GatorEngine 
    "src/main.cpp"
    "src/Entity.cpp" 
    "src/EntityManager.cpp" 
    "src/AssetManager.cpp"
    "src/Vec2.h"
    "src/editor/Editor.cpp" 
    "src/editor/UIWindow.cpp" 
    "src/editor/FileBarWindow.cpp" 
    "src/editor/TabBarWindow.cpp"
    "src/editor/ExplorerWindow.cpp"
    "src/editor/PropertyWindow.cpp"
    "src/Scene.cpp"
    "src/Scene_Play.cpp"
    "src/GatorPhysics.cpp"
    "src/editor/SceneLayoutWindow.cpp"
    "src/ActionBus.cpp" 
    "src/components/CUserInput.h"
    "src/Animation.cpp"
    "src/components/CAnimation.cpp"
    "src/components/CSprite.cpp"
    "src/GameEngine.cpp" 
    "src/components/CSprite.h" 
    "src/components/includes.h" 
    "src/components/CRigidBody.h" 
    "src/lua_interpreter/LuaManager.cpp")

target_link_libraries(GatorEngine
    libluajit
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    ImGui-SFML::ImGui-SFML
    box2d
    nfd
    libluajit
)

# Set the include directories for the GatorEngine target
target_include_directories(GatorEngine PRIVATE ${SFML_INCLUDE_DIR} ${IMGUI_DIR} ${IMGUI_SFML_DIR})

# Copy asset folder
add_custom_command(TARGET GatorEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:GatorEngine>/assets)


if (WIN32)
        # Copy SFML DLLs
        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            $<TARGET_FILE_DIR:sfml-graphics>
            $<TARGET_FILE_DIR:GatorEngine>)

        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            $<TARGET_FILE_DIR:sfml-audio>
            $<TARGET_FILE_DIR:GatorEngine>)

        #Copy OpenAL dlls
        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/extern/SFML/extlibs/bin/x64/openal32.dll
            $<TARGET_FILE_DIR:GatorEngine>)

        # Copy Box2D DLLs
        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            $<TARGET_FILE_DIR:box2d>
            $<TARGET_FILE_DIR:GatorEngine>)

        # Copy ImGui DLLs
        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            $<TARGET_FILE_DIR:ImGui-SFML::ImGui-SFML>
            $<TARGET_FILE_DIR:GatorEngine>)

        # Copy Luajit DLLs
        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/extern/luajit.dll
            $<TARGET_FILE_DIR:GatorEngine>)

        # Copy lua scripts
        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/lua_scripts
            $<TARGET_FILE_DIR:GatorEngine>)

endif()



# Copy asset folder
add_custom_command(TARGET GatorEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:GatorEngine>/assets)