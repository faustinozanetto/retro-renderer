#pragma once

#include "physics/actors/physics_dynamic_actor.h"

#include "scene/actors/components/base_component.h"

namespace retro::scene
{
    class physics_dynamic_actor_component : public base_component
    {
    public:
        physics_dynamic_actor_component(const std::shared_ptr<physics::physics_dynamic_actor> &dynamic_actor);
        ~physics_dynamic_actor_component() override;

        /* Getters */
        const std::shared_ptr<physics::physics_dynamic_actor> &get_dynamic_actor() const { return m_dynamic_actor; }

        /* Setters */
        void set_material(const std::shared_ptr<physics::physics_dynamic_actor> &dynamic_actor) { m_dynamic_actor = dynamic_actor; }

    private:
        std::shared_ptr<physics::physics_dynamic_actor> m_dynamic_actor;
    };
}