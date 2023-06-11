#include "rtpch.h"
#include "physics_static_actor_component.h"

namespace retro::scene
{
    physics_static_actor_component::physics_static_actor_component(const std::shared_ptr<physics::physics_static_actor> &static_actor)
    {
        m_static_actor = static_actor;
    }

    physics_static_actor_component::~physics_static_actor_component()
    {
    }

}