#pragma once

#include "Mesh.hpp"
#include "fwd.hpp"
#include <assimp/scene.h>

#include <unordered_map>
#include <memory>

class Model {
public:
  Model();
  ~Model();

  void Load(std::string filepath);
  void Draw(GLuint program);

private:
  std::vector<std::unique_ptr<Mesh>> m_meshes;

  std::unordered_map<std::string, GLuint> m_texturemap;

  std::unique_ptr<Mesh> BuildMesh(const aiScene *const, size_t,
                                  std::string const &);

  GLuint ReadDiffuseTexture(aiMaterial const * material,
                            std::string const &rootpath);
};
