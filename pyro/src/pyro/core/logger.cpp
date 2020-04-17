#include "pyro_pch.h"
#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

pyro::ref<spdlog::logger> pyro::logger::s_core_logger;
pyro::ref<spdlog::logger> pyro::logger::s_client_logger;

void pyro::logger::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_core_logger = spdlog::stdout_color_mt("PYRO");
    s_core_logger->set_level(spdlog::level::trace);
    s_client_logger = spdlog::stdout_color_mt("APP");
    s_client_logger->set_level(spdlog::level::trace);
}