#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <assert.h>
#include <map>
#include <regex>

#define PROJECT_CONFIG_FILE ""

#define PROJECT_NAME_PARAM "-name"
#define PROJECT_LOCATION_PARAM "-location"
const std::filesystem::path DEBUG_BINARIES_FOLDER = "binaries/debug-windows-x86_64";
const std::filesystem::path RELEASE_BINARIES_FOLDER = "binaries/release-windows-x86_64";

const std::filesystem::path ASSIMP_DEBUG_LIB = "assimp-vc143-mtd.dll";
const std::filesystem::path ASSIMP_RELEASE_LIB = "assimp-vc143-mt.dll";

const std::filesystem::path ASSIMP_DEBUG_BINARIES = "third_party/assimp/build/bin/Debug" / ASSIMP_DEBUG_LIB;
const std::filesystem::path ASSIMP_RELEASE_BINARIES = "third_party/assimp/build/bin/Release" / ASSIMP_RELEASE_LIB;

const std::string PREMAKE_FILE = R"(project "$ARGS[0]"
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
)";

struct arguments_data
{
    std::string name;
    std::string location;
};

arguments_data parse_program_arguments(int argc, char *argv[])
{
    std::string name;
    std::string location;
    for (int i = 1; i < argc; i += 2)
    {
        std::string arg(argv[i]);
        if (arg == PROJECT_NAME_PARAM)
        {
            name = argv[i + 1];
        }
        else if (arg == PROJECT_LOCATION_PARAM)
        {
            location = argv[i + 1];
        }
        else
        {
            std::cout << "Unknown argument: " << arg << std::endl;
        }
    }
    return {name, location};
}

void show_help()
{
    printf("======================================================\n");
    printf("Project Creator Utility Program\n");
    printf("How to use the utility program to create projects\n");
    printf("for Retro Renderer. In order to properly use it\n");
    printf("you need to specify the following params: \n");
    printf("    - Name of the project: %s test_project\n", PROJECT_NAME_PARAM);
    printf("    - Location of the project: %s example_projects\n", PROJECT_LOCATION_PARAM);
    printf("======================================================\n");
}

bool create_directory(const std::string &directory_path)
{
    bool result = std::filesystem::create_directory(directory_path);
    assert(result, "Failed to create directory!");

    printf("Directory '%s' created successfully!.", directory_path.c_str());
    return true;
}

bool try_copy_file(const std::filesystem::path &source_path, const std::filesystem::path &target_path)
{
    bool result = std::filesystem::copy_file(source_path, target_path, std::filesystem::copy_options::overwrite_existing);
    assert(result, "Failed to copy file!");

    printf("File copied to '%s' successfully!.", target_path.c_str());
    return true;
}

void create_project_folders(arguments_data &data)
{
    // 1. Create project source folder
    std::filesystem::path project_path = std::filesystem::current_path() / data.location / data.name;
    create_directory(project_path);
    // 2. Create binaries folders
    std::filesystem::path project_debug_binaries_path = std::filesystem::current_path() / DEBUG_BINARIES_FOLDER / data.name;
    create_directory(project_debug_binaries_path);
    std::filesystem::path project_release_binaries_path = std::filesystem::current_path() / RELEASE_BINARIES_FOLDER / data.name;
    create_directory(project_release_binaries_path);
}

void copy_and_modify_premake_file(arguments_data &data)
{
    std::string premake_contents = PREMAKE_FILE;

    // Find all occurrences of "$ARGS[n]" and extract the integer n
    std::regex args_regex("\\$ARGS\\[(\\d+)\\]");
    std::smatch args_match;
    std::vector<int> args_indices;
    std::string::const_iterator searchStart(premake_contents.cbegin());
    while (std::regex_search(searchStart, premake_contents.cend(), args_match, args_regex))
    {
        args_indices.push_back(std::stoi(args_match[1]));
        searchStart = args_match.suffix().first;
    }

    // Create a map from argument index to argument value
    std::map<int, std::string> args_values;
    args_values[0] = data.name;

    // Replace each "$ARGS[n]" with the corresponding argument value
    for (int index : args_indices)
    {
        auto it = args_values.find(index);
        if (it != args_values.end())
        {
            std::string arg_value = it->second;
            size_t arg_pos = premake_contents.find("$ARGS[" + std::to_string(index) + "]");
            if (arg_pos != std::string::npos)
            {
                premake_contents.replace(arg_pos, std::string("$ARGS[" + std::to_string(index) + "]").length(), arg_value);
            }
        }
    }

    // Save the modified premake file to the project directory
    std::filesystem::path project_path = std::filesystem::current_path() / data.location / data.name;
    std::filesystem::path premake_file_path = project_path / "premake5.lua";
    std::ofstream premake_file(premake_file_path);
    if (!premake_file)
    {
        printf("Failed to create premake file at path: '%s'\n", premake_file_path.c_str());
    }
    premake_file << premake_contents;
    premake_file.close();
}

void copy_library_binaries(arguments_data &data)
{
    // 1. Copy assimp binaries
    std::filesystem::path assimp_debug_source = std::filesystem::current_path() / ASSIMP_DEBUG_BINARIES;
    std::filesystem::path project_debug_binaries_path = std::filesystem::current_path() / DEBUG_BINARIES_FOLDER / data.name / ASSIMP_DEBUG_LIB;
    try_copy_file(assimp_debug_source, project_debug_binaries_path);
    std::filesystem::path assimp_release_source = std::filesystem::current_path() / ASSIMP_RELEASE_BINARIES;
    std::filesystem::path project_release_binaries_path = std::filesystem::current_path() / RELEASE_BINARIES_FOLDER / data.name / ASSIMP_RELEASE_LIB;
    try_copy_file(assimp_release_source, project_release_binaries_path);
}

int main(int argc, char *argv[])
{
    if (argc > 0 && argc < 5)
    {
        show_help();
        return 1;
    }

    std::filesystem::current_path("../../");

    arguments_data arguments = parse_program_arguments(argc, argv);
    create_project_folders(arguments);
    copy_and_modify_premake_file(arguments);
    copy_library_binaries(arguments);
    return 0;
}