#pragma once

#ifdef TRACY_ENABLE
#define WIN32_LEAN_AND_MEAN
#include "tracy/Tracy.hpp"
#define RT_PROFILE ZoneScoped
#define RT_PROFILE_FRAME(x) FrameMark
#define RT_PROFILE_SECTION(x) ZoneScopedN(x)
#include "tracy/TracyOpenGL.hpp"
#define RT_PROFILE_GPU(x) TracyGpuZone(x)
#define RT_PROFILE_GPU_FRAME(x) TracyGpuZoneN(x)
#else
#define RT_PROFILE
#define RT_PROFILE_FRAME(x)
#define RT_PROFILE_SECTION(x)
#define RT_PROFILE_GPU(x)
#define RT_PROFILE_GPU_FRAME(x)
#endif