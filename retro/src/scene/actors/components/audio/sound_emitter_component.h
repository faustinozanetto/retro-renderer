#pragma once

#include "audio/sound_emitter.h"

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    struct sound_emitter_component : public base_component
    {
    public:
        sound_emitter_component();
        sound_emitter_component(const std::shared_ptr<audio::sound_emitter> &sound_emitter);
        ~sound_emitter_component() override;

        /* Getters */
        const std::shared_ptr<audio::sound_emitter> &get_sound_emitter() const { return m_sound_emitter; }

        /* Setters */
        void set_sound_emitter(const std::shared_ptr<audio::sound_emitter> &sound_emitter) { m_sound_emitter = sound_emitter; }

    private:
        std::shared_ptr<audio::sound_emitter> m_sound_emitter;
    };

};