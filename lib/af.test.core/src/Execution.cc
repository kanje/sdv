// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <deque>
#include <memory>

export module sdv.af.test:TestEngine;

import sdv.af.exec;

namespace sdv::af::test {

class TestExecutor : public detail::BaseExecutor
{
public:
    static TestExecutor *instance;

public:
    TestExecutor();
    ~TestExecutor();

public:
    void post(Work work) noexcept override;
    void run() override;
    void stop() override;

public:
    void spin() noexcept;

private:
    std::deque<Work> m_work;
};

TestExecutor *TestExecutor::instance;

TestExecutor::TestExecutor()
{
    assert(instance == nullptr, "Another test executor already exists.");
    instance = this;
}

TestExecutor::~TestExecutor()
{
    instance = nullptr;
}

void TestExecutor::post(Work work) noexcept
{
    m_work.push_back(std::move(work));
}

void TestExecutor::run()
{
    std::abort();
}

void TestExecutor::stop()
{
    std::abort();
}

void TestExecutor::spin() noexcept
{
    while (!m_work.empty()) {
        auto work = std::move(m_work.front());
        m_work.pop_front();
        work();
    }
}

export class TestEngine : public detail::BaseEngine
{
public:
    TestEngine() noexcept;

private:
    auto executor() noexcept -> std::unique_ptr<detail::BaseExecutor> override;
};

TestEngine::TestEngine() noexcept = default;

auto TestEngine::executor() noexcept -> std::unique_ptr<detail::BaseExecutor>
{
    return std::make_unique<TestExecutor>();
}

export void processEvents() noexcept
{
    TestExecutor::instance->spin();
}

} // namespace sdv::af::test
