#include "rtpch.h"
#include "debug_renderer.h"

#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
    debug_renderer_data debug_renderer::s_data = {};

    void debug_renderer::initialize()
    {
    }

    std::vector<debug_line>& debug_renderer::get_debug_lines()
    {
        RT_PROFILE;
        return s_data.debug_lines;
    }

    void debug_renderer::reset()
    {
        RT_PROFILE;
        // Clear debug shapes for next frame.
        s_data.debug_lines.clear();
    }
    
    void debug_renderer::submit_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color)
    {
        RT_PROFILE;
        s_data.debug_lines.emplace_back(point_a, point_b, color);
    }

    void debug_renderer::submit_bounding_box(const math::bounding_box& bounding_box, math::transform transform, const glm::vec3& color)
    {
        RT_PROFILE;
        const math::bounding_box& transformed_bounding_box = bounding_box.get_transformed(transform);
        const glm::vec3& min = transformed_bounding_box.get_min_extents();
        const glm::vec3& max = transformed_bounding_box.get_max_extents();

        // Submit the lines that make up the bounding box
        submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color);
        submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), color);
        submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), color);
        submit_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        submit_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color);
        submit_line(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        submit_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), color);
        submit_line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), color);
        submit_line(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z), color);
        submit_line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color);
        submit_line(glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), color);
        submit_line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color);
    }
    
    void debug_renderer::submit_physics_box_collision(const std::shared_ptr<physics::physics_box_collision>& physics_box_collision,
                                                      const math::transform& transform, const glm::vec3& color)
    {
        RT_PROFILE;
        const glm::vec3 box_size = physics_box_collision->get_half_extents();
        submit_bounding_box({-box_size, box_size}, transform, color);
    }
}
