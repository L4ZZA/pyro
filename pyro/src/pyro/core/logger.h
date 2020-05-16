#pragma once
#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace pyro
{
    /// \brief Wrapper around spdlog logger.
    class PYRO_API logger
    {
    public:
        static void init();
        static std::shared_ptr<spdlog::logger> const& core_logger() { return s_core_logger; }
        static std::shared_ptr<spdlog::logger> const& client_logger() { return s_client_logger; }
    private:
        static std::shared_ptr<spdlog::logger> s_core_logger;
        static std::shared_ptr<spdlog::logger> s_client_logger;
    };
}

// Core log macros
#define PYRO_CORE_TRACE(...)    ::pyro::logger::core_logger()->trace(__VA_ARGS__)
#define PYRO_CORE_INFO(...)     ::pyro::logger::core_logger()->info(__VA_ARGS__)
#define PYRO_CORE_WARN(...)     ::pyro::logger::core_logger()->warn(__VA_ARGS__)
#define PYRO_CORE_ERROR(...)    ::pyro::logger::core_logger()->error(__VA_ARGS__)
#define PYRO_CORE_FATAL(...)    ::pyro::logger::core_logger()->critical(__VA_ARGS__)
 // Client log macros
#define PYRO_TRACE(...)	      ::pyro::logger::client_logger()->trace(__VA_ARGS__)
#define PYRO_INFO(...)	      ::pyro::logger::client_logger()->info(__VA_ARGS__)
#define PYRO_WARN(...)	      ::pyro::logger::client_logger()->warn(__VA_ARGS__)
#define PYRO_ERROR(...)	      ::pyro::logger::client_logger()->error(__VA_ARGS__)
#define PYRO_FATAL(...)	      ::pyro::logger::client_logger()->critical(__VA_ARGS__)
