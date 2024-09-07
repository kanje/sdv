// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <deque>
#include <memory>

export module sdv.af.test:TestEngine;

import sdv.af.exec;

namespace sdv::af::test {

export class TestExecutor : public detail::BaseExecutor
{
public:
    void post(Work work) noexcept override;

public:
    void spin() noexcept;

private:
    std::deque<Work> m_work;
};

void TestExecutor::post(Work work) noexcept
{
    m_work.push_back(std::move(work));
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
    auto executor() noexcept -> std::unique_ptr<detail::BaseExecutor> override;
};

auto TestEngine::executor() noexcept -> std::unique_ptr<detail::BaseExecutor>
{
    return std::make_unique<TestExecutor>();
}

} // namespace sdv::af::test
