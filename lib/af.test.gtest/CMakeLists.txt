# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

find_package(GTest CONFIG REQUIRED COMPONENTS GTest GMock)

sdv_add_library(AfTestGTest
  MODULE_SOURCES
    src/Main.cc
    src/Test.cc

  PUBLIC_DEPS
    AfTestCore
    GTest::gmock
    GTest::gtest_main
)
