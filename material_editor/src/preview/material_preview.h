#pragma once

#include <retro.h>
#include <memory>

namespace retro::material_editor
{
    class material_preview
    {
    public:
        material_preview();
        ~material_preview();

        /* Getters */
        int get_final_render_target();
        std::shared_ptr<scene::scene_actor>& get_preview_actor() { return m_preview_actor; }

        /* Functions */
        void setup_fbo();
        void setup_shaders();
        void setup_camera();
        void setup_model();
        void setup_scene();

        void on_material_selected(const std::shared_ptr<renderer::material>& material);

        void render_preview();
        
    private:
        /* Common Variables */
        std::shared_ptr<scene::scene_actor> m_preview_actor;
        std::shared_ptr<scene::scene> m_preview_scene;
        std::shared_ptr<camera::camera> m_camera;

		/* Model Variables */
		std::shared_ptr<renderer::shader> m_geometry_shader;
		std::shared_ptr<renderer::frame_buffer> m_geometry_fbo;
        std::shared_ptr<renderer::model> m_preview_model;
    };
}