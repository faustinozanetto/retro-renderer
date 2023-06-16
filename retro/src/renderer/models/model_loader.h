#pragma once

#include "renderer/models/model.h"

#include <assimp/scene.h>

namespace retro::renderer
{
    class model_loader
    {
    public:
        static std::shared_ptr<model> load_model_from_file(const std::string& file_path);
        static std::shared_ptr<model> load_model_from_meshes(const std::vector<std::shared_ptr<mesh>>& meshes);

    private:
        static void parse_assimp_node(aiNode* assimp_node, const aiScene* assimp_scene,
                                      std::vector<std::shared_ptr<mesh>>& model_meshes);
        static std::shared_ptr<mesh> parse_assimp_mesh(aiMesh* assimp_mesh, const aiScene* assimp_scene);
    };
}
