#include "rtpch.h"
#include "sound_source_component.h"

namespace retro::scene
{
    sound_source_component::sound_source_component(const std::shared_ptr<audio::sound> &sound)
    {
        m_sound = sound;
    }

	sound_source_component::sound_source_component()
	{

	}

    sound_source_component::~sound_source_component()
    {
    }

    void sound_source_component::initialize()
    {
    }
}
