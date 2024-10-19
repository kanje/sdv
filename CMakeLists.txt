# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

cmake_minimum_required(VERSION 3.28)
project(SDV VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_compile_options(-Wno-deprecated-declarations -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)
add_link_options(-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)

include(GoogleTest)

include(cmake/Declarative.cmake)

enable_testing()

add_subdirectory(lib)
add_subdirectory(bin)
