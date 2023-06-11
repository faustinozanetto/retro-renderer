#include "rtpch.h"
#include "transform_component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace retro::scene
{
    transform_component::transform_component(const glm::vec3 &location, const glm::vec3 &rotation, const glm::vec3 &scale)
    {
        m_location = location;
        m_rotation = rotation;
        m_scale = scale;
    }

    transform_component::~transform_component()
    {
    }

    glm::mat4 transform_component::get_transform()
    {
        // Convert rotation
        glm::mat4 transform = glm::mat4(1.0f);
        glm::mat4 rotation_matrix = glm::toMat4(glm::quat(m_rotation));

        transform = glm::translate(transform, m_location);
        transform *= rotation_matrix;
        transform = glm::scale(transform, m_scale);

        return transform;
    }
}