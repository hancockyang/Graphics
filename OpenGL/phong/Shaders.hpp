#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Shader {

  enum class ShaderErrorID {
    VertexShaderError,
    FragmentShaderError,
    ShaderProgramError
  };
  
  class ShaderException {
  public:
    ShaderException(std::string, ShaderErrorID, GLchar *);
    std::string const &GetFilePath() const;
    ShaderErrorID const &GetErrorID() const;
    std::string const &GetInfoLog() const;
  
  private:
    std::string m_filepath;
    ShaderErrorID m_shadererrorid;
    std::string m_infoLog;
  };

  GLuint const &GetShaderProgram(std::string const &fshader = "",
                                 std::string const &vshader = "");
  GLuint ReadShaderProgram(std::string const & fshader, std::string const & vshader);


}

