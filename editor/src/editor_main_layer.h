#pragma once

#include "panels/editor_panel.h"
#include <vector>
#include <memory>

namespace retro::editor
{
    class editor_main_layer
    {
    public:
        editor_main_layer();
        ~editor_main_layer();

        void initialize();

        void on_update();

        void begin_dockspace();
        void end_dockspace();

    private:
        std::vector<std::shared_ptr<editor_panel>> m_panels;
    };
}