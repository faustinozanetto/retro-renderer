#pragma once

#include <glm/glm.hpp>

namespace retro::math
{
    class bounding_box
    {
    public:
        bounding_box();

        bounding_box(
            const glm::vec3 &min_extents,
            const glm::vec3 &max_extents);

        void set_min_extents(const glm::vec3 &min_extents);
        void set_max_extents(const glm::vec3 &max_extents);

        const glm::vec3 &get_min_extents() const { return m_min_extents; }
        const glm::vec3 &get_max_extents() const { return m_max_extents; }

    private:
        glm::vec3 m_min_extents;
        glm::vec3 m_max_extents;
    };
};