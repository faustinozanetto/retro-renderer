#pragma once

#include "physics/constraints/physics_spherical_joint.h"

#include "scene/actors/components/physics/joints/physics_joint_component.h"

namespace retro::scene
{
    class physics_spherical_joint_component : public physics_joint_component
    {
    public:
        physics_spherical_joint_component(const std::shared_ptr<physics::physics_spherical_joint> &spherical_joint);
        ~physics_spherical_joint_component() override;

        /* Getters */
        const std::shared_ptr<physics::physics_spherical_joint> &get_spherical_joint() const { return m_spherical_joint; }

        /* Setters */
        void set_spherical_joint(const std::shared_ptr<physics::physics_spherical_joint> &spherical_joint) { m_spherical_joint = spherical_joint; }

    private:
        std::shared_ptr<physics::physics_spherical_joint> m_spherical_joint;
    };
}