#pragma once

#include "physics/constraints/physics_joint.h"

#include <glm/gtc/constants.hpp>

namespace retro::physics
{
  class physics_revolute_joint : public physics_joint
  {
  public:
    physics_revolute_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                           const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1);
    ~physics_revolute_joint() override;

    /* Getters */
    float get_angle() const;
    float get_velocity() const;
    float get_drive_velocity() const;
    float get_drive_force_limit() const;
    float get_drive_gear_ratio() const;
    float get_projection_linear_tolerance() const;
    float get_projection_angular_tolerance() const;
    physx::PxRevoluteJointFlags get_joint_flags() const;
    physx::PxJointAngularLimitPair get_limit() const;

    /* Setters */
    void set_limit(float lower, float upper);
    void set_drive_velocity(float velocity, bool autowake = true);
    void set_drive_force_limit(float force_limit);
    void set_drive_gear_ratio(float gear_ratio);
    void set_joint_flags(physx::PxRevoluteJointFlags flags);
    void set_joint_flag(physx::PxRevoluteJointFlag::Enum flag, bool value);
    void set_projection_linear_tolerance(float distance);
    void set_projection_angular_tolerance(float tolerance);

  protected:
    void release_joint() override;

  private:
    physx::PxRevoluteJoint *m_physx_revolute_joint;
  };
}