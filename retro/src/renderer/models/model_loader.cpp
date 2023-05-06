#include "rtpch.h"
#include "model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace retro::renderer
{
    std::shared_ptr<model> model_loader::load_model_from_file(const std::string &file_path)
    {
        RT_TRACE("Retro Renderer | Started loading model from file.");
        RT_TRACE("  - File Path: '{0}'", file_path);

        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            RT_ASSERT_MSG(false, "An error ocurred while loading model from file.");
        }

        RT_TRACE("Retro Renderer | Model loaded successfully.");
        return std::shared_ptr<model>();
    }
}