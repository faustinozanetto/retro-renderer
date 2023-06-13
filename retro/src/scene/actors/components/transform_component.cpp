#include "rtpch.h"
#include "transform_component.h"



namespace retro::scene
{
    transform_component::transform_component(const glm::vec3 &location, const glm::quat &rotation, const glm::vec3 &scale)
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

        transform = glm::translate(transform, m_location);
        transform *= glm::toMat4(m_rotation);
        transform = glm::scale(transform, m_scale);

        return transform;
    }
}