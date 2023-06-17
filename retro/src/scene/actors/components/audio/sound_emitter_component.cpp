#include "rtpch.h"
#include "sound_emitter_component.h"

namespace retro::scene
{
    sound_emitter_component::sound_emitter_component()
    {
        m_sound_emitter = std::make_shared<audio::sound_emitter>();
    }

    sound_emitter_component::sound_emitter_component(const std::shared_ptr<audio::sound_emitter> &sound_emitter)
    {
        m_sound_emitter = sound_emitter;
    }

    sound_emitter_component::~sound_emitter_component()
    {
    }

}