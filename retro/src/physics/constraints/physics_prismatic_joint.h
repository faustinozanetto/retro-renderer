#pragma once

#include "physics/constraints/physics_joint.h"

#include <extensions/PxPrismaticJoint.h>

namespace retro::physics
{
  class physics_prismatic_joint : public physics_joint
  {
  public:
    physics_prismatic_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                            const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1);
    ~physics_prismatic_joint() override;

    /* Getters */
    physx::PxJointLinearLimitPair get_limit() const;
    float get_projection_linear_tolerance() const;
    float get_projection_angular_tolerance() const;
    float get_position() const;
    float get_velocity() const;
    physx::PxPrismaticJointFlags get_prismatic_joint_flags() const;

    /* Setters */
    void set_limit(float lower, float upper);
    void set_prismatic_joint_flags(physx::PxPrismaticJointFlags flags);
    void set_prismatic_joint_flag(physx::PxPrismaticJointFlag::Enum flag, bool value);
    void set_projection_linear_tolerance(float tolerance);
    void set_projection_angular_tolerance(float tolerance);

  protected:
    void release_joint() override;

  private:
    physx::PxPrismaticJoint *m_physx_prismatic_joint;
  };
}