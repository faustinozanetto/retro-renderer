#pragma once

#include <glm/glm.hpp>

namespace retro::renderer
{
    enum class light_type
    {
        point,
        spot,
        directional,
    };

    class light
    {
    public:
        light(light_type type, const glm::vec3 &color) : m_type(type), m_color(color) {}

        /* Getters*/
        light_type get_type() const { return m_type; }
        glm::vec3 get_color() const { return m_color; }

        /* Setters */
        void set_color(const glm::vec3 &color) { m_color = color; }

    private:
        light_type m_type;
        glm::vec3 m_color;
    };
}