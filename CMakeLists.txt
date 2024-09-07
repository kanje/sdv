# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

cmake_minimum_required(VERSION 3.28)
project(SDV VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)

include(cmake/Declarative.cmake)

enable_testing()

add_subdirectory(lib)
