project "ssao"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir("%{wks.location}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{wks.location}/intermediates/" .. output_dir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
        "%{wks.location}/third_party/stb/**.h",
        "%{wks.location}/third_party/stb/**.cpp",
    }
    
    includedirs {
        "%{wks.location}/retro/src",
        "%{include_dirs.glfw}",
		"%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.stb}",
        "%{include_dirs.imgui}",
        "%{include_dirs.assimp}",
        "%{include_dirs.freetype}",
    }

    links {
        "retro"
    }

    filter "system:windows"
        staticruntime "off"
        systemversion "latest"

    filter "configurations:debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:release"
        runtime "Release"
        optimize "on"