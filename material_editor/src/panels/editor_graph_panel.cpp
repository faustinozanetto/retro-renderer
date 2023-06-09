#include "editor_graph_panel.h"


namespace retro::material_editor {
	editor_graph_panel::editor_graph_panel()
	{
		initialize_editor_graph();
	}

	editor_graph_panel::~editor_graph_panel()
	{
	}

	void editor_graph_panel::initialize_editor_graph()
	{
		ImGuiNodeEditor::Config config;
		m_editor_context = ImGuiNodeEditor::CreateEditor(&config);
	}

	void editor_graph_panel::on_render_panel()
	{
		ImGuiNodeEditor::SetCurrentEditor(m_editor_context);
		ImGuiNodeEditor::Begin("Material Editor", ImVec2(0.0, 0.0f));

		ImGuiNodeEditor::End();
		ImGuiNodeEditor::SetCurrentEditor(nullptr);
	}
}