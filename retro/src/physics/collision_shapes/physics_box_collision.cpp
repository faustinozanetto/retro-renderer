#include "rtpch.h"
#include "physics_box_collision.h"

#include "physics/physics_utils.h"

namespace retro::physics
{
    physics_box_collision::physics_box_collision(const std::shared_ptr<retro::physics::physics_material>& physics_material, const glm::vec3 &half_extents) : physics_collision_shape(physics_material)
    {
        m_half_extents = half_extents;
        m_shape = physics_world::get().get_physics()->createShape(physx::PxBoxGeometry(physics_utils::convert_glm_vec3_to_physx(half_extents)), *m_physics_material->get_physx_material());
    }

    physics_box_collision::~physics_box_collision()
    {
        m_shape->release();
    }

    physx::PxShape *physics_box_collision::get_physx_shape() const
    {
        RT_PROFILE_SECTION("physics_box_collision::get_physx_shape");
        return m_shape;
    }
}