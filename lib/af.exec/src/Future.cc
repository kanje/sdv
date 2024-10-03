// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include "detail/Trace.hh"

#include <boost/core/noncopyable.hpp>

#include <coroutine>
#include <memory>

export module sdv.af.exec:Future;

import :Assert;
import :Execution;

namespace sdv::af {

namespace detail {

template<typename T>
struct SharedState
{
    std::optional<T> value;
    std::coroutine_handle<> awaiter;
};

}; // namespace detail

export template<typename T>
class Future;

export template<typename T>
class Promise
{
public:
    Promise() noexcept
        : m_state(std::make_shared<detail::SharedState<T>>())
    {
        trace_ctr();
    }

    Promise(Promise const &rhs)
        : m_state(rhs.m_state)
    {
        trace_ctr("copy from {}", trace_alias(&rhs));
    }

    Promise(Promise &&rhs) noexcept
        : m_state(std::move(rhs.m_state))
    {
        trace_ctr("move from {}", trace_alias(&rhs));
    }

    ~Promise()
    {
        trace();
    }

    void fulfill(T value) const
    {
        trace("{}", value);
        m_state->value = std::move(value);
        if (m_state->awaiter) {
            Executor::thisThread().post([awaiter = m_state->awaiter] { awaiter.resume(); });
        }
    }

    auto future() const -> Future<T>
    {
        trace();
        return {m_state};
    }

private:
    std::shared_ptr<detail::SharedState<T>> m_state;
};

template<typename T>
class Future : boost::noncopyable
{
public:
    ~Future()
    {
        trace();
    }

    auto await_ready() const noexcept -> bool
    {
        trace("{}", m_state->value.has_value());
        return m_state->value.has_value();
    }

    auto await_suspend(std::coroutine_handle<> awaiter) -> void
    {
        trace("{}", trace_alias(awaiter.address()));
        m_state->awaiter = awaiter;
    }

    auto await_resume() -> T
    {
        assert(m_state->value.has_value(), "Future does not have a value");
        trace("{}", *m_state->value);
        return *m_state->value;
    }

private:
    friend Promise<T>;

    Future(std::shared_ptr<detail::SharedState<T>> state) noexcept
        : m_state(std::move(state))
    {
        trace_ctr();
    }

private:
    std::shared_ptr<detail::SharedState<T>> m_state;
};

} // namespace sdv::af
