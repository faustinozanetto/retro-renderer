#include "rtpch.h"
#include "physics_static_actor_component.h"

namespace retro::scene
{
    physics_static_actor_component::physics_static_actor_component()
    {
    }

    physics_static_actor_component::physics_static_actor_component(
        const std::shared_ptr<physics::physics_static_actor>& static_actor)
    {
        m_static_actor = static_actor;
    }

    physics_static_actor_component::~physics_static_actor_component()
    {
    }

    void physics_static_actor_component::initialize()
    {
        if (!m_static_actor)
            m_static_actor = std::make_shared<physics::physics_static_actor>();
    }
}
