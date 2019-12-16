#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

namespace pyro{
namespace debug{

    /*
     * Data structure containing the result
     * of a profiling routine.
     */
    struct profile_result
    {
        std::string name;
        long long start;
        long long end;
        uint32_t thread_id;
    };

    //-----------------------------------------

    /*
     * 
     */
    struct profiler_session
    {
        std::string name;
    };

    /*
     * 
     */
    class profiler final
    {
    private:
        profiler();
    public:
        void begin_session(std::string const &name, std::string const &filepath = "result.json");
        void end_session();
        void write_profile(profile_result const &result);
        void write_header();
        void write_footer();

    public:
        static profiler &get();

    private:
        profiler_session *m_current_session;
        std::ofstream     m_output_stream;
        int               m_profile_count;
    };

    //-----------------------------------------

    class profiler_timer final
    {
    public:
        profiler_timer(const char *name);
        ~profiler_timer();
        void stop();

    private:
        const char *m_name;
        bool        m_stopped;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;
    };
}}

//================================================

inline
pyro::debug::profiler::profiler()
    : m_current_session(nullptr)
    , m_profile_count(0)
{
}

inline
void
pyro::debug::profiler::begin_session(std::string const &name, std::string const &filepath)
{
    m_output_stream.open(filepath);
    write_header();
    m_current_session = new profiler_session{name};
}

inline
void
pyro::debug::profiler::end_session()
{
    write_footer();
    m_output_stream.close();
    delete m_current_session;
    m_current_session = nullptr;
    m_profile_count = 0;
}

inline
void
pyro::debug::profiler::write_profile(profile_result const &result)
{
    if(m_profile_count++ > 0)
        m_output_stream << ",";

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_output_stream << "{";
    m_output_stream << "\"cat\":\"function\",";
    m_output_stream << "\"dur\":" << (result.end - result.start) << ',';
    m_output_stream << "\"name\":\"" << name << "\",";
    m_output_stream << "\"ph\":\"X\",";
    m_output_stream << "\"pid\":0,";
    m_output_stream << "\"tid\":" << result.thread_id << ",";
    m_output_stream << "\"ts\":" << result.start;
    m_output_stream << "}";

    m_output_stream.flush();
}

inline
void
pyro::debug::profiler::write_header()
{
    m_output_stream << "{\"otherData\": {},\"traceEvents\":[";
    m_output_stream.flush();
}

inline
void
pyro::debug::profiler::write_footer()
{
    m_output_stream << "]}";
    m_output_stream.flush();
}

inline
pyro::debug::profiler &
pyro::debug::profiler::get()
{
    static profiler instance;
    return instance;
}

//----------------------------------------------

inline
pyro::debug::profiler_timer::profiler_timer(const char *name)
    : m_name(name), m_stopped(false)
{
    m_start_timepoint = std::chrono::high_resolution_clock::now();
}

inline
pyro::debug::profiler_timer::~profiler_timer()
{
    if(!m_stopped)
        stop();
}

inline
void
pyro::debug::profiler_timer::stop()
{
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
    profiler::get().write_profile({m_name, start, end, threadID});

    m_stopped = true;
}

//---------------------------------------------

#define PYRO_PROFILE 0
#if PYRO_PROFILE
#define PYRO_PROFILE_BEGIN_SESSION(name, filepath) ::pyro::debug::profiler::get().begin_session(name, filepath)
#define PYRO_PROFILE_END_SESSION() ::pyro::debug::profiler::get().end_session()
#define PYRO_PROFILE_SCOPE(name) ::pyro::debug::profiler_timer timer##__LINE__(name);
#define PYRO_PROFILE_FUNCTION() PYRO_PROFILE_SCOPE(__FUNCSIG__)
#else
#define PYRO_PROFILE_BEGIN_SESSION(name, filepath)
#define PYRO_PROFILE_END_SESSION()
#define PYRO_PROFILE_SCOPE(name)
#define PYRO_PROFILE_FUNCTION()
#endif