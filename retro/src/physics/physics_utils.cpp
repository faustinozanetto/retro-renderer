#include "rtpch.h"
#include "physics_utils.h"
#include "glm/ext/quaternion_float.hpp"

namespace retro::physics
{
    glm::vec2 physics_utils::convert_physx_vec2_to_glm(const physx::PxVec2 &physx_vec2)
    {
        return glm::vec2(physx_vec2.x, physx_vec2.y);
    }

    glm::vec3 physics_utils::convert_physx_vec3_to_glm(const physx::PxVec3 &physx_vec3)
    {
        return glm::vec3(physx_vec3.x, physx_vec3.y, physx_vec3.z);
    }

    glm::quat physics_utils::convert_physx_quat_to_glm(const physx::PxQuat& physx_quat)
    {
        return glm::quat(physx_quat.w, physx_quat.x, physx_quat.y, physx_quat.z);
    }

    physx::PxVec2 physics_utils::convert_glm_vec2_to_physx(const glm::vec2 &glm_vec2)
    {
        return physx::PxVec2(glm_vec2.x, glm_vec2.y);
    }

    physx::PxVec3 physics_utils::convert_glm_vec3_to_physx(const glm::vec3 &glm_vec3)
    {
        return physx::PxVec3(glm_vec3.x, glm_vec3.y, glm_vec3.z);
    }

    physx::PxQuat physics_utils::convert_glm_quat_to_physx(const glm::quat& glm_quat)
    {
        return physx::PxQuat(glm_quat.x, glm_quat.y, glm_quat.z, glm_quat.w);
    }

    physx::PxQuat physics_utils::convert_glm_vec3_to_physx_quat(const glm::vec3 &glm_vec3)
    {
        glm::quat glm_rotation = glm::quat(glm::radians(glm_vec3));

        return convert_glm_quat_to_physx(glm_rotation);
    }

    physx::PxTransform physics_utils::create_transform_from_glm(const glm::vec3 &position, const glm::vec3 &rotation)
    {
        return physx::PxTransform(convert_glm_vec3_to_physx(position), convert_glm_vec3_to_physx_quat(rotation));
    }
}