// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <gmock/gmock.h>

export module sdv.af.test.gtest:Test;

export import sdv.af.test;

namespace sdv::af::test::gtest {

export class Test : public ::testing::Test
{
private:
    TestEngine m_engine;
};

} // namespace sdv::af::test::gtest
