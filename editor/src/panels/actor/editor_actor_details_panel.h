#pragma once

#include "../editor_panel.h"

#include <vector>
#include <memory>

namespace retro::editor
{
    class editor_actor_details_panel : public editor_panel
    {
    public:
        editor_actor_details_panel();
        ~editor_actor_details_panel() override;

        void initialize();

        void on_render_panel() override;

    private:
        std::vector<std::shared_ptr<editor_panel>> m_component_panels;
    };
}