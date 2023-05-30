#include "rtpch.h"
#include "input.h"

#include "core/application.h"
#include "renderer/window/window.h"

namespace retro
{
    bool input::is_key_pressed(key_code key_code)
    {
        GLFWwindow* window = core::application::get().get_window()->get_handle();
        const int state = glfwGetKey(window, key_code);
        return state == GLFW_PRESS;
    }

    glm::vec2 input::get_mouse_position()
    {
        GLFWwindow* window = core::application::get().get_window()->get_handle();
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return {static_cast<float>(x_pos), static_cast<float>(y_pos)};
    }
} // namespace input