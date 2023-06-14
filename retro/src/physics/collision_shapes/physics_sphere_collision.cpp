#include "rtpch.h"
#include "physics_sphere_collision.h"

namespace retro::physics
{
    physics_sphere_collision::physics_sphere_collision(const std::shared_ptr<retro::physics::physics_material>& physics_material, float radius) : physics_collision_shape(physics_material)
    {
        m_radius = radius;
		m_shape = physics_world::get().get_physics()->createShape(physx::PxSphereGeometry(radius), *m_physics_material->get_physx_material());
    }

    physics_sphere_collision::~physics_sphere_collision()
    {
        m_shape->release();
    }

    physx::PxShape* physics_sphere_collision::get_physx_shape() const
    {
        RT_PROFILE_SECTION("physics_sphere_collision::get_physx_shape");
        return m_shape;
    }
}