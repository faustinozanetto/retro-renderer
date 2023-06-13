#pragma once

#include "physics/constraints/physics_joint.h"

#include <extensions/PxFixedJoint.h>

namespace retro::physics
{
  class physics_spherical_joint : public physics_joint
  {
  public:
    physics_spherical_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                            const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1);
    ~physics_spherical_joint() override;

    /* Getters */
    physx::PxJointLimitCone get_limit_cone() const;
    float get_swing_y_angle() const;
    float get_swing_z_angle() const;
    float get_projection_linear_tolerance() const;
    physx::PxSphericalJointFlags get_spherical_joint_flags() const;

    /* Setters */
    void set_limit_cone(const physx::PxJointLimitCone &limit);
    void set_projection_linear_tolerance(float tolerance);
    void set_spherical_joint_flags(physx::PxSphericalJointFlags flags);
    void set_spherical_joint_flag(physx::PxSphericalJointFlag::Enum flag, bool value);

  protected:
    void release_joint() override;

  private:
    physx::PxSphericalJoint *m_physx_spherical_joint;
  };
}