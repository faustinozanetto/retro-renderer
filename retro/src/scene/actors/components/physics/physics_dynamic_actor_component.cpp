#include "rtpch.h"
#include "physics_dynamic_actor_component.h"

namespace retro::scene
{
    physics_dynamic_actor_component::physics_dynamic_actor_component()
    {
        m_dynamic_actor = std::make_shared<physics::physics_dynamic_actor>();
    }

    physics_dynamic_actor_component::physics_dynamic_actor_component(const std::shared_ptr<physics::physics_dynamic_actor> &dynamic_actor)
    {
        m_dynamic_actor = dynamic_actor;
    }

    physics_dynamic_actor_component::~physics_dynamic_actor_component()
    {
    }

}