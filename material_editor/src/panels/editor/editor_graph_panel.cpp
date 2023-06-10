#include "editor_graph_panel.h"

#include <retro.h>
#include <glm/gtc/type_ptr.hpp>

#include "nodes/editor_graph_node_material.h"
#include "nodes/editor_graph_node_float.h"
#include "nodes/editor_graph_node_color3.h"
#include "nodes/editor_graph_node_texture.h"
#include "../../material_editor_app.h"

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

		const auto& app = dynamic_cast<material_editor_app*>(&core::application::get());
		auto& current_material = app->get_current_material();

		std::shared_ptr<editor_graph_node_material> material_node = std::make_shared<editor_graph_node_material>(); // material output
		m_editor_nodes.push_back(material_node);

		std::shared_ptr<editor_graph_node_color3> albedo_color_node = std::make_shared<editor_graph_node_color3>(); // albedo
		albedo_color_node->set_value_updated_callback([albedo_color_node](editor_graph_node* graph_node) {
			const auto& app = dynamic_cast<material_editor_app*>(&core::application::get());
			editor_graph_node_color3* color3_graph_node = dynamic_cast<editor_graph_node_color3*>(graph_node);
			app->get_current_material()->set_albedo(color3_graph_node->get_value());
			});
		m_editor_nodes.push_back(albedo_color_node);

		std::shared_ptr<editor_graph_node_color3> emissive_color_node = std::make_shared<editor_graph_node_color3>(); // emissive
		m_editor_nodes.push_back(emissive_color_node);

		std::shared_ptr<editor_graph_node_float> roughness_float_node = std::make_shared<editor_graph_node_float>(); // roughness
		m_editor_nodes.push_back(roughness_float_node);

		std::shared_ptr<editor_graph_node_float> metallic_float_node = std::make_shared<editor_graph_node_float>(); // metallic
		m_editor_nodes.push_back(metallic_float_node);

		std::shared_ptr<editor_graph_node_float> ambient_occlusion_float_node =std::make_shared<editor_graph_node_float>(); // ambient occlusion
		m_editor_nodes.push_back(ambient_occlusion_float_node);

		std::shared_ptr<editor_graph_node_float> emissive_strenght_float_node =std::make_shared<editor_graph_node_float>(); // emissive strength
		m_editor_nodes.push_back(emissive_strenght_float_node);

		std::shared_ptr<editor_graph_node_float> texture_tilling_float_node =std::make_shared<editor_graph_node_float>(); // texture tilling
		m_editor_nodes.push_back(texture_tilling_float_node);

		std::shared_ptr<editor_graph_node_texture> albedo_texture_node = std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::albedo).texture); // albedo
		albedo_texture_node->set_value_updated_callback([albedo_texture_node](editor_graph_node* graph_node) {
			const auto& app = dynamic_cast<material_editor_app*>(&core::application::get());
			editor_graph_node_texture* texture_graph_node = dynamic_cast<editor_graph_node_texture*>(graph_node);
			renderer::material_texture albedo_texture;
			albedo_texture.is_enabled = true;
			albedo_texture.type = renderer::material_texture_type::albedo;
			albedo_texture.texture = texture_graph_node->get_value();
			app->get_current_material()->set_texture(albedo_texture);
			});
		m_editor_nodes.push_back(albedo_texture_node);

		std::shared_ptr<editor_graph_node_texture> normal_texture_node =std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::normal).texture); // normal
		m_editor_nodes.push_back(normal_texture_node);

		std::shared_ptr<editor_graph_node_texture> roughness_texture_node =std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::roughness).texture); // roughness
		m_editor_nodes.push_back(roughness_texture_node);

		std::shared_ptr<editor_graph_node_texture> metallic_texture_node =std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::metallic).texture); // metallic
		m_editor_nodes.push_back(metallic_texture_node);

		std::shared_ptr<editor_graph_node_texture> ambient_occlusion_texture_node =std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::ambient_occlusion).texture); // ambient occlusion
		m_editor_nodes.push_back(ambient_occlusion_texture_node);

		std::shared_ptr<editor_graph_node_texture> emissive_texture_node =std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::emissive).texture); // emissive
		m_editor_nodes.push_back(emissive_texture_node);

		std::shared_ptr<editor_graph_node_texture> opacity_texture_node =std::make_shared<editor_graph_node_texture>(current_material->get_data().textures.at(renderer::material_texture_type::opacity).texture); // opacity
		m_editor_nodes.push_back(opacity_texture_node);

		graph_node_link albedo_color_node_link = graph_node_link(get_next_id(), albedo_color_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[0].id);
		m_editor_links.push_back(albedo_color_node_link);

		graph_node_link emissive_color_node_link = graph_node_link(get_next_id(), emissive_color_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[1].id);
		m_editor_links.push_back(emissive_color_node_link);

		graph_node_link roughness_float_node_link = graph_node_link(get_next_id(), roughness_float_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[2].id);
		m_editor_links.push_back(roughness_float_node_link);

		graph_node_link metallic_float_node_link = graph_node_link(get_next_id(), metallic_float_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[3].id);
		m_editor_links.push_back(metallic_float_node_link);

		graph_node_link ambient_occlusion_node_link = graph_node_link(get_next_id(), ambient_occlusion_float_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[4].id);
		m_editor_links.push_back(ambient_occlusion_node_link);

		graph_node_link emissive_strenght_float_node_link = graph_node_link(get_next_id(), emissive_strenght_float_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[5].id);
		m_editor_links.push_back(emissive_strenght_float_node_link);

		graph_node_link texture_tilling_float_node_link = graph_node_link(get_next_id(), texture_tilling_float_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[6].id);
		m_editor_links.push_back(texture_tilling_float_node_link);

		// Textures links
		graph_node_link albedo_texture_node_link = graph_node_link(get_next_id(), albedo_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[7].id);
		m_editor_links.push_back(albedo_texture_node_link);

		graph_node_link normal_texture_node_link = graph_node_link(get_next_id(), normal_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[8].id);
		m_editor_links.push_back(normal_texture_node_link);

		graph_node_link roughness_texture_node_link = graph_node_link(get_next_id(), roughness_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[9].id);
		m_editor_links.push_back(roughness_texture_node_link);

		graph_node_link metallic_texture_node_link = graph_node_link(get_next_id(), metallic_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[10].id);
		m_editor_links.push_back(metallic_texture_node_link);

		graph_node_link ambient_occlusion_texture_node_link = graph_node_link(get_next_id(), ambient_occlusion_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[11].id);
		m_editor_links.push_back(ambient_occlusion_texture_node_link);

		graph_node_link emissive_texture_node_link = graph_node_link(get_next_id(), emissive_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[12].id);
		m_editor_links.push_back(emissive_texture_node_link);

		graph_node_link opacity_texture_node_link = graph_node_link(get_next_id(), opacity_texture_node->get_graph_node().outputs[0].id, material_node->get_graph_node().inputs[13].id);
		m_editor_links.push_back(opacity_texture_node_link);
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