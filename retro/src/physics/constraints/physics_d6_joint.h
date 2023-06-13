#pragma once

#include "physics/constraints/physics_joint.h"

namespace retro::physics
{
  class physics_d6_joint : public physics_joint
  {
  public:
    physics_d6_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                     const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1);
    ~physics_d6_joint() override;

    /* Getters */
    physx::PxD6Motion::Enum get_motion(physx::PxD6Axis::Enum axis) const;
    float get_twist_angle() const;
    float get_twist() const;
    float get_swing_y_angle() const;
    float get_swing_z_angle() const;
    physx::PxJointLinearLimit get_distance_limit() const;
    physx::PxJointLinearLimitPair get_linear_limit(physx::PxD6Axis::Enum axis) const;
    physx::PxJointAngularLimitPair get_twist_limit() const;
    physx::PxJointLimitCone get_swing_limit() const;
    physx::PxJointLimitPyramid get_pyramid_swing_limit() const;
    physx::PxD6JointDrive get_drive(physx::PxD6Drive::Enum index) const;
    physx::PxTransform get_drive_position() const;
    void get_drive_velocity(physx::PxVec3 &linear, physx::PxVec3 &angular) const;
    float get_projection_linear_tolerance() const;
    float get_projection_angular_tolerance() const;

    /* Setters */
    void set_motion(physx::PxD6Axis::Enum axis, physx::PxD6Motion::Enum type);
    void set_distance_limit(const physx::PxJointLinearLimit &limit);
    void set_linear_limit(physx::PxD6Axis::Enum axis, const physx::PxJointLinearLimitPair &limit);
    void set_twist_limit(const physx::PxJointAngularLimitPair &limit);
    void set_swing_limit(const physx::PxJointLimitCone &limit);
    void set_pyramid_swing_limit(const physx::PxJointLimitPyramid &limit);
    void set_drive(physx::PxD6Drive::Enum index, const physx::PxD6JointDrive &drive);
    void set_drive_position(const physx::PxTransform &pose, bool autowake = true);
    void set_drive_velocity(const physx::PxVec3 &linear, const physx::PxVec3 &angular, bool autowake = true);
    void set_projection_linear_tolerance(float tolerance);
    void set_projection_angular_tolerance(float tolerance);

  protected:
    void release_joint() override;

  private:
    physx::PxD6Joint *m_physx_d6_joint;
  };
}