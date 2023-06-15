#pragma once

#include "scene/actors/components/physics/collision_shapes/physics_collision_shape_component.h"

#include "physics/collision_shapes/physics_plane_collision.h"

namespace retro::scene
{
    struct physics_plane_collision_shape_component : public physics_collision_shape_component
    {
    public:
        physics_plane_collision_shape_component(const std::shared_ptr<physics::physics_plane_collision> &plane_collision) : m_plane_collision(plane_collision) {}
        virtual ~physics_plane_collision_shape_component() override = default;

        /* Getters */
        const std::shared_ptr<physics::physics_plane_collision> &get_plane_collision() const { return m_plane_collision; }

        /* Setters */
        void set_plane_collision(const std::shared_ptr<physics::physics_plane_collision> &plane_collision) { m_plane_collision = plane_collision; }

    private:
        std::shared_ptr<physics::physics_plane_collision> m_plane_collision;
    };
}