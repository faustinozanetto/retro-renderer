#include "rtpch.h"
#include "physics_d6_joint_component.h"

namespace retro::scene
{
    physics_d6_joint_component::physics_d6_joint_component(const std::shared_ptr<physics::physics_d6_joint> &d6_joint)
    {
        m_d6_joint = d6_joint;
    }

    physics_d6_joint_component::~physics_d6_joint_component()
    {
    }
}