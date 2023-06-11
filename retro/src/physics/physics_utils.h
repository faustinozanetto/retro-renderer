#pragma once

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

namespace retro::physics
{
    class physics_utils
    {
    public:
        static glm::vec2 convert_physx_vec2_to_glm(const physx::PxVec2& physx_vec2);
        static glm::vec3 convert_physx_vec3_to_glm(const physx::PxVec3& physx_vec3);

        static physx::PxVec2 convert_glm_vec2_to_physx(const glm::vec2& glm_vec2);
        static physx::PxVec3 convert_glm_vec3_to_physx(const glm::vec3& glm_vec3);
    };
}