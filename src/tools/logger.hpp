#pragma once

#include <string>

#include <spdlog/spdlog.h>

namespace vsa::tools {

class Logger
{
public:
    static Logger& get_instance(const std::string& name);

private:
    explicit Logger(std::string name);

public:
    enum class Level { DEBUG, INFO, WARNING, ERROR };

    void log(Level lvl, const std::string& msg) const;

    template <typename... Args>
    void log(const Level lvl, spdlog::format_string_t<Args...> fmt, Args &&...args) const
    {
        spdlog::level::level_enum spd_lvl = spdlog::level::trace;
        switch (lvl) {
            case Level::DEBUG:
                spd_lvl = spdlog::level::debug;
                break;
            case Level::INFO:
                spd_lvl = spdlog::level::info;
                break;
            case Level::WARNING:
                spd_lvl = spdlog::level::warn;
                break;
            case Level::ERROR:
                spd_lvl = spdlog::level::err;
                break;
        }

        m_logger->log(spd_lvl, fmt, std::forward<Args>(args)...);
    }

private:
    const std::string m_name;
    std::shared_ptr<spdlog::logger> m_logger;
};

}

#define VSA_LOGGER_CALL(logger, level, ...) \
(logger).log(level, __VA_ARGS__)

#define VSA_LOG_DEBUG(name, ...) \
VSA_LOGGER_CALL(vsa::tools::Logger::get_instance(name), vsa::tools::Logger::Level::DEBUG, __VA_ARGS__);

#define VSA_LOG_INFO(name, ...) \
VSA_LOGGER_CALL(vsa::tools::Logger::get_instance(name), vsa::tools::Logger::Level::INFO, __VA_ARGS__);

#define VSA_LOG_WARNING(name, ...) \
VSA_LOGGER_CALL(vsa::tools::Logger::get_instance(name), vsa::tools::Logger::Level::WARNING, __VA_ARGS__);

#define VSA_LOG_ERROR(name, ...) \
VSA_LOGGER_CALL(vsa::tools::Logger::get_instance(name), vsa::tools::Logger::Level::ERROR, __VA_ARGS__);