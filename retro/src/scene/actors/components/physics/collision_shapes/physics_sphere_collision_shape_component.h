#pragma once

#include "scene/actors/components/physics/collision_shapes/physics_collision_shape_component.h"

#include "physics/collision_shapes/physics_sphere_collision.h"

namespace retro::scene
{
    struct physics_sphere_collision_shape_component : public physics_collision_shape_component
    {
    public:
        physics_sphere_collision_shape_component(const std::shared_ptr<physics::physics_sphere_collision> &sphere_collision) : m_sphere_collision(sphere_collision) {}
        virtual ~physics_sphere_collision_shape_component() override = default;

        /* Getters */
        const std::shared_ptr<physics::physics_sphere_collision> &get_sphere_collision() const { return m_sphere_collision; }

        /* Setters */
        void set_sphere_collision(const std::shared_ptr<physics::physics_sphere_collision> &sphere_collision) { m_sphere_collision = sphere_collision; }

    private:
        std::shared_ptr<physics::physics_sphere_collision> m_sphere_collision;
    };
}