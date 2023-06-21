#pragma once

#include "profiling/profiling.h"
#include <string>
#include <imgui.h>

namespace retro::editor
{
    class editor_panel
    {
    public:
        editor_panel(const std::string& name) : m_name(name), m_show(true)
        {
        }

        virtual ~editor_panel() = default;

        /* Getters */
        bool& get_show_panel() { return m_show; }
        const std::string& get_name() const { return m_name; }

        /* Setters */
        void set_show_panel(bool show_panel) { m_show = show_panel; }

        /* Functions */
        virtual void on_render_panel() = 0;

    protected:
        bool m_show;
        std::string m_name;
    };
}
