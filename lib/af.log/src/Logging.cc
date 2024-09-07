// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

module;

#include <format>
#include <iostream>

export module sdv.af.log:Logging;

export namespace sdv::af {

void log(std::string message)
{
    std::cout << message << std::endl;
}

template<typename... Args> void info(std::format_string<Args...> format, Args &&...args)
{
    log(std::format(format, std::forward<Args>(args)...));
}

} // namespace sdv::af
