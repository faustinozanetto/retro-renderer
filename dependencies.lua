include_dirs = {}

include_dirs["glfw"] = "%{wks.location}/third_party/glfw/include"
include_dirs["glad"] = "%{wks.location}/third_party/glad/include"
include_dirs["glm"] = "%{wks.location}/third_party/glm"
include_dirs["spdlog"] = "%{wks.location}/third_party/spdlog/include"
include_dirs["stb"] = "%{wks.location}/third_party/stb"
include_dirs["imgui"] = "%{wks.location}/third_party/imgui"
include_dirs["assimp"] = "%{wks.location}/third_party/assimp/include"

library_dirs = {}

library_dirs["assimp_debug"] = "%{wks.location}/third_party/assimp/build/lib/Debug/assimp-vc143-mtd.lib"
library_dirs["assimp_release"] = "%{wks.location}/third_party/assimp/build/lib/Release/assimp-vc143-mt.lib"