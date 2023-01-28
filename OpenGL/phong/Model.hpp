#pragma once

#include "IMVP.hpp"
#include "Mesh.hpp"
#include <assimp/scene.h>

#include <memory>
#include <unordered_map>

class Model
{
    glm::mat4 const unit {glm::vec4{1, 0, 0, 0}, glm::vec4{0, 1, 0, 0},
                         glm::vec4{0, 0, 1, 0}, glm::vec4{0, 0, 0, 1}};

public:
    Model(glm::vec3&& scale, glm::vec3&& translation, glm::vec3&& rotation,
          std::string&& filepath);
    ~Model();

    void MoveModel(glm::vec3&& translation);

    glm::vec3 const& GetPosition();

    void Draw(GLuint program);


private:
    glm::vec3 m_scale;
    glm::vec3 m_translation;
    glm::vec3 m_rotation;
    glm::mat4 m_modelmatrix;
    glm::mat4 const CalModelMatrix();
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::unordered_map<std::string, GLuint> m_texturemap;
    std::unique_ptr<Mesh> BuildMesh(const aiScene* const, size_t,
                                    std::string const&);
    GLuint ReadDiffuseTexture(aiMaterial const* material,
                              std::string const& rootpath);
    void Load(std::string&& filepath);
};

std::unique_ptr<Model> LoadAModel(glm::vec3&& scale, glm::vec3&& translation,
                                  glm::vec3&& rotation, std::string&& filepath);
