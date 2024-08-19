# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

sdv_add_library(AfCore
  SOURCES
    src/Execution.cc
    src/Logging.cc
    src/Module.cc

  TEST_SOURCES
    test/ExecutionTest.cc
    test/LoggingTest.cc
)
