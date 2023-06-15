#include "rtpch.h"
#include "audio_context.h"

#include <AL/alc.h>

namespace retro::audio
{
    audio_context::audio_context()
    {
        RT_SEPARATOR();
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

        print_debug_info();

        RT_TRACE("Retro Renderer | Audio context initialized successfully!");
        RT_SEPARATOR();
    }

    audio_context::~audio_context()
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_audio_context);
        alcCloseDevice(m_audio_device);
    }

    void audio_context::print_debug_info()
    {
        RT_PROFILE;
        // Available device names
        const ALCchar *deviceNames = alcGetString(m_audio_device, ALC_ALL_DEVICES_SPECIFIER);
        RT_TRACE("  - Available Devices: {}", deviceNames);

        // Default device name
        const ALCchar *defaultDeviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
        RT_TRACE("  - Default Device: {}", defaultDeviceName);

        // Context attributes
        ALCint attributes[3];
        alcGetIntegerv(m_audio_device, ALC_MAJOR_VERSION, 1, &attributes[0]);
        alcGetIntegerv(m_audio_device, ALC_MINOR_VERSION, 1, &attributes[1]);
        alcGetIntegerv(m_audio_device, ALC_ATTRIBUTES_SIZE, 1, &attributes[2]);
        RT_TRACE("  - Context Version: {}.{}", attributes[0], attributes[1]);
        RT_TRACE("  - Context Attributes Size: {}", attributes[2]);

        // Additional attributes
        ALCint frequency, refresh, sync, monoSources, stereoSources;
        alcGetIntegerv(m_audio_device, ALC_FREQUENCY, 1, &frequency);
        alcGetIntegerv(m_audio_device, ALC_REFRESH, 1, &refresh);
        alcGetIntegerv(m_audio_device, ALC_SYNC, 1, &sync);
        alcGetIntegerv(m_audio_device, ALC_MONO_SOURCES, 1, &monoSources);
        alcGetIntegerv(m_audio_device, ALC_STEREO_SOURCES, 1, &stereoSources);
        RT_TRACE("  - Frequency: {}", frequency);
        RT_TRACE("  - Refresh: {}", refresh);
        RT_TRACE("  - Sync: {}", sync);
        RT_TRACE("  - Mono Sources: {}", monoSources);
        RT_TRACE("  - Stereo Sources: {}", stereoSources);

        // OpenAL extensions
        const ALCchar *extensions = alcGetString(m_audio_device, ALC_EXTENSIONS);
        RT_TRACE("  - OpenAL Extensions: {}", extensions);
    }
}
