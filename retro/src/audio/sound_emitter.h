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

        /* Getters */
		glm::vec3 get_location() const { return m_location; }
		const std::shared_ptr<sound>& get_sound() const { return m_sound; }
        float get_volume() const { return m_volume; }
        float get_pitch() const { return m_pitch; }
        float get_range() const { return m_range; }
        bool get_is_loop() const { return m_loop; }
        bool get_is_paused() const { return m_paused; }

        /* Setters */
		void set_location(const glm::vec3& location);
		void set_sound(const std::shared_ptr<sound>& sound);
		void set_volume(float volume);
        void set_pitch(float pitch);
        void set_range(float range);
        void set_is_loop(bool is_loop);

        /* Functions */
        void play();
        void pause();
        void stop();

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
