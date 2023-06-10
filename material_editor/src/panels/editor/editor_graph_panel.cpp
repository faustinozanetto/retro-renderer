#include "editor_graph_panel.h"

#include <retro.h>
#include <glm/gtc/type_ptr.hpp>

#include "nodes/editor_graph_node_material.h"
#include "nodes/editor_graph_node_float.h"
#include "nodes/editor_graph_node_color3.h"

namespace retro::material_editor
{
	editor_graph_panel::editor_graph_panel()
	{
		initialize_editor_graph();
	}

	editor_graph_panel::~editor_graph_panel()
	{
		ImGuiNodeEditor::DestroyEditor(m_editor_context);
	}

	void editor_graph_panel::initialize_editor_graph()
	{
		ImGuiNodeEditor::Config config;
		m_editor_context = ImGuiNodeEditor::CreateEditor(&config);

		m_create_new_node = false;
		m_new_link_pin = nullptr;
		m_new_node_link_pin = nullptr;

		m_editor_nodes.push_back(std::make_shared<editor_graph_node_material>());
		m_editor_nodes.push_back(std::make_shared<editor_graph_node_float>());
		m_editor_nodes.push_back(std::make_shared<editor_graph_node_float>());
		m_editor_nodes.push_back(std::make_shared<editor_graph_node_color3>());
	}

	graph_node_pin *editor_graph_panel::get_pin_by_id(ImGuiNodeEditor::PinId id)
	{
		if (!id)
			return nullptr;

		for (auto &node : m_editor_nodes)
		{
			auto &graph_node = node->get_graph_node();
			for (auto &pin : graph_node.inputs)
				if (pin.id == id)
					return &pin;

			for (auto &pin : graph_node.outputs)
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
		for (auto &node : m_editor_nodes)
		{
			auto &graph_node = node->get_graph_node();

			ImGuiNodeEditor::BeginNode(graph_node.id);
			ImGui::PushID(graph_node.id.AsPointer());
			// Node name
			ImGui::SetWindowFontScale(1.5f);
			ImGui::Text(graph_node.name.c_str());
			ImGui::SetWindowFontScale(1.0f);

			ImGui::Spacing();

			// Draw inputs
			for (auto &input : graph_node.inputs)
			{
				ImGuiNodeEditor::BeginPin(input.id, ImGuiNodeEditor::PinKind::Input);
				// Input name
				if (!input.name.empty())
					ImGui::Text("<- %s", input.name.c_str());
				ImGuiNodeEditor::EndPin();
			}

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(250, 0));
			ImGui::SameLine();

			// Draw outputs
			for (auto &input : graph_node.outputs)
			{
				ImGuiNodeEditor::BeginPin(input.id, ImGuiNodeEditor::PinKind::Output);
				// Input name
				if (!input.name.empty())
					ImGui::Text("%s ->", input.name.c_str());
				ImGuiNodeEditor::EndPin();
			}

			// Draw content from the node
			ImGui::PushItemWidth(250);
			node->on_draw_node();
			ImGui::PopItemWidth();

			ImGui::PopID();
			ImGuiNodeEditor::EndNode();
		}

		// Links
		for (auto &link : m_editor_links)
		{
			ImGuiNodeEditor::Link(link.id, link.start_id, link.end_id, ImVec4(0.85, 0.85, 0.85, 1.0), 2.0f);
		}

		// Link create
		if (!m_create_new_node)
		{
			if (ImGuiNodeEditor::BeginCreate(ImColor(255, 255, 255), 2.0f))
			{
				auto showLabel = [](const char *label, ImColor color)
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

					if (start_pin->kind == graph_node_pin_kind::input)
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
								graph_node_link created_link = graph_node_link(m_editor_next_link_id++, start_pin_id, end_pin_id);
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
			else
			{
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
						auto id = std::find_if(m_editor_links.begin(), m_editor_links.end(), [link_id](auto &link)
											   { return link.id == link_id; });
						if (id != m_editor_links.end())
							m_editor_links.erase(id);
					}
				}

				ImGuiNodeEditor::NodeId node_id = 0;
				while (ImGuiNodeEditor::QueryDeletedNode(&node_id))
				{
					if (ImGuiNodeEditor::AcceptDeletedItem())
					{
						auto id = std::find_if(m_editor_nodes.begin(), m_editor_nodes.end(), [node_id](std::shared_ptr<editor_graph_node> &node)
											   { return node->get_graph_node().id == node_id; });
						if (id != m_editor_nodes.end())
							m_editor_nodes.erase(id);
					}
				}
			}
			ImGuiNodeEditor::EndDelete();
		}
	}
}