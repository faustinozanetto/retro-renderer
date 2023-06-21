#include "editor_renderer_panel.h"

#include "../editor_app.h"
#include "../utils/editor_ui_utils.h"

#include <core/application.h>

namespace retro::editor
{
	editor_renderer_panel::editor_renderer_panel() : editor_panel("Renderer")
	{
	}

	editor_renderer_panel::~editor_renderer_panel() {}

	void editor_renderer_panel::on_render_panel()
	{
		RT_PROFILE;
		ImGui::Begin("Renderer", &m_show);
		bool use_vsync = renderer::renderer::get_is_vsync_enabled();
		if (editor_ui_utils::draw_property("VSync Enabled", use_vsync))
		{
			renderer::renderer::set_vsync_enabled(use_vsync);
		}
		ImGui::End();
	}
}