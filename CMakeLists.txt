cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0091 NEW)
project(grog VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(3rdParty/DPF)
add_subdirectory(3rdParty/vcl-graph)

dpf_add_plugin(grog-synth
    TARGETS clap lv2 vst2 vst3 jack
    FILES_COMMON
        src/Common/AtomicSharedPtr.hpp
        src/Common/Filesystem.cpp
        src/Common/Filesystem.hpp
    FILES_DSP
        src/DSP/GrogPlugin.cpp
    FILES_UI
        src/UI/ImguiUI.cpp
        src/UI/GrogUI.cpp
    UI_TYPE opengl)

dpf_add_plugin(grog-effect
    TARGETS clap lv2 vst2 vst3 jack
    FILES_COMMON
        src/Common/AtomicSharedPtr.hpp
        src/Common/Filesystem.cpp
        src/Common/Filesystem.hpp
    FILES_DSP
        src/DSP/GrogPlugin.cpp
    FILES_UI
        src/UI/ImguiUI.cpp
        src/UI/GrogUI.cpp
    UI_TYPE opengl)

target_include_directories(grog-synth PUBLIC src)
target_include_directories(grog-effect PUBLIC src)

target_compile_definitions(grog-synth PUBLIC GROG_IS_SYNTH)

# IMGUI

add_library(grog-imgui STATIC 
    3rdParty/imgui/imgui_demo.cpp
    3rdParty/imgui/imgui_draw.cpp
    3rdParty/imgui/imgui_tables.cpp
    3rdParty/imgui/imgui_widgets.cpp
    3rdParty/imgui/imgui.cpp
    3rdParty/imgui/backends/imgui_impl_opengl2.cpp
    3rdParty/imgui/backends/imgui_impl_opengl3.cpp)

target_include_directories(grog-imgui PUBLIC 3rdParty/imgui)

target_link_libraries(grog-synth-ui PRIVATE grog-imgui vclg)
target_link_libraries(grog-effect-ui PRIVATE grog-imgui vclg)

target_link_libraries(grog-synth-dsp PRIVATE vclg)
target_link_libraries(grog-effect-dsp PRIVATE vclg)