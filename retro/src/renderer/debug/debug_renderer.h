#pragma once

#include "physics/collision_shapes/physics_box_collision.h"
#include "math/bounding_box.h"
#include "math/transform/transform.h"

#include <glm/glm.hpp>

namespace retro::renderer
{
    struct debug_line
    {
        glm::vec3 point_a;
        glm::vec3 point_b;
        glm::vec3 color;

        debug_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color) : point_a(point_a), point_b(point_b), color(color) {}
    };

	struct debug_line_vertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

    struct debug_renderer_data
    {
        std::vector<debug_line> debug_lines;
    };

    class debug_renderer
    {
    public:
        static void initialize();

        /* Getters */
        static std::vector<debug_line>& get_debug_lines();

        /* Functions */
        static void reset();

       // static void begin_render(const std::shared_ptr<camera::camera>& camera);
        //static void end_render();

        static void submit_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color);
        static void submit_bounding_box(const math::bounding_box& bounding_box, math::transform transform = math::transform(), const glm::vec3& color = glm::vec3(0.85f));

        static void submit_physics_box_collision(const std::shared_ptr<physics::physics_box_collision>& physics_box_collision,
                                                 const math::transform& transform = math::transform(), const glm::vec3& color = glm::vec3(0.85f));
    private:
        static debug_renderer_data s_data;
    };
}
