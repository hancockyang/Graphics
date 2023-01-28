#include "Shaders.hpp"
#include "readFiles.hpp"


namespace Shader 
{
  ShaderException::ShaderException(std::string filepath,
                                   ShaderErrorID shadererrorid, GLchar *infoLog)
      : m_filepath{filepath}, m_shadererrorid{shadererrorid}, m_infoLog{
                                                                  infoLog} {};
  
  std::string const &ShaderException::GetFilePath() const { return m_filepath; };
  
  ShaderErrorID const &ShaderException::GetErrorID() const {
    return m_shadererrorid;
  };
  
  std::string const &ShaderException::GetInfoLog() const { return m_infoLog; }


  GLuint const &GetShaderProgram(std::string const & fshader, std::string const & vshader) 
  {
    static GLuint program = ReadShaderProgram(fshader, vshader);  
    return program;
  }
  
  
  GLuint ReadShaderProgram(std::string const & fshader, std::string const & vshader) {
    // shader source file
    assert(fshader != "" && vshader != "");
    std::string fSource = readShaderFile(fshader);
    std::string vSource = readShaderFile(vshader);
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
      throw ShaderException{vshader + fshader, ShaderErrorID::ShaderProgramError,
                            infoLog};
    }
  
    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  
    return shaderProgram;
  }

}

