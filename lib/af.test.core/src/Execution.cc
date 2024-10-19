// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <deque>
#include <functional>
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
    void setRun(std::function<void()> run) noexcept;
    void spin() noexcept;

private:
    std::function<void()> m_run;
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
    assert(m_run != nullptr, "Run action is not set");
    m_run();
}

void TestExecutor::stop()
{
    std::abort();
}

void TestExecutor::setRun(std::function<void()> run) noexcept
{
    m_run = std::move(run);
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

TestEngine::TestEngine() noexcept
    : detail::BaseEngine("TEST")
{}

auto TestEngine::executor() noexcept -> std::unique_ptr<detail::BaseExecutor>
{
    return std::make_unique<TestExecutor>();
}

export void setRun(std::function<void()> run) noexcept
{
    TestExecutor::instance->setRun(std::move(run));
}

export void processEvents() noexcept
{
    TestExecutor::instance->spin();
}

} // namespace sdv::af::test
