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
        light(light_type type, const glm::vec3 &diffuse, const glm::vec3& specular) : m_type(type), m_diffuse(diffuse), m_specular(specular) {}

        /* Getters*/
        light_type get_type() const { return m_type; }
        glm::vec3 get_diffuse() const { return m_diffuse; }
        glm::vec3 get_specular() const { return m_specular; }

        /* Setters */
        void set_diffuse(const glm::vec3 & diffuse) { m_diffuse = diffuse; }
        void set_specular(const glm::vec3 & specular) { m_specular = specular; }

        static std::string get_light_type_to_string(light_type type);

    private:
        light_type m_type;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
    };
}