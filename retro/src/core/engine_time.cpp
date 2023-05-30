#include "rtpch.h"
#include "engine_time.h"

namespace retro::core::time
{
    static time_point now = std::chrono::high_resolution_clock::now();
    static duration delta_time = duration::zero();
    static size_t ticks = 0;
    static float time = 0.0f;
    static float raw_time = 0.0f;
    static float time_scale = 1.0f;
    
    void set_delta_time(float t)
    {
        delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<float, std::ratio<1, 1>>(t));
    }

    void set_time_scale(float t)
    {
        time_scale = t;
    }

    void update_time()
    {
        ticks++;
        time += get_delta_time();
        raw_time += get_raw_delta_time();
        delta_time = std::chrono::high_resolution_clock::now() - now;
        now = std::chrono::high_resolution_clock::now();
    }

    float get_raw_delta_time()
    {
        return delta_time.count() / 1000000000.0f;
    }

    float get_delta_time()
    {
        return get_raw_delta_time() * get_time_scale();
    }

    float get_time_scale()
    {
        return time_scale;
    }
}