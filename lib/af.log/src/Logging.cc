// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <format>
#include <iostream>

export module sdv.af.log:Logging;

export namespace sdv::af {

template<typename... Args>
void info(std::format_string<Args...> format, Args &&...args)
{
    std::println(std::cout, std::move(format), std::forward<Args>(args)...);
}

template<typename... Args>
void fatal(std::format_string<Args...> format, Args &&...args)
{
    std::println(std::cout, std::move(format), std::forward<Args>(args)...);
}

} // namespace sdv::af
