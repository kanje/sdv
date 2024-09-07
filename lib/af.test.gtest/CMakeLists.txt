# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

find_package(GTest CONFIG REQUIRED COMPONENTS GTest GMock)

sdv_add_library(AfTestGTest
  SOURCES
    src/Main.cc
    src/Test.cc

  TEST_SOURCES
)

target_link_libraries(AfTestGTest
  PUBLIC
    AfTestCore
    GTest::gmock
    GTest::gtest_main
)
