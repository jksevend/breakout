#include <iostream>

#include "spdlog/spdlog.h"
#include "src/Application.h"

auto main(int argc, char** argv) -> int
{
    try
    {
        const auto application = std::make_unique<Breakout::Application>();
        application->run();
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error: {}", e.what());
        return EXIT_FAILURE;
    }
}
