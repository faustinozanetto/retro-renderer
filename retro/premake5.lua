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
    }

    links
	{
        "glfw",
        "glad"
    }

    filter "configurations:debug"
        defines "RT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:release"
        defines "RT_RELEASE"
        runtime "Release"
        optimize "on"
