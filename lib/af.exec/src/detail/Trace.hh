// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

#pragma once

import sdv.af.log;

#include <format>
#include <map>
#include <string>
#include <string_view>

namespace sdv::af::detail {

#define trace_instance sdv::af::detail::Trace::instance

#define trace(...) trace_instance().print(this, __FUNCTION__, ##__VA_ARGS__)
#define trace_ctr(...)                                                                             \
    do {                                                                                           \
        trace_instance().record(__FUNCTION__, this);                                               \
        trace(__VA_ARGS__);                                                                        \
    } while (false)
#define trace_alias(ptr) trace_instance().alias(ptr)

class Trace
{
public:
    static auto instance() -> Trace &
    {
        static Trace trace;
        return trace;
    }

    auto alias(void const *ptr) -> std::string
    {
        auto it = m_aliases.find(ptr);
        if (it == m_aliases.cend()) {
            return std::format("[{}]", ptr);
        }
        return it->second;
    }

    auto record(std::string_view name, void const *ptr) -> std::string_view
    {
        m_aliases[ptr] = std::format("{}<{}>", name, ++m_next[name]);
        return m_aliases[ptr];
    }

    void print(void const *ptr, std::string_view function)
    {
        info("-- {:>20} | {}", alias(ptr), function);
    }

    template<typename... Args>
    void print(void const *ptr, std::string_view function, std::format_string<Args...> format,
               Args &&...args)
    {
        info("-- {:>20} | {}: {}", alias(ptr), function,
             std::format(format, std::forward<Args>(args)...));
    }

private:
    std::map<void const *, std::string> m_aliases;
    std::map<std::string_view, int> m_next;
};

} // namespace sdv::af::detail
