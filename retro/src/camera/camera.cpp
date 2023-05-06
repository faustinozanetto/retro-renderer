#include "rtpch.h"
#include "camera.h"

#include "renderer/renderer/renderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace retro::camera
{
    camera::camera(camera_type type, float fov, float near_plane, float far_plane)
    {
        RT_TRACE("Retro Renderer | Camera initialization started.");
        m_type = type;
        m_fov = fov;
        m_near_plane = near_plane;
        m_far_plane = far_plane;
        m_yaw = -90.0f;
        m_pitch = 0.0f;

        m_position = glm::vec3(0.0f);
        m_world_up = glm::vec3(0, 1, 0);
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);

        RT_TRACE("  - Type: {0}", get_camera_type_to_string(m_type));
        RT_TRACE("  - Fov: {0}", m_fov);
        RT_TRACE("  - Near Plane: {0}", m_near_plane);
        RT_TRACE("  - Far Plane: {0}", m_far_plane);
        RT_TRACE("  - Yaw: {0}", m_yaw);
        RT_TRACE("  - Pitch: {0}", m_pitch);

        construct_matrices();
        update_vectors();
        RT_TRACE("Retro Renderer | Camera initialization completed.");
    }

    void camera::update_vectors()
    {
        glm::vec3 updated_front;
        updated_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        updated_front.y = sin(glm::radians(m_pitch));
        updated_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = normalize(updated_front);
        m_right = normalize(cross(m_front, m_world_up));
        m_up = normalize(cross(m_right, m_front));
    }

    void camera::construct_matrices()
    {
        if (m_type == camera_type::perspective)
        {
            float aspect_ratio = renderer::renderer::get_viewport_size().x / renderer::renderer::get_viewport_size().y;
            m_projection_matrix = glm::perspective(m_fov, aspect_ratio, m_near_plane, m_far_plane);
            m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
        }
        else
        {
            m_projection_matrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, m_near_plane, m_far_plane);
        }
    }

    void camera::set_fov(float fov)
    {
        m_fov = fov;
        construct_matrices();
    }

    void camera::set_near_plane(float near_plane)
    {
        m_near_plane = near_plane;
        construct_matrices();
    }

    void camera::set_far_plane(float far_plane)
    {
        m_far_plane = far_plane;
        construct_matrices();
    }

    void camera::set_position(const glm::vec3 &position)
    {
        m_position = position;
        construct_matrices();
    }

    std::string camera::get_camera_type_to_string(camera_type type)
    {
        switch (type)
        {
        case camera_type::perspective:
            return "perspective";
        case camera_type::orthographic:
            return "orthographic";
        }
        RT_ASSERT_MSG(false, "Invalid camera type!");
    }
}
