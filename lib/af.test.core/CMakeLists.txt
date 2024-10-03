# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

sdv_add_library(AfTestCore
  MODULE_SOURCES
    src/Main.cc
    src/Execution.cc

  TEST_SOURCES
    test/ExecutionTest.cc
)

target_link_libraries(AfTestCore
  PUBLIC
    AfExec
)
