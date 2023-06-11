#include "rtpch.h"
#include "physics_sphere_collision.h"

namespace retro::physics
{
    physics_sphere_collision::physics_sphere_collision(physx::PxPhysics* physics_world, physx::PxMaterial* material, float radius)
    {
        m_radius = radius;
		m_shape = physics_world->createShape(physx::PxSphereGeometry(radius), *material);
    }

    physics_sphere_collision::~physics_sphere_collision()
    {
        m_shape->release();
    }

    physx::PxShape* physics_sphere_collision::get_physx_shape() const
    {
        return m_shape;
    }
}