#pragma once

namespace retro::ui
{
    class engine_ui
    {
    public:
        static void initialize();

        static void begin_frame();
        static void end_frame();

    private:
        static void initialize_imgui();
    };
}