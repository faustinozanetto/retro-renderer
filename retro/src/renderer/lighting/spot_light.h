#pragma once

#include "renderer/lighting/light.h"

namespace retro::renderer
{
    class spot_light : public light
    {
    public:
        spot_light(const glm::vec3 &position, const glm::vec3 &direction, float inner_angle, float outer_angle, const glm::vec3 &color) : light(light_type::point, color), m_position(position), m_direction(direction), m_inner_angle(inner_angle), m_outer_angle(outer_angle) {}

        /* Getters */
        glm::vec3 get_position() const { return m_position; }
        glm::vec3 get_direction() const { return m_direction; }
        float get_inner_angle() const { return m_inner_angle; }
        float get_outer_angle() const { return m_outer_angle; }

        /* Setters */
        void set_position(const glm::vec3 &position) { m_position = position; }
        void set_direction(const glm::vec3 &direction) { m_direction = direction; }
        void set_inner_angle(float angle) { m_inner_angle = angle; }
        void set_outer_angle(float angle) { m_outer_angle = angle; }

    private:
        glm::vec3 m_position;
        glm::vec3 m_direction;
        float m_inner_angle;
        float m_outer_angle;
    };
}