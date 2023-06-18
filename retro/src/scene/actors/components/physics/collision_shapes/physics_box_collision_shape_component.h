#pragma once

#include "scene/actors/components/physics/collision_shapes/physics_collision_shape_component.h"

#include "physics/collision_shapes/physics_box_collision.h"

namespace retro::scene
{
    class physics_box_collision_shape_component : public physics_collision_shape_component
    {
    public:
        physics_box_collision_shape_component(const std::shared_ptr<physics::physics_box_collision> &box_collision) : m_box_collision(box_collision) {}
        virtual ~physics_box_collision_shape_component() override = default;

        /* Getters */
        const std::shared_ptr<physics::physics_box_collision> &get_box_collision() const { return m_box_collision; }

        /* Setters */
        void set_box_collision(const std::shared_ptr<physics::physics_box_collision> &box_collision) { m_box_collision = box_collision; }

    private:
        std::shared_ptr<physics::physics_box_collision> m_box_collision;
    };
}