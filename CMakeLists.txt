cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0091 NEW)
project(grog VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(CURL REQUIRED COMPONENTS HTTPS HTTP HTTP2)

add_subdirectory(3rdParty/DPF)
add_subdirectory(3rdParty/json)
add_subdirectory(3rdParty/miniz)
add_subdirectory(3rdParty/vcl-graph)

dpf_add_plugin(grog-synth
    TARGETS clap lv2 vst2 vst3 jack
    FILES_COMMON
        src/Common/AtomicSharedPtr.cpp
        src/Common/AtomicSharedPtr.hpp
    FILES_DSP
        src/DSP/GrogPlugin.cpp
        src/DSP/GrogPlugin.hpp
    FILES_UI
        src/UI/Application.cpp
        src/UI/Application.hpp
        src/UI/Config.cpp
        src/UI/Config.hpp
        src/UI/Event.cpp
        src/UI/Event.hpp
        src/UI/Filesystem.cpp
        src/UI/Filesystem.hpp
        src/UI/ImguiUI.cpp
        src/UI/ImguiUI.hpp
        src/UI/Module.cpp
        src/UI/Module.hpp

        src/UI/Modules/AsyncResourceManager.hpp
        src/UI/Modules/AsyncResourceManager.cpp
        src/UI/Modules/Logger.hpp
        src/UI/Modules/Logger.cpp
        src/UI/Modules/ModalWindowManager.hpp
        src/UI/Modules/ModalWindowManager.cpp
    UI_TYPE opengl)

dpf_add_plugin(grog-effect
    TARGETS clap lv2 vst2 vst3 jack
    FILES_COMMON
        src/Common/AtomicSharedPtr.cpp
        src/Common/AtomicSharedPtr.hpp
    FILES_DSP
        src/DSP/GrogPlugin.cpp
        src/DSP/GrogPlugin.hpp
    FILES_UI
        src/UI/Application.cpp
        src/UI/Application.hpp
        src/UI/Config.cpp
        src/UI/Config.hpp
        src/UI/Event.cpp
        src/UI/Event.hpp
        src/UI/Filesystem.cpp
        src/UI/Filesystem.hpp
        src/UI/ImguiUI.cpp
        src/UI/ImguiUI.hpp
        src/UI/Module.cpp
        src/UI/Module.hpp

        src/UI/Modules/AsyncResourceManager.hpp
        src/UI/Modules/AsyncResourceManager.cpp
        src/UI/Modules/Logger.hpp
        src/UI/Modules/Logger.cpp
        src/UI/Modules/ModalWindowManager.hpp
        src/UI/Modules/ModalWindowManager.cpp
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

target_link_libraries(grog-synth-ui PRIVATE grog-imgui vclg nlohmann_json ${CURL_LIBRARIES} miniz)
target_link_libraries(grog-effect-ui PRIVATE grog-imgui vclg nlohmann_json ${CURL_LIBRARIES} miniz)

target_link_libraries(grog-synth-dsp PRIVATE vclg)
target_link_libraries(grog-effect-dsp PRIVATE vclg)