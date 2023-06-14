#include "rtpch.h"
#include "physics_revolute_joint_component.h"

namespace retro::scene
{
    physics_revolute_joint_component::physics_revolute_joint_component(const std::shared_ptr<physics::physics_revolute_joint> &revolute_joint)
    {
        m_revolute_joint = revolute_joint;
    }

    physics_revolute_joint_component::~physics_revolute_joint_component()
    {
    }
}