#pragma once

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

enum class FileErrorID {
  ObjFileFailedToOpen,
  OFFFileFailedToOpen,
  ShaderFileFailedToOpen
};

class FileException {
public:
  FileException(std::string, FileErrorID);
  std::string const &GetFileName() const;
  std::filesystem::path const &GetCurrentFolder() const;
  FileErrorID const &GetErrorID() const;

private:
  std::string m_filename;
  std::filesystem::path m_currentfolder;
  FileErrorID m_errorID;
};

FileException::FileException(std::string filename, FileErrorID errorid)
    : m_filename{filename}, m_errorID{errorid},
      m_currentfolder{std::filesystem::current_path()} {};

std::filesystem::path const &FileException::GetCurrentFolder() const {
  return m_currentfolder;
}

std::string const &FileException::GetFileName() const { return m_filename; }

FileErrorID const &FileException::GetErrorID() const { return m_errorID; }

std::string readShaderFile(std::string const &filepath) {
  std::string res, line;
  std::ifstream fin(filepath);
  if (!fin.is_open()) {
    throw FileException{filepath, FileErrorID::ShaderFileFailedToOpen};
  }
  while (std::getline(fin, line)) {
    res += line + '\n';
  }
  fin.close();
  return res;
}

void readObj(std::string filepath, std::vector<glm::vec3> &points,
             std::vector<glm::vec2> &texcoords,
             std::vector<glm::vec3> &normals) {
  std::vector<glm::vec3> vertexPosition;
  std::vector<glm::vec2> vertexTexcoord;
  std::vector<glm::vec3> vectexNormal;

  std::vector<glm::ivec3> positionIndex;
  std::vector<glm::ivec3> texcoordIndex;
  std::vector<glm::ivec3> normalIndex;

  std::ifstream fin(filepath);
  std::string line;

  if (!fin.is_open()) {
    throw FileException{filepath, FileErrorID::ObjFileFailedToOpen};
  }

  while (std::getline(fin, line)) {
    std::istringstream sin(line);
    std::string type;
    GLfloat x, y, z;
    int v0, vt0,
        vn0; // fragment's first 1 vertex [position, texture, normal] index
    int v1, vt1, vn1; // 2
    int v2, vt2, vn2; // 3
    char slash;

    sin >> type;
    if (type == "v") {
      sin >> x >> y >> z;
      vertexPosition.emplace_back(glm::vec3(x, y, z));
    }
    if (type == "vt") {
      sin >> x >> y;
      vertexTexcoord.emplace_back(glm::vec2(x, y));
    }
    if (type == "vn") {
      sin >> x >> y >> z;
      vectexNormal.emplace_back(glm::vec3(x, y, z));
    }
    if (type == "f") {
      sin >> v0 >> slash >> vt0 >> slash >> vn0;
      sin >> v1 >> slash >> vt1 >> slash >> vn1;
      sin >> v2 >> slash >> vt2 >> slash >> vn2;
      positionIndex.emplace_back(glm::ivec3(v0 - 1, v1 - 1, v2 - 1));
      texcoordIndex.emplace_back(glm::ivec3(vt0 - 1, vt1 - 1, vt2 - 1));
      normalIndex.emplace_back(glm::ivec3(vn0 - 1, vn1 - 1, vn2 - 1));
    }
  }

  for (auto &&indx : positionIndex) {
    points.emplace_back(vertexPosition[indx.x]);
    points.emplace_back(vertexPosition[indx.y]);
    points.emplace_back(vertexPosition[indx.z]);
  }

  for (auto &&indx : texcoordIndex) {
    texcoords.emplace_back(vertexTexcoord[indx.x]);
    texcoords.emplace_back(vertexTexcoord[indx.y]);
    texcoords.emplace_back(vertexTexcoord[indx.z]);
  }
  for (auto &&indx : normalIndex) {
    normals.emplace_back(vectexNormal[indx.x]);
    normals.emplace_back(vectexNormal[indx.y]);
    normals.emplace_back(vectexNormal[indx.z]);
  }
}
void readOff(std::string const &filepath, std::vector<glm::vec3> &points,
             std::vector<glm::vec3> &colors) {
  std::string line;
  std::ifstream fin(filepath);
  if (!fin.is_open()) {
    throw FileException{filepath, FileErrorID::OFFFileFailedToOpen};
  }

  std::vector<glm::vec3> vertexPosition, vertexColor;
  fin >> line;
  int vertexNum, faceNum, edgeNum;
  fin >> vertexNum >> faceNum >> edgeNum;
  for (int i = 0; i < vertexNum; i++) {
    float p1, p2, p3;
    fin >> p1 >> p2 >> p3;
    vertexPosition.push_back(glm::vec3(p1, p2, p3));
    vertexColor.push_back(glm::vec3(p1 + 0.5, p2 + 0.5, p3 + 0.5));
  }
  points.clear();
  colors.clear();
  for (int i = 0; i < faceNum; i++) {
    int n, index1, index2, index3;
    fin >> n >> index1 >> index2 >> index3;
    points.emplace_back(vertexPosition[index1]);
    points.emplace_back(vertexPosition[index2]);
    points.emplace_back(vertexPosition[index3]);
    colors.emplace_back(vertexColor[index1]);
    colors.emplace_back(vertexColor[index2]);
    colors.emplace_back(vertexColor[index3]);
  }
  fin.close();
}
