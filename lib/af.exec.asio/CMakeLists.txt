# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

find_package(Boost REQUIRED COMPONENTS system)

sdv_add_library(AfExecAsio
  MODULE_SOURCES
    src/Main.ccm
    src/Execution.ccm

  PUBLIC_DEPS
    AfExec

  PRIVATE_DEPS
    Boost::system
)
