// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <boost/core/noncopyable.hpp>

#include <functional>
#include <memory>

export module sdv.af.exec:Execution;

import sdv.af.log;
import :Assert;

namespace sdv::af {

export using Work = std::move_only_function<void()>;
export class Executor;

export namespace detail {

class BaseExecutor : boost::noncopyable
{
public:
    virtual ~BaseExecutor() = default;
    virtual void post(Work work) noexcept = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};

class BaseEngine : boost::noncopyable
{
public:
    static int seqNr;
    static BaseEngine *instance;

public:
    virtual ~BaseEngine();
    virtual auto executor() noexcept -> std::unique_ptr<BaseExecutor> = 0;

protected:
    BaseEngine(std::string_view name) noexcept;
};

int BaseEngine::seqNr;
BaseEngine *BaseEngine::instance;

BaseEngine::BaseEngine(std::string_view name) noexcept
{
    assert(instance == nullptr, "Another execution engine is already instantiated");

    static int nextSeqNr{1};
    seqNr = nextSeqNr++;
    instance = this;

    info("Initialized {} execution engine (seqNr={})", name, seqNr);
}

BaseEngine::~BaseEngine()
{
    instance = nullptr;
    info("Terminated execution engine (seqNr={})", seqNr);
}

class ThisThreadExecutor
{
public:
    static auto lock() noexcept -> BaseExecutor *;

private:
    ThisThreadExecutor() noexcept;

private:
    int m_seqNr;
    std::unique_ptr<BaseExecutor> m_executor;
};

ThisThreadExecutor::ThisThreadExecutor() noexcept
    : m_seqNr(BaseEngine::seqNr)
    , m_executor(BaseEngine::instance->executor())
{}

auto ThisThreadExecutor::lock() noexcept -> BaseExecutor *
{
    thread_local ThisThreadExecutor self;

    if (BaseEngine::seqNr != self.m_seqNr) [[unlikely]] {
        self.m_executor.reset();
        self = ThisThreadExecutor();
    }

    return self.m_executor.get();
}

class MainThreadExecutor
{
public:
    static auto lock() noexcept -> BaseExecutor *;

private:
    MainThreadExecutor() noexcept;

private:
    int m_seqNr;
    BaseExecutor *m_executor;
};

MainThreadExecutor::MainThreadExecutor() noexcept
    : m_seqNr(BaseEngine::seqNr)
    , m_executor(ThisThreadExecutor::lock())
{}

auto MainThreadExecutor::lock() noexcept -> BaseExecutor *
{
    static MainThreadExecutor self;

    if (BaseEngine::seqNr != self.m_seqNr) [[unlikely]] {
        self = MainThreadExecutor();
    }

    return self.m_executor;
}

} // namespace detail

class Executor final : boost::noncopyable
{
public:
    static auto thisThread() noexcept -> Executor;
    static auto mainThread() noexcept -> Executor;

public:
    bool operator==(Executor const &rhs) const noexcept;

public:
    void post(Work work) noexcept;
    int run();
    void stop();

private:
    explicit Executor(detail::BaseExecutor *impl);

private:
    detail::BaseExecutor *m_impl;
};

auto Executor::thisThread() noexcept -> Executor
{
    return Executor{detail::ThisThreadExecutor::lock()};
}

auto Executor::mainThread() noexcept -> Executor
{
    return Executor{detail::MainThreadExecutor::lock()};
}
Executor::Executor(detail::BaseExecutor *impl)
    : m_impl(impl)
{}

bool Executor::operator==(Executor const &rhs) const noexcept
{
    return m_impl == rhs.m_impl;
}

void Executor::post(Work work) noexcept
{
    m_impl->post(std::move(work));
}

int Executor::run()
{
    m_impl->run();
    return 0;
}

void Executor::stop()
{
    m_impl->stop();
}

} // namespace sdv::af
