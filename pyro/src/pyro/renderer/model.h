#pragma once

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace pyro
{
    class mesh;
    class texture_2d;

    /// \brief
    class model
    {
    public:
        model(const std::string& path);
        ~model();

        const std::vector<ref<mesh>>& meshes() const { return m_meshes; }

    private: // methods
        void process_node(aiNode* node, const aiScene* scene);
        ref<pyro::mesh> process_mesh(aiMesh* mesh, const aiScene* scene) const;
        std::vector<ref<texture_2d>> load_textures(aiMaterial* mat, aiTextureType type, const std::string& type_name) const;

    private: // fields
        std::vector<ref<mesh>> m_meshes;
        std::string m_directory;
        std::string m_path;
    };
}
