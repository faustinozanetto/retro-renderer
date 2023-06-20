include "dependencies.lua"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "retro"
	architecture "x86_64"

	configurations
	{
		"debug",
		"release",
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Dependencies"
	include "third_party/glfw"
	include "third_party/glad"
	include "third_party/imgui"
	include "third_party/imgui_node_editor"
	include "third_party/imguizmo"
group ""

group "core"
    include "retro"
	include "editor"
	include "material_editor"
group ""

group "utils"
	include "utils/project_creator"
group ""

group "examples"
	include "examples/simple_triangle"
	include "examples/simple_triangle_textured"
	include "examples/simple_3d"
	include "examples/simple_instancing"
	include "examples/simple_model_loading"
	include "examples/simple_geometry_shader"
	include "examples/simple_lighting"
	include "examples/point_shadows"
	include "examples/normal_mapping"
	include "examples/parallax_mapping"
	include "examples/ssao"
	include "examples/pbr"
	include "examples/pbr-ibl"
	include "examples/bloom"
	include "examples/simple_fonts"
	include "examples/space_invaders"
	include "examples/physx"
group ""