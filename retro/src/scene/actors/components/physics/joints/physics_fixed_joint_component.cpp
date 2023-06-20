#include "rtpch.h"
#include "physics_fixed_joint_component.h"

namespace retro::scene
{
    physics_fixed_joint_component::physics_fixed_joint_component(const std::shared_ptr<physics::physics_fixed_joint> &fixed_joint)
    {
        m_fixed_joint = fixed_joint;
    }

    physics_fixed_joint_component::~physics_fixed_joint_component()
    {
    }

    void physics_fixed_joint_component::initialize()
    {
    }
}
