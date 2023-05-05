#include "rtpch.h"
#include "window.h"

namespace retro::renderer
{
    window::window(int width, int height, std::string title)
    {
        m_data.width = width;
        m_data.height = height;
        m_data.title = title;
    }

    window::~window()
    {
    }
}