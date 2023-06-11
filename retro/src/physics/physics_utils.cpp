#include "rtpch.h"
#include "physics_utils.h"

namespace retro::physics
{
    glm::vec2 physics_utils::convert_physx_vec2_to_glm(const physx::PxVec2& physx_vec2)
    {
        return glm::vec2(physx_vec2.x, physx_vec2.y);
    }

    glm::vec3 physics_utils::convert_physx_vec3_to_glm(const physx::PxVec3 &physx_vec3)
    {
        return glm::vec3(physx_vec3.x, physx_vec3.y, physx_vec3.z);
    }

    physx::PxVec2 physics_utils::convert_glm_vec2_to_physx(const glm::vec2& glm_vec2)
    {
        return physx::PxVec2(glm_vec2.x, glm_vec2.y);
    }

    physx::PxVec3 physics_utils::convert_glm_vec3_to_physx(const glm::vec3& glm_vec3)
    {
        return physx::PxVec3(glm_vec3.x, glm_vec3.y, glm_vec3.z);
    }
}