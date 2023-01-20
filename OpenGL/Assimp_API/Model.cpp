#include "Model.hpp"

#include "readFiles.hpp"
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <optional>

Model::Model(){};

Model::~Model(){};

GLuint Model::ReadDiffuseTexture(aiMaterial const * material,
                                 std::string const &pathroot) {
  aiString aistr;
  material->GetTexture(aiTextureType_DIFFUSE, 0, &aistr);
  std::string texpath = aistr.C_Str();
  texpath = pathroot + '/' + texpath;

  if (m_texturemap.find(texpath) == m_texturemap.end()) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    int texturewidth, textureheight;

    unsigned char *image = SOIL_load_image(texpath.c_str(), &texturewidth,
                                           &textureheight, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturewidth, textureheight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image);
    delete[] image;

    m_texturemap.insert({texpath, tex});
  }

  return m_texturemap[texpath];
}

std::unique_ptr<Mesh> Model::BuildMesh(const aiScene *const scene,
                                       size_t meshID,
                                       std::string const &rootpath) {
  const auto aimesh = scene->mMeshes[meshID];
  std::vector<glm::vec3> vertexposition;
  std::vector<glm::vec3> vertexnormals;
  std::vector<glm::vec2> vertextexcoord;
  for (size_t i = 0; i < aimesh->mNumVertices; ++i) {
    vertexposition.emplace_back(aimesh->mVertices[i].x, aimesh->mVertices[i].y,
                                aimesh->mVertices[i].z);
    vertexnormals.emplace_back(aimesh->mNormals[i].x, aimesh->mNormals[i].y,
                               aimesh->mNormals[i].z);

    aimesh->mTextureCoords[0]
        ? vertextexcoord.emplace_back(aimesh->mTextureCoords[0][i].x,
                                      aimesh->mTextureCoords[0][i].y)
        : vertextexcoord.emplace_back(0.0f, 0.0f);
  }

  std::optional<GLuint> diffusetexture;
  if (aimesh->mMaterialIndex >= 0) {
    aiMaterial * material = scene->mMaterials[aimesh->mMaterialIndex];

    aiString aiStr;
    // get diffuse path, only need 1
    material->GetTexture(aiTextureType_DIFFUSE, 0, &aiStr);

    diffusetexture = ReadDiffuseTexture(material, rootpath);
  }

  std::vector<int> indices;
  for (GLuint j = 0; j < aimesh->mNumFaces; ++j) {
    auto const &face = aimesh->mFaces[j];
    for (GLuint k = 0; k < face.mNumIndices; ++k) {
      indices.emplace_back(face.mIndices[k]);
    }
  }

  return std::make_unique<Mesh>(
      std::move(vertexposition), std::move(vertextexcoord),
      std::move(vertexnormals), std::move(indices), *diffusetexture);
};

void Model::Load(std::string filepath) {
  std::unique_ptr<Assimp::Importer> importer =
      std::make_unique<Assimp::Importer>();

  const aiScene * scene =
      importer->ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw FileException{filepath, FileErrorID::AssimpReadFileError};
  }

  std::string rootpath = filepath.substr(0, filepath.find_last_of('/'));

  for (size_t meshID = 0; meshID < scene->mNumMeshes; ++meshID) 
  {
    m_meshes.emplace_back(std::move(BuildMesh(scene, meshID, rootpath)));
    m_meshes.back()->BindData();
  }
}

void Model::Draw(GLuint program)
{ 
  for (auto &&mesh :m_meshes) 
  {
    mesh->Draw(program);
  }
}
