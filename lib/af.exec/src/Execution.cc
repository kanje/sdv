// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <boost/core/noncopyable.hpp>

#include <functional>
#include <memory>

export module sdv.af.exec:Execution;

namespace sdv::af {

export using Work = std::move_only_function<void()>;
export class Executor;

export namespace detail {

class BaseExecutor : boost::noncopyable
{
public:
    virtual ~BaseExecutor() = default;
    virtual void post(Work work) noexcept = 0;
};

class BaseEngine : boost::noncopyable
{
public:
    virtual ~BaseEngine() = default;
    virtual auto executor() noexcept -> std::unique_ptr<BaseExecutor> = 0;
};

} // namespace detail

class Executor final : boost::noncopyable
{
public:
    static auto thisThread() noexcept -> Executor;
    static auto mainThread() noexcept -> Executor;

public:
    void post(Work work) noexcept;

private:
    explicit Executor(detail::BaseExecutor *impl);

private:
    detail::BaseExecutor *m_impl;
};

auto Executor::thisThread() noexcept -> Executor
{
    return Executor{nullptr};
}

auto Executor::mainThread() noexcept -> Executor
{
    return Executor{nullptr};
}

Executor::Executor(detail::BaseExecutor *impl)
    : m_impl(impl)
{}

void Executor::post(Work work) noexcept
{
    m_impl->post(std::move(work));
}

} // namespace sdv::af
