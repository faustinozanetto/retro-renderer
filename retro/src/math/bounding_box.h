#pragma once

#include "transform/transform.h"

#include <glm/glm.hpp>

namespace retro::math
{
    class bounding_box
    {
    public:
        bounding_box();
        bounding_box(const bounding_box& other);
        bounding_box(
            const glm::vec3 &min_extents,
            const glm::vec3 &max_extents);
        
        /* Getters */
        const glm::vec3 &get_min_extents() const { return m_min_extents; }
        const glm::vec3 &get_max_extents() const { return m_max_extents; }
        glm::vec3 get_size() const;
        glm::vec3 get_center() const;

        /* Setters */
        void set_min_extents(const glm::vec3 &min_extents);
        void set_max_extents(const glm::vec3 &max_extents);

        /* Functions */
        void apply_transform(transform& transform);
        bounding_box get_transformed(transform& transform) const;
    private:
        glm::vec3 m_min_extents;
        glm::vec3 m_max_extents;
    };
};