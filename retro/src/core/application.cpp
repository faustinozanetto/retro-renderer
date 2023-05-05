#include "rtpch.h"

#include "application.h"

#include "renderer/renderer/renderer.h"

namespace retro::core
{
    application *application::s_instance = nullptr;

    application::application()
    {
        s_instance = this;
        m_window = std::make_shared<renderer::window>(1280, 720, "Retro Renderer");
        renderer::renderer::initialize();
    }

    application::~application()
    {
    }

    void application::main_loop()
    {
        while (!renderer::renderer::get_window_should_close())
        {
        }
    }
}
