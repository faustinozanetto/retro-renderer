#pragma once

#include "renderer/lighting/light.h"

namespace retro::renderer
{
    class point_light : public light
    {
    public:
        point_light(const glm::vec3 &position, const glm::vec3& diffuse, const glm::vec3& specular) : light(light_type::point, diffuse, specular), m_position(position) {}

        /* Getters */
        glm::vec3 get_position() const { return m_position; }

        /* Setters */
        void set_position(const glm::vec3 &position) { m_position = position; }

    private:
        glm::vec3 m_position;
    };
}