#pragma once

#include "../editor_panel.h"
#include "editor_console_message.h"

#include <memory>
#include <vector>

namespace retro::editor
{
    class editor_console_panel : public editor_panel
    {
    public:
        editor_console_panel();
        ~editor_console_panel() override;

        void on_render_panel() override;

        /* Functions */
        void add_message(const std::shared_ptr<editor_console_message>& message);

    private:
        std::vector<std::shared_ptr<editor_console_message>> m_messages;
    };
}