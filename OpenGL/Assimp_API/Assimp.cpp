#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Shaders.hpp"
#include "GlobalVariables.hpp"
#include "Control.hpp"
#include "IMVP.hpp"
#include <SOIL2/SOIL2.h>


void init(glm::vec3 && cameralocation, glm::vec3 && camerafacingdirection, glm::vec3 && cameralookupdirection) {
  std::string fshaderPath = "shaders/fshader.fsh";
  std::string vshaderPath = "shaders/vshader.vsh";
  try {
    GlobalVariables::program = getShaderProgram(fshaderPath, vshaderPath);
  } catch(...) {
    throw;
  }
  glUseProgram(GlobalVariables::program);  // shader


  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  MVP::Camera::SetCamera(std::move(cameralocation), std::move(camerafacingdirection), std::move(cameralookupdirection),
                         glm::vec4{glm::radians(70.0f), 1.0f, 0.01f, 100.0f});
  MVP::ModelMatrix::CreateModelMatrix(glm::vec3{10.0f, 10.0f, 10.0f}, // scaling
                                      glm::vec3{0.0f, -1.1f, 0.0f},   // translating
                                      glm::vec3{0.0f, 0.0f, 0.0f});   // rotation
  try {
    GlobalVariables::model->Load("models/plane/plane.obj");
  } catch (...) {
    throw;
  }
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
  GlobalVariables::model->Draw(GlobalVariables::program);
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
  GlobalVariables::model = std::make_unique<Model>();
  glm::vec3 cameralocation {0.0f, 0.0f, 0.0f};
  glm::vec3 camerafacingdirection {0.0f, 0.0f, -1.0f};
  glm::vec3 cameralookupdirection {0.0f, 1.0f, 0.0f};
  try {
    init(std::move(cameralocation), std::move(camerafacingdirection), std::move(cameralookupdirection));
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
