#include "rtpch.h"
#include "engine_time.h"

namespace retro::core::time
{
    static time_point now = std::chrono::high_resolution_clock::now();
    static time_point previous_time = std::chrono::high_resolution_clock::now();
    static duration delta_time = duration::zero();
    static size_t ticks = 0;
    static float time = 0.0f;
    static float time_scale = 1.0f;

    static const int MAX_FRAME_SAMPLES = 200; // Maximum number of frame samples to store
    static std::vector<float> frame_times;    // Store frame times for FPS calculation

    void set_delta_time(float t)
    {
        RT_PROFILE;
        delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<float, std::ratio<1, 1>>(t));
    }

    void set_time_scale(float t)
    {
        RT_PROFILE;
        time_scale = t;
    }

    void update_time()
    {
        RT_PROFILE;
        // Calculate the frame time
        time_point current_time = std::chrono::high_resolution_clock::now();
        duration delta = current_time - previous_time;
        previous_time = current_time;
        float frame_time = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

        // Store frame time for FPS calculation
        frame_times.push_back(frame_time);
        if (frame_times.size() > MAX_FRAME_SAMPLES)
            frame_times.erase(frame_times.begin());

        ticks++;
        time += get_delta_time();
        delta_time = std::chrono::high_resolution_clock::now() - now;
        now = std::chrono::high_resolution_clock::now();
    }

    float get_raw_delta_time()
    {
        RT_PROFILE;
        return delta_time.count() / 1000000000.0f;
    }

    float get_delta_time()
    {
        RT_PROFILE;
        return get_raw_delta_time() * get_time_scale();
    }

    float get_frame_time()
    {
        RT_PROFILE;
        if (frame_times.empty())
            return 0.0f;

        return frame_times.back();
    }

    float get_fps()
    {
        RT_PROFILE;
        if (frame_times.empty())
            return 0.0f;

        float totalTime = 0.0f;
        for (float frame_time : frame_times)
            totalTime += frame_time;

        float avg_frame_time = totalTime / frame_times.size();
        if (avg_frame_time > 0.0f)
            return 1.0f / avg_frame_time;

        return 0.0f;
    }

    float get_time()
    {
        RT_PROFILE;
        return time;
    }

    std::vector<float> &get_frame_times()
    {
        return frame_times;
    }

    float get_time_scale()
    {
        RT_PROFILE;
        return time_scale;
    }
}