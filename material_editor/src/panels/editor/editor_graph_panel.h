#pragma once

#include "../material_editor_panel.h"
#include "nodes/editor_graph_node.h"

#include <imgui_node_editor.h>

#include <string>
#include <vector>
#include <memory>

namespace ImGuiNodeEditor = ax::NodeEditor;

static int m_editor_next_link_id = 100;

namespace retro::material_editor
{

	class editor_graph_panel : public material_editor_panel
	{
	public:
		editor_graph_panel();
		~editor_graph_panel() override;

		/* Functions */
		void initialize_editor_graph();
		void render_graph_nodes();

		/* Getters */
		graph_node_pin *get_pin_by_id(ImGuiNodeEditor::PinId id);
		static int get_next_id() { return m_editor_next_link_id++; }

		void on_render_panel() override;

	private:
		/* ImGui Node Editor */
		ImGuiNodeEditor::EditorContext *m_editor_context = nullptr;

		std::vector<std::shared_ptr<editor_graph_node>> m_editor_nodes;
		std::vector<graph_node_link> m_editor_links;

		bool m_create_new_node;
		graph_node_pin *m_new_node_link_pin;
		graph_node_pin *m_new_link_pin;
	};
}