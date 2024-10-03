# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

sdv_add_library(AfLog
  MODULE_SOURCES
    src/Logging.cc
    src/Main.cc

  TEST_SOURCES
    test/LoggingTest.cc
)
