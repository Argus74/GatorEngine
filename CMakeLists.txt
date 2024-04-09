cmake_minimum_required(VERSION 3.18)
project(GatorEngine)

set(CMAKE_CXX_STANDARD 17)
set(EXTERNALS_DIR ${PROJECT_SOURCE_DIR}/extern)
set(LUA_DIR ${EXTERNALS_DIR}/lua)
set(SOL_2_DIR ${EXTERNALS_DIR}/sol)

add_library(
    lua STATIC
    ${LUA_DIR}/lapi.c
    ${LUA_DIR}/lauxlib.c
    ${LUA_DIR}/lbaselib.c
    ${LUA_DIR}/lcode.c
    ${LUA_DIR}/lcorolib.c
    ${LUA_DIR}/lctype.c
    ${LUA_DIR}/ldblib.c
    ${LUA_DIR}/ldebug.c
    ${LUA_DIR}/ldo.c
    ${LUA_DIR}/ldump.c
    ${LUA_DIR}/lfunc.c
    ${LUA_DIR}/lgc.c
    ${LUA_DIR}/linit.c
    ${LUA_DIR}/liolib.c
    ${LUA_DIR}/llex.c
    ${LUA_DIR}/lmathlib.c
    ${LUA_DIR}/lmem.c
    ${LUA_DIR}/loadlib.c
    ${LUA_DIR}/lobject.c
    ${LUA_DIR}/lopcodes.c
    ${LUA_DIR}/loslib.c
    ${LUA_DIR}/lparser.c
    ${LUA_DIR}/lstate.c
    ${LUA_DIR}/lstring.c
    ${LUA_DIR}/lstrlib.c
    ${LUA_DIR}/ltable.c
    ${LUA_DIR}/ltablib.c
    ${LUA_DIR}/ltm.c
    ${LUA_DIR}/lua.c
    ${LUA_DIR}/lundump.c
    ${LUA_DIR}/lutf8lib.c
    ${LUA_DIR}/lvm.c
    ${LUA_DIR}/lzio.c
)

include(FetchContent)

#[[
FetchContent_Declare(luajit
    GIT_REPOSITORY https://github.com/WohlSoft/LuaJIT.git
    GIT_TAG        879f3976f3657c11464206b2d49e21accdfb62bc
)
FetchContent_MakeAvailable(luajit)
]]


# Add SFML
set(SFML_DIR ${CMAKE_SOURCE_DIR}/extern/SFML)
add_subdirectory(${SFML_DIR})

FetchContent_Declare(
    nativefiledialog-extended
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG        17b6e8ce219c0677f94b63636abb9296b28841ca
)
FetchContent_MakeAvailable(nativefiledialog-extended)

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
    "src/lua_interpreter/LuaState.cpp"
    )

target_link_libraries(GatorEngine
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    ImGui-SFML::ImGui-SFML
    box2d
    nfd
    lua
)

# Set the include directories for the GatorEngine target
target_include_directories(GatorEngine PRIVATE 
${SFML_INCLUDE_DIR} 
${IMGUI_DIR} 
${IMGUI_SFML_DIR}
PRIVATE ${EXTERNALS_DIR}
PRIVATE ${LUA_DIR} # as of sol2 3.2.2, lua.hpp must be accesible through root include '#include <lua.hpp>'
)

set_property(
    TARGET GatorEngine
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${APP_SOURCE_DIR}"
)
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

        add_custom_command(TARGET GatorEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/extern/nfd.dll
            $<TARGET_FILE_DIR:GatorEngine>)

endif()



# Copy asset folder
add_custom_command(TARGET GatorEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:GatorEngine>/assets)