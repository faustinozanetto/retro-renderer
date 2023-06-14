#include "rtpch.h"
#include "physics_box_collision_shape_component.h"

namespace retro::scene
{
    physics_box_collision_shape_component::physics_box_collision_shape_component(const std::shared_ptr<physics::physics_box_collision> &box_collision)
    {
        m_box_collision = box_collision;
    }

    physics_box_collision_shape_component::~physics_box_collision_shape_component()
    {
    }
}