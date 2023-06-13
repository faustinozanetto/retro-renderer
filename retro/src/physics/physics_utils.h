#pragma once

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include "glm/ext/quaternion_float.hpp"

namespace retro::physics
{
    class physics_utils
    {
    public:
        static glm::vec2 convert_physx_vec2_to_glm(const physx::PxVec2 &physx_vec2);
        static glm::vec3 convert_physx_vec3_to_glm(const physx::PxVec3 &physx_vec3);
        static glm::quat convert_physx_quat_to_glm(const physx::PxQuat &physx_quat);

        static physx::PxVec2 convert_glm_vec2_to_physx(const glm::vec2 &glm_vec2);
        static physx::PxVec3 convert_glm_vec3_to_physx(const glm::vec3 &glm_vec3);
        static physx::PxQuat convert_glm_quat_to_physx(const glm::quat &glm_quat);
        static physx::PxQuat convert_glm_vec3_to_physx_quat(const glm::vec3 &glm_vec3);

        static physx::PxTransform create_transform_from_glm(const glm::vec3 &position = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    };
}