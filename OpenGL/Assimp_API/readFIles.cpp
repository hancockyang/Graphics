#pragma once
#include "readFiles.hpp"

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
