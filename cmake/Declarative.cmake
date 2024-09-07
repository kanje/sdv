# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

function(sdv_add_library NAME)
    cmake_parse_arguments("" "" "" "SOURCES;TEST_SOURCES" ${ARGN})
    add_library(${NAME} SHARED)
    target_sources(${NAME}
      PUBLIC
        FILE_SET CXX_MODULES FILES
          ${_SOURCES}
    )

    if (_TEST_SOURCES)
        add_executable(ut${NAME} ${_TEST_SOURCES})
        target_link_libraries(ut${NAME} ${NAME} AfTestGTest)
        gtest_discover_tests(ut${NAME} TEST_PREFIX ${NAME}.)
    endif()
endfunction()
