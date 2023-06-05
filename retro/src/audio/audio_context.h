#pragma once

#include <AL/alc.h>

namespace retro::audio
{
    class audio_context
    {
    public:
        audio_context();
        ~audio_context();

    private:
        void print_debug_info();
        ALCcontext* m_audio_context;
        ALCdevice* m_audio_device;
    };
}