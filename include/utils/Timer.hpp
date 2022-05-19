#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

class Timer
{
private:
    decltype(std::chrono::high_resolution_clock::now()) m_startTime;
    bool m_started;
    float m_record;

public:
    Timer() : m_started(false), m_record(0.f) {}
    void start()
    {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_started = true;
    }

    float query()
    {
        if (!m_started)
            return 0.f;
        auto currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> dur = currTime - m_startTime;
        return dur.count();
    }

    void stop()
    {
        if (m_started)
        {
            m_record = query();
            m_started = false;
        }
    }

    float record() const
    {
        return m_record;
    }
};

#endif
