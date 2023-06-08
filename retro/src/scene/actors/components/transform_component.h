#pragma once

#include "scene/actors/components/base_component.h"

#include <glm/glm.hpp>

namespace retro::scene
{
    class transform_component : public base_component
    {
    public:
        transform_component(const glm::vec3 &location = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f), const glm::vec3 &scale = glm::vec3(1.0f));
        ~transform_component() override;

        /* Getters */
        glm::vec3 &get_location() { return m_location; }
        glm::vec3 &get_rotation() { return m_rotation; }
        glm::vec3 &get_scale() { return m_scale; }
        glm::mat4 get_transform();

        /* Setters */
        void set_location(const glm::vec3 &location) { m_location = location; }
        void set_rotation(const glm::vec3 &rotation) { m_rotation = rotation; }
        void set_scale(const glm::vec3 &scale) { m_scale = scale; }

    private:
        glm::vec3 m_location;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
    };
}