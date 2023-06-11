#include "rtpch.h"
#include "physics_box_collision.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_box_collision::physics_box_collision(physx::PxMaterial *material, const glm::vec3 &half_extents)
    {
        m_half_extents = half_extents;
        m_shape = physics_world::get().get_physics()->createShape(physx::PxBoxGeometry(physics_utils::convert_glm_vec3_to_physx(half_extents)), *material);
    }

    physics_box_collision::~physics_box_collision()
    {
        m_shape->release();
    }

    physx::PxShape *physics_box_collision::get_physx_shape() const
    {
        return m_shape;
    }
}