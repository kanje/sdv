// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include "detail/Trace.hh"

#include <boost/core/noncopyable.hpp>

#include <coroutine>
#include <format>
#include <map>
#include <string>
#include <string_view>

export module sdv.af.exec:Coroutines;

import sdv.af.log;

namespace sdv::af {

export template<typename T>
class Async;

namespace detail {

struct ResumeAwaiter;

template<typename T>
struct CoPromise : boost::noncopyable
{
    CoPromise();
    ~CoPromise();

    auto get_return_object() -> Async<T>;
    auto initial_suspend() -> std::suspend_never;
    auto final_suspend() noexcept -> ResumeAwaiter;
    auto return_value(T v) -> void;
    void unhandled_exception();

    T value;
    std::coroutine_handle<CoPromise> awaiter;
};

struct ResumeAwaiter
{
    ResumeAwaiter();
    ~ResumeAwaiter();

    auto await_ready() const noexcept -> bool;

    template<typename T>
    auto await_suspend(std::coroutine_handle<CoPromise<T>> self) -> std::coroutine_handle<>;

    auto await_resume() noexcept -> void;
};

template<typename T>
CoPromise<T>::CoPromise()
{
    trace_ctr("{}",
              trace_instance().record(
                  "co_handle", std::coroutine_handle<CoPromise>::from_promise(*this).address()));
}

template<typename T>
CoPromise<T>::~CoPromise()
{
    trace();
}

template<typename T>
auto CoPromise<T>::get_return_object() -> Async<T>
{
    trace();
    return {std::coroutine_handle<CoPromise>::from_promise(*this)};
}

template<typename T>
auto CoPromise<T>::initial_suspend() -> std::suspend_never
{
    trace("never");
    return {};
}

template<typename T>
auto CoPromise<T>::final_suspend() noexcept -> ResumeAwaiter
{
    trace();
    return {};
}

template<typename T>
auto CoPromise<T>::return_value(T v) -> void
{
    trace("{}", v);
    value = v;
}

template<typename T>
auto CoPromise<T>::unhandled_exception() -> void
{
    trace();
}

ResumeAwaiter::ResumeAwaiter()
{
    trace_ctr();
}

ResumeAwaiter::~ResumeAwaiter()
{
    trace();
}

auto ResumeAwaiter::await_ready() const noexcept -> bool
{
    trace();
    return false;
}

template<typename T>
auto ResumeAwaiter::await_suspend(std::coroutine_handle<CoPromise<T>> self)
    -> std::coroutine_handle<>
{
    trace("{}", trace_alias(self.address()));
    if (auto awaiter = self.promise().awaiter; awaiter) {
        return awaiter;
    }
    return std::noop_coroutine();
}

auto ResumeAwaiter::await_resume() noexcept -> void
{
    trace();
    std::abort();
}

} // namespace detail

template<typename T>
class Async : boost::noncopyable
{
public:
    using promise_type = detail::CoPromise<T>;

public:
    Async(std::coroutine_handle<promise_type> self);
    ~Async();

    auto await_ready() const -> bool;
    auto await_suspend(std::coroutine_handle<promise_type> awaiter) -> void;
    auto await_resume() -> T;

    auto value() const -> T;

private:
    std::coroutine_handle<promise_type> m_self;
};

template<typename T>
Async<T>::Async(std::coroutine_handle<promise_type> self)
    : m_self(self)
{
    trace_ctr();
}

template<typename T>
Async<T>::~Async()
{
    trace();
    m_self.destroy();
}

template<typename T>
auto Async<T>::await_ready() const -> bool
{
    trace();
    return false;
}

template<typename T>
auto Async<T>::await_suspend(std::coroutine_handle<promise_type> awaiter) -> void
{
    trace("{}", trace_instance().alias(awaiter.address()));
    m_self.promise().awaiter = awaiter;
}

template<typename T>
auto Async<T>::await_resume() -> T
{
    trace("{}", m_self.promise().value);
    return m_self.promise().value;
}

template<typename T>
auto Async<T>::value() const -> T
{
    trace("{}", m_self.promise().value);
    return m_self.promise().value;
}

} // namespace sdv::af
