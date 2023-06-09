#include "rtpch.h"
#include "model_loader.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "utils/utils.h"
#include <glm/gtc/type_ptr.hpp>

namespace retro::renderer
{
    std::shared_ptr<model> model_loader::load_model_from_file(const std::string &file_path)
    {
        RT_PROFILE;
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading model from file.");
        RT_TRACE("  - File Path: '{0}'", file_path);

        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
                                                              aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            RT_ASSERT_MSG(false, "An error occurred while loading model from file.");
        }

        // Recursively parse all the assimp nodes and store the parsed mesh into the meshes vector.
        std::vector<std::shared_ptr<mesh>> model_meshes;
        parse_assimp_node(scene->mRootNode, scene, model_meshes);
        auto created_model = std::make_shared<model>(file_path, model_meshes);

        // Create bounding box
		glm::vec3 min(FLT_MAX);
		glm::vec3 max(-FLT_MAX);
		calculate_bounding_box(scene, scene->mRootNode, min, max);

        math::bounding_box bounding_box = math::bounding_box(min, max);
        created_model->set_bounding_box(bounding_box);

        RT_TRACE("Retro Renderer | Model loaded from file successfully!");
        RT_SEPARATOR();
        return created_model;
    }

    std::shared_ptr<model> model_loader::load_model_from_meshes(const std::vector<std::shared_ptr<mesh>> &meshes)
    {
        RT_PROFILE;
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading model from meshes.");
        auto created_model = std::make_shared<model>("from_meshes", meshes);
        RT_TRACE("Retro Renderer | Model loaded from meshes successfully!");
        RT_SEPARATOR();
        return created_model;
    }

	void model_loader::parse_assimp_node(aiNode* assimp_node, const aiScene* assimp_scene, std::vector<std::shared_ptr<mesh>>& model_meshes)
	{
		RT_PROFILE;
		// process all the node's meshes (if any)
		for (int i = 0; i < assimp_node->mNumMeshes; i++)
		{
			aiMesh* mesh = assimp_scene->mMeshes[assimp_node->mMeshes[i]];
			model_meshes.push_back(parse_assimp_mesh(mesh, assimp_scene));
		}
		// then do the same for each of its children
		for (int i = 0; i < assimp_node->mNumChildren; i++)
		{
			parse_assimp_node(assimp_node->mChildren[i], assimp_scene, model_meshes);
		}
	}

    std::shared_ptr<mesh> model_loader::parse_assimp_mesh(aiMesh *assimp_mesh, const aiScene *assimp_scene)
    {
        RT_PROFILE;
        RT_TRACE("  - Mesh: '{0}'", assimp_mesh->mName.C_Str());
        RT_TRACE("      - Vertex Count: {0}", assimp_mesh->mNumVertices);
        // 1. Initialize vectors for storing the data.
        std::vector<mesh_vertex> vertices;
        std::vector<mesh_index> indices;

        // 2. Process vertices
        for (int i = 0; i < assimp_mesh->mNumVertices; i++)
        {
            mesh_vertex vertex;
            // Position.
            glm::vec3 position;
            position.x = assimp_mesh->mVertices[i].x;
            position.y = assimp_mesh->mVertices[i].y;
            position.z = assimp_mesh->mVertices[i].z;
            // Normals
            glm::vec3 normals(0.0f);
            normals.x = assimp_mesh->mNormals[i].x;
            normals.y = assimp_mesh->mNormals[i].y;
            normals.z = assimp_mesh->mNormals[i].z;
            // Tex coords
            glm::vec2 tex_coords(0.0f);

            if (assimp_mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
            {
                // tex coords
                tex_coords = glm::vec2(assimp_mesh->mTextureCoords[0][i].x, assimp_mesh->mTextureCoords[0][i].y);
            }
            glm::vec3 tangent(0.0f);
            glm::vec3 bi_tangent(0.0f);
            // tangent
            if (assimp_mesh->mTangents)
            {
                tangent.x = assimp_mesh->mTangents[i].x;
                tangent.y = assimp_mesh->mTangents[i].y;
                tangent.z = assimp_mesh->mTangents[i].z;
            }
            // bitangent
            if (assimp_mesh->mBitangents)
            {
                bi_tangent.x = assimp_mesh->mBitangents[i].x;
                bi_tangent.y = assimp_mesh->mBitangents[i].y;
                bi_tangent.z = assimp_mesh->mBitangents[i].z;
            }
            vertex.position = position;
            vertex.tex_coords = tex_coords;
            vertex.normals = normals;
            vertex.tangent = tangent;
            vertex.bitangent = bi_tangent;
            vertices.push_back(vertex);
        }

        // 3. Process indices
        for (int i = 0; i < assimp_mesh->mNumFaces; i++)
        {
            aiFace face = assimp_mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector.
            for (int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // 4. Create the mesh
        return std::make_shared<mesh>(vertices, indices);
    }

    void model_loader::calculate_bounding_box(const aiScene* scene, const aiNode* node, glm::vec3& min, glm::vec3& max)
    {
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
			{
				const aiVector3D& vertex = mesh->mVertices[j];
				glm::vec3 position = glm::make_vec3(&vertex.x);

				min = glm::min(min, position);
				max = glm::max(max, position);
			}
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			calculate_bounding_box(scene, node->mChildren[i], min, max);
		}
    }
}
