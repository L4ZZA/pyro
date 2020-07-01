#pragma once
#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace pyro
{
    /// Wrapper around spdlog logger.
    class PYRO_API logger
    {
    public:
        enum class e_level;

    public:
        static void init();
        static void log_level(e_level const& level);
        static std::shared_ptr<spdlog::logger> const& core_logger() { return s_core_logger; }
        static std::shared_ptr<spdlog::logger> const& client_logger() { return s_client_logger; }
    private:
        static std::shared_ptr<spdlog::logger> s_core_logger;
        static std::shared_ptr<spdlog::logger> s_client_logger;
    };
}

#ifdef PYRO_DEBUG

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

#else

// Core log macros
#define PYRO_CORE_TRACE(...)
#define PYRO_CORE_INFO(...) 
#define PYRO_CORE_WARN(...) 
#define PYRO_CORE_ERROR(...)
#define PYRO_CORE_FATAL(...)
 // Client log macros
#define PYRO_TRACE(...)	    
#define PYRO_INFO(...)	    
#define PYRO_WARN(...)	    
#define PYRO_ERROR(...)	    
#define PYRO_FATAL(...)	 

#endif 