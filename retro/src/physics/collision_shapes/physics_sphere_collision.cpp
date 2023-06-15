#include "rtpch.h"
#include "physics_sphere_collision.h"

namespace retro::physics
{
    physics_sphere_collision::physics_sphere_collision(const std::shared_ptr<retro::physics::physics_material> &physics_material, float radius) : physics_collision_shape(physics_material)
    {
        m_radius = radius;
        m_shape = physics_world::get().get_physics()->createShape(physx::PxSphereGeometry(radius), *m_physics_material->get_physx_material());
    }

    physics_sphere_collision::~physics_sphere_collision()
    {
        if (m_shape)
        {
            // Release the existing shape
            m_shape->release();
            m_shape = nullptr;
        }
    }

    physx::PxShape *physics_sphere_collision::get_physx_shape() const
    {
        RT_PROFILE_SECTION("physics_sphere_collision::get_physx_shape");
        return m_shape;
    }

    void physics_sphere_collision::set_radius(float radius)
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
        m_shape = physics_world::get().get_physics()->createShape(physx::PxSphereGeometry(m_radius),
                                                                  *m_physics_material->get_physx_material());
    }
}