#pragma once

#include <chrono>

namespace retro::core::time
{
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;
    using duration  = std::chrono::high_resolution_clock::duration;

    void set_delta_time(float t);
    void set_time_scale(float t);

    void update_time();

    float get_raw_delta_time();
    float get_delta_time();

    float get_frame_time();
    float get_fps();

    float get_time();
    std::vector<float>& get_frame_times();

    float get_time_scale();
}