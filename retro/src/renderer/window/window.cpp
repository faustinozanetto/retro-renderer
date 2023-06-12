#include "rtpch.h"
#include "window.h"

#include "events/key_events.h"
#include "events/mouse_events.h"

namespace retro::renderer
{
    window::window(const window_specification &window_specification)
    {
        m_data.title = window_specification.title;
        m_data.width = window_specification.width;
        m_data.height = window_specification.height;
        RT_TRACE("Retro Renderer | Window Initialization");
        RT_TRACE("   - Title: {0}", m_data.title);
        RT_TRACE("   - Width: {0}", m_data.width);
        RT_TRACE("   - Height: {0}", m_data.height);

        initialize();
    }

    window::~window()
    {
    }

    void window::initialize()
    {
        const int glfwInitializeState = glfwInit();
        RT_ASSERT_MSG(glfwInitializeState != GLFW_FALSE, "Could not initialize GLFW!");

        // GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        // Window handle creation
        m_handle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

        // Store data pointer
        glfwSetWindowUserPointer(m_handle, &m_data);

        setup_callbacks();

        RT_TRACE("Retro Renderer | Window initialization completed.");
    }

    void window::setup_callbacks()
    {
        // 1. Resize callback
        glfwSetWindowSizeCallback(m_handle, [](GLFWwindow *native_window, int width, int height)
                                  {
            window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(native_window));
            data.width = width;
            data.height = height;

            events::window_resize_event event({width, height});
            data.event_func(event); });

        // 2. Key callback
        glfwSetKeyCallback(m_handle, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
            const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                {
                    events::key_pressed_event event((input::key)key, false);
                    data.event_func(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    events::key_released_event event((input::key)key);
                    data.event_func(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    events::key_pressed_event event((input::key)key, true);
                    data.event_func(event);
                    break;
                }
            } });

        // 3. Mouse button callback
        glfwSetMouseButtonCallback(m_handle, [](GLFWwindow *window, int button, int action, int mods)
                                   {
		      const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
                    events::mouse_button_pressed_event event((input::mouse_button)button);
					data.event_func(event);
					break;
				}
				case GLFW_RELEASE:
				{
				 events::mouse_button_released_event event((input::mouse_button)button);
					data.event_func(event);
					break;
				}
			} });

        // 4. Mouse scroll callback
        glfwSetScrollCallback(m_handle, [](GLFWwindow* window, double x_offset, double y_offset)
            {
                const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

                events::mouse_scrolled_event event((float)x_offset, (float)y_offset);
                data.event_func(event); });

        // 5. Mouse move callback
		glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double x_pos, double y_pos)
			{
                const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

				events::mouse_moved_event event((float)x_pos, (float)y_pos);
				data.event_func(event);
			});
    }

    void window::set_event_function(const std::function<void(events::base_event &)> func)
    {
        m_data.event_func = func;
    }
}
