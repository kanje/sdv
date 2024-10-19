// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

import sdv.af.app;
import sdv.af.test.gtest;

#include <gmock/gmock.h>

#include <coroutine>

using namespace sdv::af;
using namespace sdv::af::app;
using namespace sdv::af::test;

class MyService final : public Service
{
public:
    auto run(std::function<int()> onRun) -> int
    {
        setRun([onRun = std::move(onRun)] {
            processEvents();
            onRun();
        });
        return Service::run();
    }

    auto run() -> int
    {
        return run([] { return 0; });
    }

public:
    int cntInitialized{0};

private:
    auto initialize() -> Async<int> override
    {
        info("Inside test's initialize()");
        cntInitialized += 1;
        co_return 0;
    }

private:
    std::function<int()> m_onRun;
};

class ServiceTest : public gtest::Test
{
};

TEST_F(ServiceTest, run)
{
    MyService app;
    app.run([&app] {
        info("Inside an event loop");
        EXPECT_EQ(1, app.cntInitialized);
        return 0;
    });
}
