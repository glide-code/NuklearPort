cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(NuklearPort CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(SDL3 REQUIRED)
find_package(blend2d REQUIRED)

add_executable(NuklearPort
    test/main.cpp
    backends/nuklear_sdl3.cpp
    backends/nuklear_blend2d.cpp
    )

target_include_directories(NuklearPort PUBLIC
    test
    backends
    nuklear
    )

target_compile_definitions(NuklearPort PUBLIC
    BL_STAIC)

# Add Blend2D dependency to your application. The dependency
# should provide everything needed - include paths, libraries,
# compiler flags, and compiler definitions.
target_link_libraries(NuklearPort blend2d::blend2d)
target_link_libraries(NuklearPort SDL3::SDL3)
target_link_libraries(NuklearPort user32 kernel32)