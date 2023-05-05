project "glad"
	kind "StaticLib"
	language "C"
	
	targetdir ("%{wks.location}/binaries/" .. output_dir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. output_dir .. "/%{prj.name}")

	files {
		"include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
	}
	
    includedirs {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"
		staticruntime "off"

    filter "configurations:debug"
        runtime "debug"
        symbols "on"

    filter "configurations:release"
        runtime "release"
        optimize "on"