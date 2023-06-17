#pragma once

#include "audio/sound.h"

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    struct sound_source_component : public base_component
    {
    public:
        sound_source_component(const std::shared_ptr<audio::sound> &sound);
        ~sound_source_component() override;

        /* Getters */
        const std::shared_ptr<audio::sound> &get_sound() const { return m_sound; }

        /* Setters */
        void set_sound(const std::shared_ptr<audio::sound> &sound) { m_sound = sound; }

    private:
        std::shared_ptr<audio::sound> m_sound;
    };

};