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
group ""

group "core"
    include "retro"
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
group ""