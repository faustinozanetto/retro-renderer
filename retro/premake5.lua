project "retro"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

    targetdir("%{wks.location}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{wks.location}/intermediates/" .. output_dir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
	{
		"src",
    }

    filter "configurations:debug"
        defines "RT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:release"
        defines "RT_RELEASE"
        runtime "Release"
        optimize "on"
