#pragma once

#include "physics/collision_shapes/physics_collision_shape.h"

namespace retro::physics
{
    class physics_actor
    {
    public:
        physics_actor() : m_initialized(false), m_collision_shapes({}) {}
        virtual ~physics_actor() = default;

        /* Getters */
        const std::vector<std::shared_ptr<physics_collision_shape>>& get_collision_shapes() const { return m_collision_shapes; }

        /* Functions */
        virtual void initialize() = 0;
        void add_collision_shape(const std::shared_ptr<physics_collision_shape>& collision_shape) { m_collision_shapes.push_back(collision_shape); }

    protected:
        virtual void attach_collision_shapes() = 0;

        bool m_initialized;
        std::vector<std::shared_ptr<physics_collision_shape>> m_collision_shapes;
    };
}
