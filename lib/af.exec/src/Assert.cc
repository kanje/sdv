// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <format>

export module sdv.af.exec:Assert;

import sdv.af.log;

export namespace sdv::af {

template<typename... Args>
void assert(bool condition, std::format_string<Args...> format, Args &&...args)
{
    if (!condition) {
        fatal(std::move(format), std::forward<Args>(args)...);
        std::abort();
    }
}

} // namespace sdv::af
