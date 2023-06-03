#include "rtpch.h"
#include "audio_context.h"

namespace retro::audio
{
    audio_context::audio_context()
    {
        RT_TRACE("Retro Renderer | Initializing audio context");
        // Initialize OpenAL.
        m_audio_device = alcOpenDevice(nullptr);
        RT_ASSERT_MSG(m_audio_device, "Could not initialize OpenLA device!");
        m_audio_context = alcCreateContext(m_audio_device, nullptr);
        RT_ASSERT_MSG(m_audio_context, "Could not initialize OpenLA context!");

        if (!alcMakeContextCurrent(m_audio_context))
        {
            alcDestroyContext(m_audio_context);
            alcCloseDevice(m_audio_device);
            RT_ASSERT_MSG(false, "Could not establish OpenLA context!");
        }
        RT_TRACE("Retro Renderer | Audio context initialized successfully!");
    }

    audio_context::~audio_context()
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_audio_context);
        alcCloseDevice(m_audio_device);
    }
}
