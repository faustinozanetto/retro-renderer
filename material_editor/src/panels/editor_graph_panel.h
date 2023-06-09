#pragma once

#include "material_editor_panel.h"

#include <imgui_node_editor.h>

#include <string>
#include <vector>

namespace ImGuiNodeEditor = ax::NodeEditor;

namespace retro::material_editor
{
	enum class editor_graph_node_pin_type {
		p_float,
		p_texture
	};

	enum class editor_grpah_node_pin_kind {
		input,
		output,
	};

	struct editor_graph_node;

	struct editor_graph_node_pin {
		ImGuiNodeEditor::PinId id;
		std::string name;
		editor_graph_node* node;
		editor_graph_node_pin_type type;
		editor_grpah_node_pin_kind kind;

		editor_graph_node_pin(const std::string& name, int id, editor_graph_node_pin_type pin_type) : name(name), 
			id(id), type(pin_type), kind(editor_grpah_node_pin_kind::input) {}
	};

	struct editor_graph_node {
		ImGuiNodeEditor::NodeId id;
		std::string name;
		std::vector<editor_graph_node_pin> inputs;
		std::vector<editor_graph_node_pin> outputs;

		editor_graph_node(const std::string& name, int id) : name(name), id(id) {}
	};

	struct editor_graph_node_link {
		ImGuiNodeEditor::LinkId id;

		ImGuiNodeEditor::PinId start_id;
		ImGuiNodeEditor::PinId end_id;

		editor_graph_node_link(ImGuiNodeEditor::LinkId id, ImGuiNodeEditor::PinId start_id, ImGuiNodeEditor::PinId end_id) :
			id(id), start_id(start_id), end_id(end_id) {}
	};

	class editor_graph_panel : public material_editor_panel {
	public:
		editor_graph_panel();
		~editor_graph_panel() override;

		/* Functions */
		void initialize_editor_graph();
		void render_graph_nodes();

		void construct_material_node();

		/* Getters */
		editor_graph_node_pin* get_pin_by_id(ImGuiNodeEditor::PinId id);

		void on_render_panel() override;

	private:
		/* ImGui Node Editor */
		ImGuiNodeEditor::EditorContext* m_editor_context = nullptr;
		int                  m_editor_next_link_id = 100;
		std::vector<editor_graph_node> m_editor_nodes;
		std::vector<editor_graph_node_link> m_editor_links;
		bool m_create_new_node;
		editor_graph_node_pin* m_new_node_link_pin;
		editor_graph_node_pin* m_new_link_pin;
	};
}