#include "rtpch.h"
#include "camera_controller.h"

namespace retro::camera
{
    camera_controller::camera_controller(const std::shared_ptr<camera> &camera)
    {
        m_camera = camera;
        m_last_mouse_x = 0.0;
        m_last_mouse_y = 0.0;
        m_mouse_sensitivity = 0.1;
        m_movement_speed = 2.5;
    }

    void camera_controller::on_update()
    {

    }

    void camera_controller::set_camera(const std::shared_ptr<camera> &camera) 
    {
        RT_PROFILE;
        m_camera = camera;
    }

    void camera_controller::proccess_keyboard_input(
        camera_keyboard_direction direction)
    {
        RT_PROFILE;
        glm::vec3 updated_position = m_camera->get_position();
        if (direction == camera_keyboard_direction::forward)
        {
            updated_position += m_camera->get_front() * m_movement_speed;
        }
        else if (direction == camera_keyboard_direction::backward)
        {
            updated_position -= m_camera->get_front() * m_movement_speed;
        }
        else if (direction == camera_keyboard_direction::left)
        {
            updated_position -= m_camera->get_right() * m_movement_speed;
        }
        else if (direction == camera_keyboard_direction::right)
        {
            updated_position += m_camera->get_right() * m_movement_speed;
        }
        else if (direction == camera_keyboard_direction::up)
        {
            updated_position += m_camera->get_up() * m_movement_speed;
        }
        else if (direction == camera_keyboard_direction::down)
        {
            updated_position -= m_camera->get_up() * m_movement_speed;
        }
        m_camera->set_position(updated_position);
        m_camera->construct_matrices();
    }

    void camera_controller::process_mouse_input(float dx, float dy)
    {
        RT_PROFILE;
		if (m_is_first_mouse)
		{
			m_last_mouse_x = dx;
			m_last_mouse_y = dy;
            m_is_first_mouse = false;
		}

		float xoffset = dx - m_last_mouse_x;
		float yoffset = m_last_mouse_y - dy; // reversed since y-coordinates go from bottom to top
        m_last_mouse_x = dx;
        m_last_mouse_y = dy;

		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_camera->set_yaw(m_camera->get_yaw() + xoffset);
		m_camera->set_pitch(m_camera->get_pitch() + yoffset);

		RT_TRACE("Yaw: {}, Pitch: {}", m_camera->get_yaw(), m_camera->get_pitch());

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (m_camera->get_pitch() > 89.0f)
            m_camera->set_pitch(89.0f);
		if (m_camera->get_pitch() < -89.0f)
            m_camera->set_pitch(-89.0f);

        m_camera->update_vectors();
        m_camera->construct_matrices();
    }
}