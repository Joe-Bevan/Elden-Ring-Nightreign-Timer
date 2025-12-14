#pragma once

#include <chrono>

// A scope based timer
class Stopwatch
{
public:
    Stopwatch() = default;
    // Should the timer start now or wait till Start() is called
    Stopwatch(const bool startInstantly = true)
    {
        if (startInstantly)
        {
            Start();
        }
    }

    void Start()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    void Stop()
    {
        m_end = std::chrono::high_resolution_clock::now();
    }

    // Returns time elapsed in seconds
    float fGetElapsedTime()
    {
        const std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_start;
        return duration.count();
    }

    // Returns time elapsed in seconds
    double dGetElapsedTime()
    {
        const std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - m_start;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_end;

};