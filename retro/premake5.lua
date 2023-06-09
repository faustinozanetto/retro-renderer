project "retro"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

    targetdir("%{wks.location}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{wks.location}/intermediates/" .. output_dir .. "/%{prj.name}")

	pchheader "rtpch.h"
	pchsource "src/rtpch.cpp"

    files {
        "src/**.h",
        "src/**.cpp",
        "%{wks.location}/third_party/stb/**.h",
        "%{wks.location}/third_party/stb/**.cpp",
    }

    includedirs
	{
		"src",
        "%{include_dirs.glfw}",
		"%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.stb}",
        "%{include_dirs.imgui}",
        "%{include_dirs.imgui_node_editor}",
        "%{include_dirs.assimp}",
        "%{include_dirs.freetype}",
        "%{include_dirs.openal}",
        "%{include_dirs.dr_libs}",
        "%{include_dirs.entt}",
    }

    links
	{
        "glfw",
        "glad",
        "imgui",
        "imgui_node_editor"
    }

    filter "configurations:debug"
        defines "RT_DEBUG"
        runtime "Debug"
        symbols "on"

        links {
            "%{library_dirs.assimp_debug}",
            "%{library_dirs.freetype_debug}",
            "%{library_dirs.openal_debug}",
        }

    filter "configurations:release"
        defines "RT_RELEASE"
        runtime "Release"
        optimize "on"

        links {
            "%{library_dirs.assimp_release}",
            "%{library_dirs.freetype_release}",
            "%{library_dirs.openal_release}",
        }