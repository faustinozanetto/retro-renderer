#include "rtpch.h"
#include "sound_emitter.h"

namespace retro::audio
{
    sound_emitter::sound_emitter()
    {
        m_location = glm::vec3(0.0f, 0.0f, 0.0f);
        m_pitch = 1.0f;
        m_volume = 1.0f;
        m_range = 50.0f;
        m_loop = false;
        m_paused = false;
        alGenSources(1, &m_source);
    }

    sound_emitter::~sound_emitter()
    {
        alDeleteSources(1, &m_source);
    }

    void sound_emitter::play()
    {
        RT_PROFILE;
        alSourcePlay(m_source);
        m_paused = false;
    }

    void sound_emitter::pause()
    {
        RT_PROFILE;
        alSourcePause(m_source);
        m_paused = true;
    }

    void sound_emitter::stop()
    {
        RT_PROFILE;
        alSourceStop(m_source);
        m_paused = false;
    }

    void sound_emitter::set_volume(float volume)
    {
        RT_PROFILE;
        m_volume = volume;
        alSourcef(m_source, AL_GAIN, volume);
    }

    void sound_emitter::set_location(const glm::vec3 &location)
    {
        RT_PROFILE;
        m_location = location;
        alSourcefv(m_source, AL_POSITION, reinterpret_cast<float *>(&m_location));
    }

    void sound_emitter::set_sound(const std::shared_ptr<sound> &sound)
    {
        RT_PROFILE;
        stop();
        m_sound = sound;
        alSourcei(m_source, AL_BUFFER, m_sound->get_sound_buffer());
        alSourcef(m_source, AL_MAX_DISTANCE, m_range);
        alSourcef(m_source, AL_REFERENCE_DISTANCE, 1.0f);
        alSourcef(m_source, AL_ROLLOFF_FACTOR, 1.0f);
        alSourcei(m_source, AL_LOOPING, m_loop ? 1 : 0);
        alSourcef(m_source, AL_GAIN, m_volume);
        alSourcef(m_source, AL_PITCH, m_pitch);
    }
}
