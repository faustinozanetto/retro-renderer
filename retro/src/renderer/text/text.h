#pragma once

#include <glm/glm.hpp>

namespace retro::renderer
{
    struct text_vertex {
        glm::vec4 pos_tex_coords;
    };

    class text
    {
    public:
        text(const std::string &content, const glm::vec2 &position, const glm::vec2 &scale = glm::vec2(1.0f), const glm::vec3 &color = glm::vec3(1.0f));
        ~text();

        /* Getters */
        std::string get_content() const { return m_content; }
        glm::vec3 get_color() const { return m_color; }
        glm::vec2 get_position() const { return m_position; }
        glm::vec2 get_scale() const { return m_scale; }

        /* Setters */
        void set_content(const std::string &content) { m_content = content; }
        void set_color(const glm::vec3 &color) { m_color = color; }
        void set_posiiton(const glm::vec2 &position) { m_position = position; }
        void set_scale(const glm::vec2 &scale) { m_scale = scale; }

    private:
        std::string m_content;
        glm::vec3 m_color;
        glm::vec2 m_position;
        glm::vec2 m_scale;
    };
};