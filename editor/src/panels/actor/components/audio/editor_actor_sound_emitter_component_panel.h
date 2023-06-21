#pragma once

#include "../editor_actor_component_panel.h"

namespace retro::editor
{
    class editor_actor_sound_emitter_component_panel : public editor_actor_component_panel
    {
    public:
        editor_actor_sound_emitter_component_panel();
        
        std::pair<bool, size_t> get_actor_component_details() override;
        void on_render_component_details() override;
        void on_remove_component() override;
    };
}