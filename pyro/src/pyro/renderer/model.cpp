﻿#include "pyro_pch.h"
#include "model.h"
#include "mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

pyro::model::model(const std::string& path)
    :m_path(path)
{
    PYRO_CORE_INFO("[model] Creating model '{0}'.", m_path);
    Assimp::Importer importer;
    // to calculate tangent and bitangent uncomment the flag on next line, extend vertex and follow this: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h 
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs /*| aiProcess_CalcTangentSpace*/);

    if((!scene) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode))
    {
        PYRO_CORE_ERROR("[model] Unable to load model '{}'.", m_path);
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/') + 1);

    process_node(scene->mRootNode, scene);
}

pyro::model::~model()
{
    PYRO_CORE_INFO("[model] Destroying model '{0}'.", m_path);
}

void pyro::model::process_node(aiNode* node, const aiScene* scene)
{
    for(uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(process_mesh(mesh, scene));
    }

    for(uint32_t i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

pyro::ref<pyro::mesh> pyro::model::process_mesh(aiMesh* mesh, const aiScene* scene) const
{
    std::vector<mesh::vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<ref<texture_2d>> textures;

    // == Process vertices
    for(uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        mesh::vertex vert{};

        // Position
        glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vert.position = pos;

        // Normal
        glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vert.normal = norm;

        // TexCoords
        if(mesh->mTextureCoords[0])
        {// Does it have any texture coordinates?
            glm::vec2 tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vert.tex_coords = tex;
        }
        else
        {
            vert.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        // Push into vertex array
        vertices.push_back(vert);
    }

    // == Process indices
    for(uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // == Process materials/textures
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<ref<texture_2d>> diffuseMaps = load_textures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<ref<texture_2d>> specularMaps = load_textures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return ref<pyro::mesh>(mesh::create(vertices, indices));
}

std::vector<pyro::ref<pyro::texture_2d>> pyro::model::load_textures(aiMaterial* mat, aiTextureType type, const std::string& type_name) const
{
    static std::vector<ref<texture_2d>> textures_loaded;
    std::vector<ref<texture_2d>> textures;
    for(uint32_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString filename;
        mat->GetTexture(type, i, &filename);

        //texture texture(texture::create(full_path));
        //texture.LoadTexture();
        //textures.push_back(texture);

        bool skip = false;
        for(uint32_t j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j]->path().data(), filename.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            const std::string full_path = m_directory + std::string(filename.C_Str());
            ref<texture_2d> texture2d = texture_2d::create(full_path);
            textures.push_back(texture2d);
            textures_loaded.push_back(texture2d); // add to loaded textures
        }
    }

    return textures;
}
