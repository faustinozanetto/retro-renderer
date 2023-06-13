project "retro"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

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
        "%{include_dirs.assimp}",
        "%{include_dirs.freetype}",
        "%{include_dirs.openal}",
        "%{include_dirs.dr_libs}",
        "%{include_dirs.entt}",
        "%{include_dirs.physx}",
        "%{include_dirs.physx_omnipvd}",
    }

    links
	{
        "glfw",
        "glad",
        "imgui",
    }

    filter "configurations:debug"
        defines "RT_DEBUG"
        runtime "Debug"
        symbols "on"

        links {
            "%{library_dirs.assimp_debug}",
            "%{library_dirs.freetype_debug}",
            "%{library_dirs.openal_debug}",
            "%{library_dirs.physx_debug}",
            "%{library_dirs.physx_common_debug}",
            "%{library_dirs.physx_foundation_debug}",
            "%{library_dirs.physx_extensions_debug}",
            "%{library_dirs.physx_pvd_runtime_debug}",
            "%{library_dirs.physx_pvd_sdk_debug}",
        }

    filter "configurations:release"
        defines "RT_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"

        links {
            "%{library_dirs.assimp_release}",
            "%{library_dirs.freetype_release}",
            "%{library_dirs.openal_release}",
            "%{library_dirs.physx_release}",
            "%{library_dirs.physx_common_release}",
            "%{library_dirs.physx_foundation_release}",
            "%{library_dirs.physx_extensions_release}",
            "%{library_dirs.physx_pvd_runtime_release}",
            "%{library_dirs.physx_pvd_sdk_release}",
        }