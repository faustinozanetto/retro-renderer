#pragma once

#include "scene/actors/components/base_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace retro::scene
{
    struct transform_component : public base_component
    {
    public:
        transform_component(const glm::vec3 &location = glm::vec3(0.0f), const glm::quat&rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f));
        ~transform_component() override;

        /* Getters */
        const glm::vec3 &get_location() const { return m_location; }
        const glm::quat &get_rotation() const { return m_rotation; }
        const glm::vec3 &get_scale() const { return m_scale; }
        glm::mat4 get_transform();

        /* Setters */
        void set_location(const glm::vec3 &location) { m_location = location; }
        void set_rotation(const glm::quat &rotation) { m_rotation = rotation; }
        void set_scale(const glm::vec3 &scale) { m_scale = scale; }

    private:
        glm::vec3 m_location;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };
}