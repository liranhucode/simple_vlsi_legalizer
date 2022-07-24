#pragma once
#include <iostream>
#include <string>
#include <chrono>
 
class Timer {
public:
    Timer() : label_("Time elapsed:")
    {
        restart();
    }

    explicit Timer(const std::string &label) : label_(label+ ":")
    {
        restart();
    }

    inline void Restart()
    {
        restart();
    }

    inline void Restart(const std::string &label)
    {
        label_ = label + ":";
        restart();
    }

    void Report(const std::string &label = "", bool is_ms = true)
    {
        if (is_ms)
        {
            if (label.empty())
                std::cout << label_ << elapsed() << "ms" << std::endl;
            else
                std::cout << label + ":" << elapsed() << "ms" << std::endl;
        }
        else
        {
            if (label.empty())
                std::cout << label_ << elapsed() / 1000.0 << "s" << std::endl;
            else
                std::cout << label + ":" << elapsed() / 1000.0 << "s" << std::endl;
        }
    }
 
private:
    inline void restart()
    {
        start_time_ = std::chrono::steady_clock::now();
    }

    inline double elapsed()
    {
        end_time_ = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end_time_ - start_time_;
        return diff.count() * 1000;
    }

private:
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point end_time_;
    std::string label_;
};  