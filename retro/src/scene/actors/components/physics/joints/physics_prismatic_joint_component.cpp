#include "rtpch.h"
#include "physics_prismatic_joint_component.h"

namespace retro::scene
{
    physics_prismatic_joint_component::physics_prismatic_joint_component(const std::shared_ptr<physics::physics_prismatic_joint> &prismatic_joint)
    {
        m_prismatic_joint = prismatic_joint;
    }

    physics_prismatic_joint_component::~physics_prismatic_joint_component()
    {
    }
}