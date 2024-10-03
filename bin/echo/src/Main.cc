// SDV Ecosystem Playground
// Copyright (c) 2024 Antons Jeļkins

import sdv.af.exec.asio;

using namespace sdv;

auto main() -> int
{
    af::asio::Engine engine;

    auto executor = af::Executor::thisThread();
    executor.post([&executor] { executor.stop(); });

    executor.run();
}
