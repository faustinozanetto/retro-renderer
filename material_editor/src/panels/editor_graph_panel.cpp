#include "editor_graph_panel.h"

#include <retro.h>

namespace retro::material_editor
{
	editor_graph_panel::editor_graph_panel()
	{
		initialize_editor_graph();
		construct_material_node();
	}

	editor_graph_panel::~editor_graph_panel()
	{
		ImGuiNodeEditor::DestroyEditor(m_editor_context);
	}

	void editor_graph_panel::initialize_editor_graph()
	{
		ImGuiNodeEditor::Config config;
		m_editor_context = ImGuiNodeEditor::CreateEditor(&config);

		{
			editor_graph_node node = editor_graph_node("Test Node", m_editor_next_link_id++);
			editor_graph_node_pin input_pin = editor_graph_node_pin("Texture", m_editor_next_link_id++, editor_graph_node_pin_type::p_texture);
			input_pin.node = &node;
			input_pin.kind = editor_grpah_node_pin_kind::input;

			editor_graph_node_pin output_pin = editor_graph_node_pin("Mixed", m_editor_next_link_id++, editor_graph_node_pin_type::p_texture);
			output_pin.node = &node;
			output_pin.kind = editor_grpah_node_pin_kind::output;

			node.inputs.push_back(input_pin);
			node.outputs.push_back(output_pin);
			m_editor_nodes.push_back(node);
		}
		{
			editor_graph_node node = editor_graph_node("Test Node", m_editor_next_link_id++);
			editor_graph_node_pin input_pin = editor_graph_node_pin("Texture", m_editor_next_link_id++, editor_graph_node_pin_type::p_texture);
			input_pin.node = &node;
			input_pin.kind = editor_grpah_node_pin_kind::input;

			editor_graph_node_pin output_pin = editor_graph_node_pin("Mixed", m_editor_next_link_id++, editor_graph_node_pin_type::p_texture);
			output_pin.node = &node;
			output_pin.kind = editor_grpah_node_pin_kind::output;

			node.inputs.push_back(input_pin);
			node.outputs.push_back(output_pin);
			m_editor_nodes.push_back(node);
		}
		editor_graph_node_link node_link = editor_graph_node_link(m_editor_next_link_id++, m_editor_nodes[0].outputs[0].id, m_editor_nodes[1].inputs[0].id);
		m_editor_links.push_back(node_link);

		m_create_new_node = false;
		m_new_link_pin = nullptr;
		m_new_node_link_pin = nullptr;
	}

	void editor_graph_panel::construct_material_node()
	{
		editor_graph_node node = editor_graph_node("Material Output", m_editor_next_link_id++);
		for (auto texture_type : renderer::material::get_material_texture_types()) {
			editor_graph_node_pin texture_input_pin = editor_graph_node_pin(renderer::material::get_material_texture_type_to_string(texture_type), 
				m_editor_next_link_id++, editor_graph_node_pin_type::p_texture);

			texture_input_pin.node = &node;
			texture_input_pin.kind = editor_grpah_node_pin_kind::input;
			node.inputs.push_back(texture_input_pin);
		}
		m_editor_nodes.push_back(node);
	}

	editor_graph_node_pin* editor_graph_panel::get_pin_by_id(ImGuiNodeEditor::PinId id)
	{
		if (!id)
			return nullptr;

		for (auto& node : m_editor_nodes)
		{
			for (auto& pin : node.inputs)
				if (pin.id == id)
					return &pin;

			for (auto& pin : node.outputs)
				if (pin.id == id)
					return &pin;
		}

		return nullptr;
	}

	void editor_graph_panel::on_render_panel()
	{
		ImGuiNodeEditor::SetCurrentEditor(m_editor_context);
		ImGuiNodeEditor::Begin("Material Editor");

		render_graph_nodes();

		ImGuiNodeEditor::End();
	}

	void editor_graph_panel::render_graph_nodes()
	{
		// Nodes
		for (auto& node : m_editor_nodes) {
			ImGuiNodeEditor::BeginNode(node.id);
			ImGui::PushID(node.id.AsPointer());
			// Node name
			ImGui::TextUnformatted(node.name.c_str());
			// Draw inputs
			for (auto& input : node.inputs)
			{
				ImGuiNodeEditor::BeginPin(input.id, ImGuiNodeEditor::PinKind::Input);
				// Input name
				if (!input.name.empty())
					ImGui::Text("<- %s", input.name.c_str());
				ImGuiNodeEditor::EndPin();
			}
			// Draw outputs
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(10, 0));
			ImGui::SameLine();
			for (auto& input : node.outputs)
			{
				ImGuiNodeEditor::BeginPin(input.id, ImGuiNodeEditor::PinKind::Output);
				// Input name
				if (!input.name.empty())
					ImGui::Text("%s ->", input.name.c_str());
				ImGuiNodeEditor::EndPin();
			}
			ImGui::PopID();
			ImGuiNodeEditor::EndNode();
		}

		// Links
		for (auto& link : m_editor_links) {
			ImGuiNodeEditor::Link(link.id, link.start_id, link.end_id, ImVec4(0.85, 0.85, 0.85, 1.0), 2.0f);
		}

		// Link create
		if (!m_create_new_node)
		{
			if (ImGuiNodeEditor::BeginCreate(ImColor(255, 255, 255), 2.0f))
			{
				auto showLabel = [](const char* label, ImColor color)
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
						auto size = ImGui::CalcTextSize(label);

						auto padding = ImGui::GetStyle().FramePadding;
						auto spacing = ImGui::GetStyle().ItemSpacing;

						ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

						auto rectMin = ImGui::GetCursorScreenPos() - padding;
						auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

						auto drawList = ImGui::GetWindowDrawList();
						drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
						ImGui::TextUnformatted(label);
					};

				ImGuiNodeEditor::PinId start_pin_id = 0, end_pin_id = 0;
				if (ImGuiNodeEditor::QueryNewLink(&start_pin_id, &end_pin_id))
				{
					auto start_pin = get_pin_by_id(start_pin_id);
					auto end_pin = get_pin_by_id(end_pin_id);

					m_new_link_pin = start_pin ? start_pin : end_pin;

					if (start_pin->kind == editor_grpah_node_pin_kind::input)
					{
						std::swap(start_pin, end_pin);
						std::swap(start_pin_id, end_pin_id);
					}

					if (start_pin && end_pin)
					{
						if (end_pin == start_pin)
						{
							ImGuiNodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
						}
						else if (end_pin->kind == start_pin->kind)
						{
							showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
							ImGuiNodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
						}
						else if (end_pin->node == start_pin->node)
						{
						    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
							ImGuiNodeEditor::RejectNewItem(ImColor(255, 0, 0), 1.0f);
						}
						else if (end_pin->type != start_pin->type)
						{
							showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
							ImGuiNodeEditor::RejectNewItem(ImColor(255, 128, 128), 1.0f);
						}
						else
						{
							showLabel("+ Create Link", ImColor(32, 45, 32, 180));
							if (ImGuiNodeEditor::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
							{
								editor_graph_node_link created_link = editor_graph_node_link(m_editor_next_link_id++, start_pin_id, end_pin_id);
								m_editor_links.emplace_back(created_link);
							}
						}
					}
				}

				ImGuiNodeEditor::PinId pin_id = 0;
				if (ImGuiNodeEditor::QueryNewNode(&pin_id))
				{
					m_new_link_pin = get_pin_by_id(pin_id);
					if (m_new_link_pin)
						showLabel("+ Create Node", ImColor(32, 45, 32, 180));

					if (ImGuiNodeEditor::AcceptNewItem())
					{
						m_create_new_node = true;
						m_new_node_link_pin = get_pin_by_id(pin_id);
						m_new_link_pin = nullptr;
						ImGuiNodeEditor::Suspend();
						ImGui::OpenPopup("Create New Node");
						ImGuiNodeEditor::Resume();
					}
				}
			}
			else {
				m_new_link_pin = nullptr;
			}

			ImGuiNodeEditor::EndCreate();

			// Link delete
			if (ImGuiNodeEditor::BeginDelete())
			{
				ImGuiNodeEditor::LinkId link_id = 0;
				while (ImGuiNodeEditor::QueryDeletedLink(&link_id))
				{
					if (ImGuiNodeEditor::AcceptDeletedItem())
					{
						auto id = std::find_if(m_editor_links.begin(), m_editor_links.end(), [link_id](auto& link) { return link.id == link_id; });
						if (id != m_editor_links.end())
							m_editor_links.erase(id);
					}
				}

				ImGuiNodeEditor::NodeId node_id = 0;
				while (ImGuiNodeEditor::QueryDeletedNode(&node_id))
				{
					if (ImGuiNodeEditor::AcceptDeletedItem())
					{
						auto id = std::find_if(m_editor_nodes.begin(), m_editor_nodes.end(), [node_id](auto& node) { return node.id == node_id; });
						if (id != m_editor_nodes.end())
							m_editor_nodes.erase(id);
					}
				}
			}
			ImGuiNodeEditor::EndDelete();
		}
	}
}