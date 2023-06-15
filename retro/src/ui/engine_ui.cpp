#include "rtpch.h"
#include "engine_ui.h"

#include "core/application.h"
#include "renderer/renderer/renderer.h"

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

namespace retro::ui
{
    void engine_ui::initialize()
    {
        RT_PROFILE;
        RT_TRACE("Retro Renderer | Interface initialization started.");
        initialize_imgui();
        RT_TRACE("Retro Renderer | Interface initializated successfully.");
    }

    void engine_ui::initialize_imgui()
    {
        RT_PROFILE;
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
        //  Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer bindings
        GLFWwindow *window = core::application::get().get_window()->get_handle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void engine_ui::begin_frame()
	{
		RT_PROFILE;
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void engine_ui::end_frame()
    {
        RT_PROFILE;
        // Rendering
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)retro::renderer::renderer::get_viewport_size().x, (float)retro::renderer::renderer::get_viewport_size().y);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}