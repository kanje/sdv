// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

import sdv.af.exec;
import sdv.af.log;
import sdv.af.test.gtest;

#include <gmock/gmock.h>

#include <coroutine>

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

class Awaitable
{
public:
    struct State
    {
        int value{0};
        std::coroutine_handle<> awaiter;
    };

public:
    Awaitable(std::shared_ptr<State> state)
        : m_state(std::move(state))
    {}

    auto await_ready() const -> bool
    {
        return false;
    }

    auto await_suspend(std::coroutine_handle<> awaiter) -> void
    {
        m_state->awaiter = awaiter;
    }

    auto await_resume() -> int
    {
        return m_state->value;
    }

private:
    std::shared_ptr<State> m_state;
};

auto delayedValue() -> Awaitable
{
    auto state = std::make_shared<Awaitable::State>();
    Executor::thisThread().post([state] {
        state->value = 42;
        state->awaiter.resume();
    });
    return {state};
}

auto third() -> Async<int>
{
    info("third(): before co_await");
    int i = co_await delayedValue();
    info("third(): after co_await; i={}", i);
    co_return i - 2;
}

auto second() -> Async<int>
{
    info("second(): before co_await");
    int i = co_await third();
    info("second(): after co_await; i={}", i);
    co_return i * 2;
}

auto first() -> Async<int>
{
    info("first(): before co_await");
    int i = co_await second();
    info("first(): after co_await; i={}", i);
    co_return i + 2;
}

TEST_F(ExecutionTest, async)
{
    info("main(): before");
    auto co = first();
    info("main(): coroutines suspended");
    processEvents();
    info("main(): coroutines comleted");
    int i = co.value();
    info("main(): done; i={}", i);
    EXPECT_EQ(82, i);
}

/* OUTPUT:

main(): before
--     CoPromise<1> | CoPromise: co_handle<1>
--     CoPromise<1> | get_return_object()
--         Async<1> | Async()
--     CoPromise<1> | initial_suspend: never
first(): before co_await
--     CoPromise<2> | CoPromise: co_handle<2>
--     CoPromise<2> | get_return_object()
--         Async<2> | Async()
--     CoPromise<2> | initial_suspend: never
second(): before co_await
--     CoPromise<3> | CoPromise: co_handle<3>
--     CoPromise<3> | get_return_object()
--         Async<3> | Async()
--     CoPromise<3> | initial_suspend: never
third(): before co_await
--         Async<3> | await_ready()
--         Async<3> | await_suspend: co_handle<2>
--         Async<2> | await_ready()
--         Async<2> | await_suspend: co_handle<1>
main(): coroutines suspended
third(): after co_await; i=42
--     CoPromise<3> | return_value: 40
--     CoPromise<3> | final_suspend()
-- ResumeAwaiter<1> | ResumeAwaiter()
-- ResumeAwaiter<1> | await_ready()
-- ResumeAwaiter<1> | await_suspend: co_handle<3>
--         Async<3> | await_resume: 40
--         Async<3> | ~Async()
-- ResumeAwaiter<1> | ~ResumeAwaiter()
--     CoPromise<3> | ~CoPromise()
second(): after co_await; i=40
--     CoPromise<2> | return_value: 80
--     CoPromise<2> | final_suspend()
-- ResumeAwaiter<2> | ResumeAwaiter()
-- ResumeAwaiter<2> | await_ready()
-- ResumeAwaiter<2> | await_suspend: co_handle<2>
--         Async<2> | await_resume: 80
--         Async<2> | ~Async()
-- ResumeAwaiter<2> | ~ResumeAwaiter()
--     CoPromise<2> | ~CoPromise()
first(): after co_await; i=80
--     CoPromise<1> | return_value: 82
--     CoPromise<1> | final_suspend()
-- ResumeAwaiter<3> | ResumeAwaiter()
-- ResumeAwaiter<3> | await_ready()
-- ResumeAwaiter<3> | await_suspend: co_handle<1>
main(): coroutines comleted
--         Async<1> | value: 82
main(): done; i=82
--         Async<1> | ~Async()
-- ResumeAwaiter<3> | ~ResumeAwaiter()
--     CoPromise<1> | ~CoPromise()

*/
