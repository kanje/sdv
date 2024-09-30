# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

sdv_add_library(AfExec
  SOURCES
    src/Assert.cc
    src/Coroutines.cc
    src/Execution.cc
    src/Main.cc

  TEST_SOURCES
    test/ExecutionTest.cc
)

target_link_libraries(AfExec
  PUBLIC
    AfLog
)
