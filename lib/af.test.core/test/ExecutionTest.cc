// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

import sdv.af.exec;
import sdv.af.log;
import sdv.af.test.gtest;

#include <gmock/gmock.h>

using namespace sdv::af;
using namespace sdv::af::test;

class ExecutionTest : public gtest::Test
{
};

TEST_F(ExecutionTest, equalExecutors)
{
    EXPECT_EQ(Executor::thisThread(), Executor::thisThread());
    EXPECT_EQ(Executor::mainThread(), Executor::mainThread());
    EXPECT_EQ(Executor::thisThread(), Executor::mainThread());
}

TEST_F(ExecutionTest, equalExecutors_nextSeqNr)
{
    EXPECT_EQ(Executor::thisThread(), Executor::thisThread());
    EXPECT_EQ(Executor::mainThread(), Executor::mainThread());
    EXPECT_EQ(Executor::thisThread(), Executor::mainThread());
}

TEST_F(ExecutionTest, post)
{
    auto executor = Executor::thisThread();

    bool isInvoked{false};
    executor.post([&isInvoked] { isInvoked = true; });

    EXPECT_FALSE(isInvoked);

    processEvents();
    EXPECT_TRUE(isInvoked);
}
