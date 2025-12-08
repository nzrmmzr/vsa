#include "tools/logger.hpp"

#include <csignal>
#include <memory>

#include <tools/logger.hpp>
#include <village_app.hpp>

std::unique_ptr<vsa::VillageApp> app;

void signal_handler(int signal)
{
    VSA_LOG_WARNING("system", "Signal {} received, shutting down..", signal);
    if (app) { app->stop(); }
}

int main(int, char**)
{
    app = std::make_unique<vsa::VillageApp>();

    std::signal(SIGABRT, signal_handler);
    std::signal(SIGFPE, signal_handler);
    std::signal(SIGILL, signal_handler);
    std::signal(SIGINT, signal_handler);
    std::signal(SIGSEGV, signal_handler);
    std::signal(SIGTERM, signal_handler);

    app->init();
    app->run();
    app->deinit();

    return 0;
}