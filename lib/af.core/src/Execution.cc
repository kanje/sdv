// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <boost/core/noncopyable.hpp>
#include <functional>
#include <memory>

export module sdv.af:Execution;

namespace sdv::af {

export using Work = std::move_only_function<void()>;
export class Executor;

namespace detail {

class BaseExecutor {
public:
  virtual ~BaseExecutor() = default;
  virtual void post(Work work) noexcept = 0;
};

class BaseEngine {
public:
  virtual std::unique_ptr<BaseExecutor> executor() noexcept = 0;
};

} // namespace detail

class Executor final : boost::noncopyable {
public:
  static Executor thisThread() noexcept;
  static Executor mainThread() noexcept;

public:
  void post(Work work) noexcept;

private:
  explicit Executor(detail::BaseExecutor *impl);

private:
  detail::BaseExecutor *m_impl;
};

Executor Executor::thisThread() noexcept { return Executor{nullptr}; }

Executor Executor::mainThread() noexcept { return Executor{nullptr}; }

Executor::Executor(detail::BaseExecutor *impl) : m_impl(impl) {}

void Executor::post(Work work) noexcept { m_impl->post(std::move(work)); }

} // namespace sdv::af
