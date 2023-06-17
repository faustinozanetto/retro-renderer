#pragma once

#include "panels/editor_panel.h"
#include "panels/console/editor_console_panel.h"

#include <retro.h>

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace retro::editor
{
    class editor_main_layer
    {
    public:
        editor_main_layer();
        ~editor_main_layer();

        /* Getters */
        uint32_t get_main_render_target() const { return m_geometry_fbo->get_attachment_id(1); }
        const std::shared_ptr<camera::camera> &get_camera() const { return m_camera; }
        const std::shared_ptr<editor_console_panel>& get_console_panel() const { return m_console_panel; }

        /* Initialization */
        void initialize();
        void setup_camera();
        void setup_scene();

        void on_render();
        void on_update();

        void begin_dockspace();
        void end_dockspace();

        /* Scene */
        static entt::entity s_selected_actor;

    private:
        /* Rendering */
        uint32_t m_main_render_target;

		std::shared_ptr<renderer::model> m_model;

        /* Model Variables */
        std::shared_ptr<retro::renderer::shader> m_shader;
        std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;
        std::shared_ptr<retro::scene::scene_actor> m_demo_actor;

        /* Camera */
        std::shared_ptr<camera::camera> m_camera;

        /* UI */
        std::vector<std::shared_ptr<editor_panel>> m_panels;
        std::shared_ptr<editor_console_panel> m_console_panel;

        bool m_initialized;
    };
}