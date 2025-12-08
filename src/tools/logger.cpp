#include "logger.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace vsa::tools {

namespace {

spdlog::level::level_enum convert(const Logger::Level lvl)
{
    switch (lvl) {
        case Logger::Level::DEBUG: return spdlog::level::debug;
        case Logger::Level::INFO: return spdlog::level::info;;
        case Logger::Level::WARNING: return spdlog::level::warn;;
        case Logger::Level::ERROR: return spdlog::level::err;;
    }
    return spdlog::level::trace;
}

}

Logger& vsa::tools::Logger::get_instance(const std::string& name)
{
    static std::mutex mutex;
    static std::map<std::string, Logger> loggers;

    std::lock_guard lg(mutex);

    const auto it = loggers.find(name);
    if (it == loggers.end()) {
        if (auto [fst, snd] = loggers.emplace(name, Logger(name)); snd) { return fst->second; }
        exit(444);
    }
    return it->second;
}

Logger::Logger(std::string name) : m_name(std::move(name))
{
    m_logger = spdlog::stdout_color_mt(m_name);
}

void Logger::log(const Level lvl, const std::string& msg) const {
    m_logger->log(convert(lvl), msg);
}

}
