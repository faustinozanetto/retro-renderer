#include "editor_profiler_panel.h"

#include "../editor_app.h"
#include "../utils/editor_ui_utils.h"

#include <core/application.h>

namespace retro::editor
{
	editor_profiler_panel::editor_profiler_panel()
	{
	}

	editor_profiler_panel::~editor_profiler_panel() {}

	void editor_profiler_panel::on_render_panel()
	{
		RT_PROFILE;
		auto frame_times = core::time::get_frame_times();
		// Calculate average frame time
		float average_frame_time = 0.0f;
		if (!frame_times.empty())
		{
			for (float frame_time : frame_times)
			{
				average_frame_time += frame_time;
			}
			average_frame_time /= frame_times.size();
		}

		ImGui::Begin("Profiler");
		ImGui::Text("FPS %.6f", core::time::get_fps());
		ImGui::PlotLines("Frame Times", frame_times.data(), static_cast<int>(core::time::get_frame_times().size()), 0, nullptr, 0.0f, 0.01f, ImVec2(0, 100));
		ImGui::Text("Average Frame Time: %.6f ms", average_frame_time);
		ImGui::End();
	}
}