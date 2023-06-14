#include "rtpch.h"
#include "physics_distance_joint_component.h"

namespace retro::scene
{
    physics_distance_joint_component::physics_distance_joint_component(const std::shared_ptr<physics::physics_distance_joint> &distance_joint)
    {
        m_distance_joint = distance_joint;
    }

    physics_distance_joint_component::~physics_distance_joint_component()
    {
    }
}