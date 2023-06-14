#include "rtpch.h"
#include "physics_spherical_joint_component.h"

namespace retro::scene
{
    physics_spherical_joint_component::physics_spherical_joint_component(const std::shared_ptr<physics::physics_spherical_joint> &spherical_joint)
    {
        m_spherical_joint = spherical_joint;
    }

    physics_spherical_joint_component::~physics_spherical_joint_component()
    {
    }
}