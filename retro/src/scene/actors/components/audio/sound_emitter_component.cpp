#include "rtpch.h"
#include "sound_emitter_component.h"

#include "sound_source_component.h"

namespace retro::scene
{
    sound_emitter_component::sound_emitter_component()
    {
        m_sound_emitter = std::make_shared<audio::sound_emitter>();
    }

    sound_emitter_component::sound_emitter_component(const std::shared_ptr<audio::sound_emitter>& sound_emitter)
    {
        m_sound_emitter = sound_emitter;
    }

    sound_emitter_component::~sound_emitter_component()
    {
    }

    void sound_emitter_component::initialize()
    {
        // Check if actor has a sound source component so that we can bind the sound
        if (m_actor->has_component<sound_source_component>())
        {
            const auto& sound_source_comp = m_actor->get_component<sound_source_component>();
            if (sound_source_comp.get_sound())
            {
                m_sound_emitter->set_sound(sound_source_comp.get_sound());
            }
        }
    }
}
