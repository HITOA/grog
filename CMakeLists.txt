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

dpf_add_plugin(Grog
    TARGETS clap lv2 vst2 vst3 jack
    FILES_DSP
        src/GrogPlugin.cpp
    FILES_UI
        src/GrogUI.cpp)

target_include_directories(Grog PUBLIC src)