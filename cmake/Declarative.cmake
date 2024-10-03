# SDV Ecosystem Playground
# Copyright (c) 2024 Antons Jeļkins

function(sdv_add_library NAME)
    cmake_parse_arguments("" "" "" "MODULE_SOURCES;SOURCES;PUBLIC_DEPS;PRIVATE_DEPS;TEST_SOURCES" ${ARGN})
    add_library(${NAME} SHARED ${_SOURCES})
    target_sources(${NAME}
      PUBLIC
        FILE_SET CXX_MODULES FILES
          ${_MODULE_SOURCES}
    )
    target_link_libraries(${NAME}
      PUBLIC
        ${_PUBLIC_DEPS}
      PRIVATE
        ${_PRIVATE_DEPS}
    )

    if (_TEST_SOURCES)
        add_executable(ut${NAME} ${_TEST_SOURCES})
        target_link_libraries(ut${NAME} ${NAME} AfTestGTest)
        gtest_discover_tests(ut${NAME} TEST_PREFIX ${NAME}.)
    endif()
endfunction()

function(sdv_add_executable NAME)
    cmake_parse_arguments("" "" "" "SOURCES;DEPS" ${ARGN})
    add_executable(${NAME} ${_SOURCES})
    target_link_libraries(${NAME} ${_DEPS})
endfunction()
