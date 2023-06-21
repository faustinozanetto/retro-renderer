#pragma once

#include "panels/editor_panel.h"
#include "panels/console/editor_console_panel.h"

#include <retro.h>

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "renderer/scene/scene_renderer.h"

namespace retro::editor
{
    class editor_main_layer
    {
    public:
        editor_main_layer();
        ~editor_main_layer();

        /* Getters */
        uint32_t get_main_render_target() { return renderer::scene_renderer::get_final_render_target(); }
        const std::shared_ptr<camera::camera>& get_camera() const { return m_camera; }
        const std::shared_ptr<editor_console_panel>& get_console_panel() const { return m_console_panel; }
        std::vector<std::shared_ptr<editor_panel>>& get_editor_panels() { return m_panels; }

        /* Initialization */
        void initialize();
        void setup_camera();
        void setup_scene();

        void on_render();
        void on_update();

        void draw_imguizmo();

        void begin_dockspace();
        void end_dockspace();

        /* Scene */
        static scene::scene_actor s_selected_actor;

    private:
        /* Rendering */
        uint32_t m_main_render_target;

        std::shared_ptr<renderer::model> m_model;

        /* Model Variables */
        std::shared_ptr<scene::scene_actor> m_demo_actor;

        /* Camera */
        std::shared_ptr<camera::camera> m_camera;

        /* UI */
        std::vector<std::shared_ptr<editor_panel>> m_panels;
        std::shared_ptr<editor_console_panel> m_console_panel;

        bool m_initialized;
    };
}
