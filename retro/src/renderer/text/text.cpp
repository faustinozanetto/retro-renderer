#include "rtpch.h"
#include "text.h"

namespace retro::renderer
{
    text::text(const std::string &content, const glm::vec2 &position, const glm::vec2 &scale, const glm::vec3 &color)
    {
        m_content = content;
        m_color = color;
        m_position = position;
        m_scale = scale;
    }

    text::~text()
    {
    }
}