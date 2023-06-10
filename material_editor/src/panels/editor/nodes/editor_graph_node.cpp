#include "editor_graph_node.h"

namespace retro::material_editor {
	editor_graph_node::editor_graph_node()
	{
	}

	editor_graph_node::~editor_graph_node()
	{
	}

	void editor_graph_node::on_value_updated()
	{
		if (m_value_updated_callback) {
			m_value_updated_callback(this);
		}
	}
}