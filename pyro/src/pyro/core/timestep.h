#pragma once

namespace pyro
{
    /// Delta time between one frame (or update) and the next one.
    class PYRO_API timestep final
    {
    public:
        // Takes the starting time in seconds
        timestep(float time = 0.f) 
            :m_time(time)
        {}

        /// Implicit cast of timestep to float. Returns seconds by default.
        operator float() const { return m_time; }

        float seconds() const { return m_time; }
        float milliseconds() const { return m_time * 1000.f; }

    private:
        /// Duration of the timestep in second.
        float m_time;
    };
}