#pragma once

namespace retro::ui
{
    class engine_ui
    {
    public:
        static void initialize();

        static void set_color_scheme();

        static void begin_frame();
        static void end_frame();

    private:
        static void initialize_imgui();
    };
}