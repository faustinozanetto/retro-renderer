#pragma once

#include <glm/glm.hpp>

namespace retro::camera
{
    enum class camera_type
    {
        perspective,
        orthographic,
    };

    class camera
    {
    public:
        camera(camera_type type, float fov, float near_plane, float far_plane);

        /* Getters */
        camera_type get_type() const { return m_type; }
        float get_fov() const { return m_fov; }
        float get_near_plane() const { return m_near_plane; }
        float get_far_plane() const { return m_far_plane; }
        float get_yaw() const { return m_yaw; }
        float get_pitch() const { return m_pitch; }
        const glm::vec3 &get_position() const { return m_position; }
        const glm::vec3 &get_front() const { return m_front; }
        const glm::vec3 &get_up() const { return m_up; }
        const glm::vec3 &get_right() const { return m_right; }

        const glm::mat4 &get_projection_matrix() const { return m_projection_matrix; }
        const glm::mat4 &get_view_matrix() const { return m_view_matrix; }

        /* Setters */
        void set_fov(float fov);
        void set_near_plane(float near_plane);
        void set_far_plane(float far_plane);
        void set_position(const glm::vec3 &position);
        void set_yaw(float yaw);

        static std::string get_camera_type_to_string(camera_type type);

    private:
        void update_vectors();
        void construct_matrices();

        camera_type m_type;

        float m_fov;
        float m_near_plane;
        float m_far_plane;
        float m_yaw;
        float m_pitch;

        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_world_up;
        glm::vec3 m_right;
    };
}