include_dirs = {}

include_dirs["glfw"] = "%{wks.location}/third_party/glfw/include"
include_dirs["glad"] = "%{wks.location}/third_party/glad/include"
include_dirs["glm"] = "%{wks.location}/third_party/glm"
include_dirs["spdlog"] = "%{wks.location}/third_party/spdlog/include"
include_dirs["stb"] = "%{wks.location}/third_party/stb"
include_dirs["imgui"] = "%{wks.location}/third_party/imgui"
include_dirs["imgui_node_editor"] = "%{wks.location}/third_party/imgui_node_editor"
include_dirs["assimp"] = "%{wks.location}/third_party/assimp/include"
include_dirs["freetype"] = "%{wks.location}/third_party/freetype/include"
include_dirs["openal"] = "%{wks.location}/third_party/openal/include"
include_dirs["dr_libs"] = "%{wks.location}/third_party/dr_libs"
include_dirs["entt"] = "%{wks.location}/third_party/entt/single_include"

library_dirs = {}

library_dirs["assimp_debug"] = "%{wks.location}/third_party/assimp/build/lib/Debug/assimp-vc143-mtd.lib"
library_dirs["assimp_release"] = "%{wks.location}/third_party/assimp/build/lib/Release/assimp-vc143-mt.lib"
library_dirs["freetype_debug"] = "%{wks.location}/third_party/freetype/lib/debug/freetype.lib"
library_dirs["freetype_release"] = "%{wks.location}/third_party/freetype/lib/release/freetype.lib"
library_dirs["openal_debug"] = "%{wks.location}/third_party/openal/build/Debug/OpenAL32.lib"
library_dirs["openal_release"] = "%{wks.location}/third_party/openal/build/Release/OpenAL32.lib"