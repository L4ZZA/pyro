#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <thread>

namespace pyro{
namespace debug{

    using floating_point_microseconds = std::chrono::duration<double, std::micro>;

    /*
     * Data structure containing the result
     * of a profiling routine.
     */
    struct profile_result
    {
        std::string name;
        floating_point_microseconds start;
        std::chrono::microseconds elapsed_time;
        std::thread::id thread_id;
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
        ~profiler();
    public:
        profiler(const profiler &) = delete;
        profiler(profiler &&) = delete;
        void begin_session(std::string const &name, std::string const &filepath = "result.json");
        void end_session();
        void write_profile(profile_result const &result);
        void write_header();
        void write_footer();
        void internal_end_session();

    public:
        static profiler &get();

    private:
        std::mutex m_mutex;
        profiler_session *m_current_session;
        std::ofstream     m_output_stream;
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
        std::chrono::time_point<std::chrono::steady_clock> m_start_timepoint;
    };


namespace profiler_utils
{

    template <size_t N>
    struct change_result
    {
        char data[N];
    };

    template <size_t N, size_t K>
    constexpr auto cleanup_output_string(const char(&expr)[N], const char(&remove)[K])
    {
        change_result<N> result = {};

        size_t src_index = 0;
        size_t dest_index = 0;
        while (src_index < N)
        {
            size_t match_index = 0;
            while (match_index < K - 1 && src_index + match_index < N - 1 && expr[src_index + match_index] == remove[match_index])
                match_index++;
            if (match_index == K - 1)
                src_index += match_index;
            result.data[dest_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
            src_index++;
        }
        return result;
    }
}
}}

//================================================

inline
pyro::debug::profiler::profiler()
    : m_current_session(nullptr)
{
}

inline
pyro::debug::profiler::~profiler()
{
    end_session();
}

inline
void
pyro::debug::profiler::begin_session(std::string const &name, std::string const &filepath)
{
    std::lock_guard lock(m_mutex);
    if (m_current_session)
    {
        // If there is already a current session, then close it 
        // before beginning new one.
        // Subsequent profiling output meant for the original session 
        // will end up in the newly opened session instead.  
        // That's better than having badly formatted profiling output.
        if (logger::core_logger()) 
        {
            // Edge case: BeginSession() might be before Log::Init()
            PYRO_CORE_ERROR("pyro::debug::profiler::begin_session('{0}') when session '{1}' already open.", 
                name, m_current_session->name);
        }
        internal_end_session();
    }
    m_output_stream.open(filepath);

    if (m_output_stream.is_open())
    {
        m_current_session = new profiler_session({ name });
        write_header();
    }
    else
    {
        if (logger::core_logger()) // Edge case: BeginSession() might be before Log::Init()
        {
            PYRO_CORE_ERROR("pyro::debug::profiler could not open results file '{0}'.", 
                filepath);
        }
    }
}

inline
void
pyro::debug::profiler::end_session()
{
    std::lock_guard lock(m_mutex);
    internal_end_session();
}

inline
void
pyro::debug::profiler::write_profile(profile_result const &result)
{
    std::stringstream json;

    json << std::setprecision(3) << std::fixed;
    m_output_stream << ",{";
    m_output_stream << "\"cat\":\"function\",";
    m_output_stream << "\"dur\":" << result.elapsed_time.count() << ',';
    m_output_stream << "\"name\":\"" << result.name << "\",";
    m_output_stream << "\"ph\":\"X\",";
    m_output_stream << "\"pid\":0,";
    m_output_stream << "\"tid\":" << result.thread_id << ",";
    m_output_stream << "\"ts\":" << result.start.count();
    m_output_stream << "}";

    {
        std::lock_guard lock(m_mutex);
        if (m_current_session)
        {
            m_output_stream << json.str();
            m_output_stream.flush();
        }
    }
}

inline
void
pyro::debug::profiler::write_header()
{
    m_output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
    m_output_stream.flush();
}

inline
void
pyro::debug::profiler::write_footer()
{
    m_output_stream << "]}";
    m_output_stream.flush();
}

// Note: you must already own lock on m_mutex before
// calling InternalEndSession()
inline 
void 
pyro::debug::profiler::internal_end_session()
{
    if (m_current_session)
    {
        write_footer();
        m_output_stream.close();
        delete m_current_session;
        m_current_session = nullptr;
    }
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
    m_start_timepoint = std::chrono::steady_clock::now();
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
    auto end_timepoint = std::chrono::steady_clock::now();
    auto high_res_start = floating_point_microseconds{ 
        m_start_timepoint.time_since_epoch() 
    };
    auto elapsed_time = 
        std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint)
            .time_since_epoch() - 
        std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint)
            .time_since_epoch();

    profiler::get().write_profile({ 
        m_name, 
        high_res_start, 
        elapsed_time, 
        std::this_thread::get_id() });

    m_stopped = true;
}

//---------------------------------------------

#define PYRO_PROFILE 0
#if PYRO_PROFILE
    // Resolve which function signature macro will be used. Note that this only
    // is resolved when the (pre)compiler starts, so the syntax highlighting
    // could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define PYRO_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define PYRO_FUNC_SIG __PRETTY_FUNCTION__
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
        #define PYRO_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define PYRO_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define PYRO_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define PYRO_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define PYRO_FUNC_SIG __func__
    #else
        #define PYRO_FUNC_SIG "PYRO_FUNC_SIG unknown!"
    #endif

    #define PYRO_PROFILE_BEGIN_SESSION(name, filepath) ::pyro::debug::profiler::get().begin_session(name, filepath)
    #define PYRO_PROFILE_END_SESSION() ::pyro::debug::profiler::get().end_session()
    #define PYRO_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixed_name##line = ::pyro::debug::profiler_utils::cleanup_output_string(name, "__cdecl ");\
											       ::pyro::debug::profiler_timer timer##line(fixed_name##line.data)
    #define PYRO_PROFILE_SCOPE_LINE(name, line) PYRO_PROFILE_SCOPE_LINE2(name, line)
    #define PYRO_PROFILE_SCOPE(name) PYRO_PROFILE_SCOPE_LINE(name, __LINE__)
    #define PYRO_PROFILE_FUNCTION() PYRO_PROFILE_SCOPE(PYRO_FUNC_SIG)
#else
    #define PYRO_PROFILE_BEGIN_SESSION(name, filepath)
    #define PYRO_PROFILE_END_SESSION()
    #define PYRO_PROFILE_SCOPE(name)
    #define PYRO_PROFILE_FUNCTION()
#endif