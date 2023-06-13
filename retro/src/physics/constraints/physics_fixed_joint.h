#pragma once

#include "physics/constraints/physics_joint.h"

#include <extensions/PxFixedJoint.h>

namespace retro::physics
{
  class physics_fixed_joint : public physics_joint
  {
  public:
    physics_fixed_joint(const std::shared_ptr<physics_actor> &physics_actor_a, const physx::PxTransform &t0,
                        const std::shared_ptr<physics_actor> &physics_actor_b, const physx::PxTransform &t1);
    ~physics_fixed_joint() override;

    /* Getters */
    float get_projection_linear_tolerance() const;
    float get_projection_angular_tolerance() const;

    /* Setters */
    void set_projection_linear_tolerance(float distance);
    void set_projection_angular_tolerance(float tolerance);

  protected:
    void release_joint() override;

  private:
    physx::PxFixedJoint *m_physx_fixed_joint;
  };
}