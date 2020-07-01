#pragma once

namespace pyro
{
    class timer
    {
    public:
        timer();
        /// Starts the timer
        void start();
        /// Returns the seconds passed since 
        /// the last elapsed() call or since the sart().
        float elapsed();
        /// Returns the milliseconds passed since 
        /// the last elapsed() call or since the sart().
        float elapsed_millis();

    private:
        double m_delta_time{ 0.0 };
        double m_last_frame{ 0.0 };
        bool m_started = false;
    };
}