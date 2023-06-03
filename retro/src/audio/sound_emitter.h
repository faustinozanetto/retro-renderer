#pragma once

#include "sound.h"

#include <AL/al.h>
#include <glm/glm.hpp>

namespace retro::audio
{
    class sound_emitter
    {
    public:
        sound_emitter();
        ~sound_emitter();

        void play();
        void pause();
        void stop();

        float get_volume() const { return m_volume; }
        void set_volume(float volume);

        glm::vec3 get_location() const { return m_location;}
        void set_location(const glm::vec3&location);

        void set_sound(const std::shared_ptr<sound>& sound);
        std::shared_ptr<sound>& get_sound() { return m_sound; }

    private:
        ALuint m_source;
        std::shared_ptr<sound> m_sound;
        glm::vec3 m_location;

        float m_volume;
        float m_pitch;
        float m_range;
        bool m_loop;
        bool m_paused;
    };
}
