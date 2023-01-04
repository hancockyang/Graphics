#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "readFiles.hpp"
#include "Shaders.hpp"
#include "GlobalVariables.hpp"
#include "Control.hpp"
#include "IMVP.hpp"
#include <SOIL2/SOIL2.h>

namespace NodeData {
    GLsizei numOfNode{0};

    void UpdateNumOfNode(GLsizei num) 
    { 
        numOfNode = num; 
    };
}


void init() {

  std::vector<glm::vec3> points, normal;
  std::vector<glm::vec2> texcoords;

  try {
    readObj("models/table.obj", points, texcoords, normal);
  } catch (...) {
   throw;
  }
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GLuint dataSize =
      sizeof(glm::vec3) * static_cast<GLint>(points.size()) +
      sizeof(glm::vec2) * static_cast<GLint>(texcoords.size());
  glBufferData(GL_ARRAY_BUFFER,
               dataSize, NULL,
               GL_STATIC_DRAW);

  GLuint pointDataOffset = 0;
  GLuint textureDataOffset =
      static_cast<GLuint>(sizeof(glm::vec3) * points.size());
  glBufferSubData(GL_ARRAY_BUFFER, pointDataOffset,
                  sizeof(glm::vec3) * points.size(), &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, textureDataOffset,
                  sizeof(glm::vec2) * texcoords.size(), &texcoords[0]);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  std::string fshaderPath = "shaders/fshader.fsh";
  std::string vshaderPath = "shaders/vshader.vsh";
  try {
    GlobalVariables::program = getShaderProgram(fshaderPath, vshaderPath);
  } catch(...) {
    throw;
  }
  glUseProgram(GlobalVariables::program);  // shader


  GLuint vlocation = glGetAttribLocation(GlobalVariables::program, "vPosition");  // vPosition
  glEnableVertexAttribArray(vlocation);
  glVertexAttribPointer(vlocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  GLuint tlocation = glGetAttribLocation(GlobalVariables::program, "vTexcoord");
  glEnableVertexAttribArray(tlocation);
  glVertexAttribPointer(tlocation, 2, GL_FLOAT, GL_FALSE, 0,
                        (GLvoid*)(sizeof(glm::vec3) * points.size()));
  
  //texture
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  //texture filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  int textureWidth, textureHeight;
  unsigned char *image = SOIL_load_image("models/table.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  NodeData::UpdateNumOfNode(static_cast<GLsizei>(points.size()));

  MVP::Camera::SetCamera(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                         glm::vec4{glm::radians(70.0f), 1.0f, 0.1f, 100.0f});
  MVP::ModelMatrix::CreateModelMatrix(glm::vec3{1.0f, 1.0f, 1.0f}, 
                                      glm::vec3{0.0f, 0.0f, 0.0f}, 
                                      glm::vec3{-90.0f, 0.0f, 0.0f});
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Control::moveKeyboard();
  
  //model matrix
  glm::mat4 const &modelmatrix = MVP::model_ptr->GetModelMatrix();
  GLuint modelLocation = glGetUniformLocation(GlobalVariables::program, "model");
  glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelmatrix));
  //view matrix
  auto viewmatrix = MVP::camera_ptr->GetViewMatrix();
  GLuint viewLocation = glGetUniformLocation(GlobalVariables::program, "view");
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewmatrix));
  //projection matrix
  auto projectionmatrix = MVP::camera_ptr->GetProjectionMatrix();
  GLuint projectionLocation = glGetUniformLocation(GlobalVariables::program, "projection");
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionmatrix));


  //draw
  glDrawArrays(GL_TRIANGLES, 0, NodeData::numOfNode);
  glutSwapBuffers();
}



int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(GlobalVariables::windowWidth, GlobalVariables::windowHeight);
  glutCreateWindow("Texture");

#ifdef __APPLE__
#else
  glewInit();
#endif
  try {
    init();
  } catch (FileException const & e) {
    std::cout << "Failed to open file: " << e.GetFileName() << std::endl;
    std::cout << "Current working folder is: " << e.GetCurrentFolder() << std::endl;
    exit(-1);
  } catch (ShaderException const &e) {

    auto const & ErrorID = e.GetErrorID();
    switch (ErrorID) {
    case ShaderErrorID::ShaderProgramError:
        std::cout << "Shader program error\n";
    case ShaderErrorID::VertexShaderError:
        std::cout << "Shader file: " << e.GetFilePath() << "has "<<std::endl;
        std::cout << "Vertex shader error: " << e.GetInfoLog() << std::endl;
        break;
    case ShaderErrorID::FragmentShaderError:
        std::cout << "Shader file: " << e.GetFilePath() << "has "<<std::endl;
        std::cout << "Fragment shader error: " << e.GetInfoLog() << std::endl;
        break;
    default:
      break;
    }
    exit(-1);
  } catch (...) {
    std::cout << "unknonw error"<<std::endl;
  }

  //register control events to CL
//  glutMotionFunc(&Control::rotationMouse);
//  glutMouseWheelFunc(&Control::scaleMouse);
  glutKeyboardFunc(&Control::downKeyboard);
  glutKeyboardUpFunc(&Control::upKeyboard);

  glutPassiveMotionFunc(&Control::rotationMouse);
  //ctrl
  glutSpecialFunc(&Control::downKeyboardSpecial);
  glutSpecialUpFunc(&Control::upKeyboardSpecial);

  glutDisplayFunc(&display);
  glutMainLoop();

  return 0;
}
