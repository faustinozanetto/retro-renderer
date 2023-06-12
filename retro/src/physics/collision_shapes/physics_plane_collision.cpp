#include "rtpch.h"
#include "physics_plane_collision.h"

namespace retro::physics
{
    physics_plane_collision::physics_plane_collision(const std::shared_ptr<retro::physics::physics_material>& physics_material) : physics_collision_shape(physics_material)
    {
        m_shape = physics_world::get().get_physics()->createShape(physx::PxPlaneGeometry(), *m_physics_material->get_physx_material());
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