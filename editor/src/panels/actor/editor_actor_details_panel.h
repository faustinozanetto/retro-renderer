#pragma once

#include "../editor_panel.h"

#include <vector>
#include <memory>
#include <string>

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
        void render_add_component();

        template <typename T>
        void render_add_component_entry(const std::string &component_name) const;

        std::vector<std::shared_ptr<editor_panel>> m_component_panels;
    };
}
