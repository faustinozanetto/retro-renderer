#include "rtpch.h"
#include "physics_capsule_collision.h"

namespace retro::physics
{
    physics_capsule_collision::physics_capsule_collision(physx::PxMaterial *material, float radius, float half_height)
    {
        m_radius = radius;
        m_half_height = half_height;
        m_shape = physics_world::get().get_physics()->createShape(physx::PxCapsuleGeometry(radius, half_height), *material);
    }

    physics_capsule_collision::~physics_capsule_collision()
    {
        m_shape->release();
    }

    physx::PxShape *physics_capsule_collision::get_physx_shape() const
    {
        return m_shape;
    }
}