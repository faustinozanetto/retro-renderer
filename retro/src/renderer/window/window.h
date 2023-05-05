#pragma once

namespace retro::renderer
{
    struct window_data
    {
        int width;
        int height;
        std::string title;
    };

    class window
    {
    public:
        window(int width, int height, std::string title);
        ~window();

    private:
        window_data m_data;
    };
}