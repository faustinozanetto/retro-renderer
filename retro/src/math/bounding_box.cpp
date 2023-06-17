#include "rtpch.h"
#include "bounding_box.h"

namespace retro::math
{
    bounding_box::bounding_box()
    {
        m_min_extents = glm::vec3(FLT_MAX);
        m_max_extents = glm::vec3(-FLT_MAX);
    }

    bounding_box::bounding_box(const glm::vec3 &min_extents, const glm::vec3 &max_extents)
    {
        m_min_extents = min_extents;
        m_max_extents = max_extents;
    }

    void bounding_box::set_min_extents(const glm::vec3 &min_extents)
    {
        m_min_extents = min_extents;
    }

    void bounding_box::set_max_extents(const glm::vec3 &max_extents)
    {
        m_max_extents = max_extents;
    }
};