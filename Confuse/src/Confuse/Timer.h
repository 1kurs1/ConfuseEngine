#pragma once

#include <iostream>
#include <string>
#include <chrono>

namespace Confuse{
    class Timer{
    public:
        Timer(){
            reset();
        }

        void reset() {m_start = std::chrono::high_resolution_clock::now();}
        float elapsed(){return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_start).count() * 0.001f * 0.001f * 0.001f;}
        float elapsedMilis(){return elapsed() * 1000.0f;}

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    };

    class ScopedTimer{
    public:
        ScopedTimer(const std::string& name) : m_name(name){}
        ~ScopedTimer(){
            float time = m_timer.elapsedMilis();
            std::cout << "[timer] " << m_name << " - " << time << "ms\n";
        }

    private:
        std::string m_name;
        Timer m_timer;
    };
}