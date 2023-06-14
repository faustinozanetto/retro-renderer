#include "rtpch.h"
#include "input_manager.h"

#include "core/application.h"
#include "renderer/window/window.h"

namespace retro::input
{
    bool input_manager::is_key_pressed(key key_code)
    {
        RT_PROFILE_SECTION("input_manager::is_key_pressed");
        GLFWwindow* window = core::application::get().get_window()->get_handle();
        const int state = glfwGetKey(window, key_code);
        return state == GLFW_PRESS;
    }

    bool input_manager::is_mouse_button_pressed(mouse_button button_code)
    {
        RT_PROFILE_SECTION("input_manager::is_mouse_button_pressed");
        GLFWwindow* window = core::application::get().get_window()->get_handle();
        const int state = glfwGetMouseButton(window, static_cast<int32_t>(button_code));
		return state == GLFW_PRESS;
    }

    glm::vec2 input_manager::get_mouse_position()
    {
        RT_PROFILE_SECTION("input_manager::get_mouse_position");
        GLFWwindow* window = core::application::get().get_window()->get_handle();
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return {static_cast<float>(x_pos), static_cast<float>(y_pos)};
    }
}