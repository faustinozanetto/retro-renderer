#include "rtpch.h"
#include "physics_capsule_collision.h"

namespace retro::physics
{
    physics_capsule_collision::physics_capsule_collision(const std::shared_ptr<retro::physics::physics_material> &physics_material, float radius, float half_height) : physics_collision_shape(physics_material)
    {
        m_radius = radius;
        m_half_height = half_height;
        m_shape = physics_world::get().get_physics()->createShape(physx::PxCapsuleGeometry(radius, half_height), *m_physics_material->get_physx_material());
    }

    physics_capsule_collision::~physics_capsule_collision()
    {
        if (m_shape)
        {
            // Release the existing shape
            m_shape->release();
            m_shape = nullptr;
        }
    }

    physx::PxShape *physics_capsule_collision::get_physx_shape() const
    {
        RT_PROFILE;
        return m_shape;
    }

    void physics_capsule_collision::set_radius(float radius)
    {
        m_radius = radius;

        // Recreate the PxShape with the new radius
        if (m_shape)
        {
            // Release the existing shape
            m_shape->release();
            m_shape = nullptr;
        }

        // Recreate the shape using the new radius
        m_shape = physics_world::get().get_physics()->createShape(physx::PxCapsuleGeometry(m_radius, m_half_height),
                                                                  *m_physics_material->get_physx_material());
    }

    void physics_capsule_collision::set_half_height(float half_height)
    {
        m_half_height = half_height;

        // Recreate the PxShape with the new half height
        if (m_shape)
        {
            // Release the existing shape
            m_shape->release();
            m_shape = nullptr;
        }

        // Recreate the shape using the new  half height
        m_shape = physics_world::get().get_physics()->createShape(physx::PxCapsuleGeometry(m_radius, m_half_height),
                                                                  *m_physics_material->get_physx_material());
    }
}