# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

sdv_add_library(AfApp
  MODULE_SOURCES
    src/Main.ccm
    src/Service.ccm

  TEST_SOURCES
    test/ServiceTest.cc

  PUBLIC_DEPS
    AfExecAsio
)
