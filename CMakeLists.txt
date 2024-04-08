cmake_minimum_required(VERSION 3.20.0)

set (NAME "HitAndBlow")
project(${NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "platform windows only")
    return()
endif()


file(GLOB CODES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Common/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Common/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Direct12/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Direct12/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Input/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Input/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Photon/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Photon/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Vulkan/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Vulkan/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/XAudio2/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/XAudio2/*.cpp"
)

set(VULKAN "$ENV{VULKAN_SDK}")
if(NOT VULKAN)
    set(VULKAN "$ENV{VK_SDK_PATH}")
endif()

if(NOT VULKAN)
    return()
endif()

add_definitions(-D NOMINMAX)

add_executable(${NAME} WIN32 ${CODES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ThirdLib")
include_directories("${VULKAN}/include")
link_directories("${VULKAN}/lib")
target_compile_options(${NAME} PRIVATE /W3 /sdl /MP)
add_compile_options("$<$<CXX_COMPILE_ID:MSVC>:/utf-8>")