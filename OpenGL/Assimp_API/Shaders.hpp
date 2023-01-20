#pragma once

#include "readFiles.hpp"

#include <GL/freeglut.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

enum class ShaderErrorID { VertexShaderError, FragmentShaderError, ShaderProgramError };

class ShaderException {
public:
  ShaderException(std::string, ShaderErrorID, GLchar*);
  std::string const &GetFilePath() const;
  ShaderErrorID const &GetErrorID() const;
  std::string const &GetInfoLog()const;

private:
  std::string m_filepath;
  ShaderErrorID m_shadererrorid;
  std::string m_infoLog;
};

ShaderException::ShaderException(std::string filepath, ShaderErrorID shadererrorid, GLchar* infoLog)
    : m_filepath{filepath}, m_shadererrorid{shadererrorid}, m_infoLog{infoLog} {};

std::string const &ShaderException::GetFilePath() const { return m_filepath; };

ShaderErrorID const &ShaderException::GetErrorID() const {return m_shadererrorid;};

std::string const &ShaderException::GetInfoLog() const {return m_infoLog;}

GLuint getShaderProgram(std::string fshader, std::string vshader) {
  // shader source file
  std::string vSource = readShaderFile(vshader);
  std::string fSource = readShaderFile(fshader);
  const char *vpointer = vSource.c_str();
  const char *fpointer = fSource.c_str();

  // error
  GLint success;
  GLchar infoLog[512];

  // create, pass GL languague to shader and compile vertex shading
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const GLchar **)(&vpointer), NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // error
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    throw ShaderException{vshader, ShaderErrorID::VertexShaderError, infoLog};
  }

  // create, pass GL language to shader and compile fragment shading
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const GLchar **)(&fpointer), NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // error
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    throw ShaderException{fshader, ShaderErrorID::FragmentShaderError, infoLog};
  }

  // bind shaders to program obj
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw ShaderException {vshader + fshader, ShaderErrorID::ShaderProgramError, infoLog};
  }

  // delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
