#pragma once

#include "material_editor_panel.h"

#include <imgui_node_editor.h>

namespace ImGuiNodeEditor = ax::NodeEditor;

namespace retro::material_editor
{
	struct editor_link_info
	{
		ImGuiNodeEditor::LinkId id;
		ImGuiNodeEditor::PinId  input_id;
		ImGuiNodeEditor::PinId  output_id;
	};


	class editor_graph_panel : public material_editor_panel {
	public:
		editor_graph_panel();
		~editor_graph_panel() override;

		void initialize_editor_graph();

		void on_render_panel() override;
	private:
		/* ImGui Node Editor */
		ImGuiNodeEditor::EditorContext* m_editor_context = nullptr;
		ImVector<editor_link_info>   m_editor_links;
		int                  m_editor_next_link_id = 100;
	};
}