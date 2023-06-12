#pragma once

#include "physics/actors/physics_actor.h"

namespace retro::physics
{
    class physics_joint
    {
    public:
        physics_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const std::shared_ptr<physics_actor> &physics_actor_b) : m_physics_actor_a(physics_actor_a), m_physics_actor_b(physics_actor_b) { }
        virtual ~physics_joint() = default;

        /* Getters */
        const std::shared_ptr<physics_actor> &get_physics_actor_a() const { return m_physics_actor_a; }
        const std::shared_ptr<physics_actor> &get_physics_actor_b() const { return m_physics_actor_b; }

    protected:
        virtual void initialize_joint() = 0;
        virtual void release_joint() = 0;

        std::shared_ptr<physics_actor> m_physics_actor_a;
        std::shared_ptr<physics_actor> m_physics_actor_b;
    };
}