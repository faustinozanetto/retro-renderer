project "project-creator"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir("%{wks.location}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{wks.location}/intermediates/" .. output_dir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
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