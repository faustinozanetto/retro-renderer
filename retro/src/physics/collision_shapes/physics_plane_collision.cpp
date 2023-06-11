#include "rtpch.h"
#include "physics_plane_collision.h"

namespace retro::physics
{
    physics_plane_collision::physics_plane_collision(physx::PxMaterial *material)
    {
        m_shape = physics_world::get().get_physics()->createShape(physx::PxPlaneGeometry(), *material);
    }

    physics_plane_collision::~physics_plane_collision()
    {
        m_shape->release();
    }

    physx::PxShape *physics_plane_collision::get_physx_shape() const
    {
        return m_shape;
    }
}