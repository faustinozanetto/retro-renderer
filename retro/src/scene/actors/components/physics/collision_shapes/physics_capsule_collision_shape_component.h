#pragma once

#include "scene/actors/components/physics/collision_shapes/physics_collision_shape_component.h"

#include "physics/collision_shapes/physics_capsule_collision.h"

namespace retro::scene
{
    struct physics_capsule_collision_shape_component : public physics_collision_shape_component
    {
    public:
        physics_capsule_collision_shape_component(const std::shared_ptr<physics::physics_capsule_collision> &capsule_collision) : m_capsule_collision(capsule_collision) {}
        virtual ~physics_capsule_collision_shape_component() override = default;

        /* Getters */
        const std::shared_ptr<physics::physics_capsule_collision> &get_capsule_collision() const { return m_capsule_collision; }

        /* Setters */
        void set_capsule_collision(const std::shared_ptr<physics::physics_capsule_collision> &capsule_collision) { m_capsule_collision = capsule_collision; }

    private:
        std::shared_ptr<physics::physics_capsule_collision> m_capsule_collision;
    };
}