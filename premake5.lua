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
group ""

group "core"
    include "retro"
group ""

group "examples"
    include "examples/demo"
	include "examples/simple_triangle"
	include "examples/simple_triangle_textured"
	include "examples/simple_3d"
	include "examples/simple_instancing"
group ""