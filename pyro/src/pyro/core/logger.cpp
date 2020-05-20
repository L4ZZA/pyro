#include "pyro_pch.h"
#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

pyro::ref<spdlog::logger> pyro::logger::s_core_logger;
pyro::ref<spdlog::logger> pyro::logger::s_client_logger;

enum class pyro::logger::e_level
{
    debug   = (int)spdlog::level::trace,
    info    = (int)spdlog::level::info,
    warning = (int)spdlog::level::warn,
    error   = (int)spdlog::level::err,
    severe  = (int)spdlog::level::critical,
    off     = (int)spdlog::level::off,
};

void pyro::logger::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_core_logger = spdlog::stdout_color_mt("PYRO");
    s_core_logger->set_level(spdlog::level::trace);
    s_client_logger = spdlog::stdout_color_mt("APP");
    s_client_logger->set_level(spdlog::level::trace);
}

void pyro::logger::log_level(e_level const& level)
{
    s_core_logger->set_level(static_cast<spdlog::level::level_enum>(level));
    s_client_logger->set_level(static_cast<spdlog::level::level_enum>(level));
}
