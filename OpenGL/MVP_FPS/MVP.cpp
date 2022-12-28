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

namespace NodeData {
    GLsizei numOfNode{0};

    void UpdateNumOfNode(GLsizei num) 
    { 
        numOfNode = num; 
    };
}


void init() {

  std::vector<glm::vec3> points, colors;

  readOff("models/Pikachu.OFF", points, colors);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(glm::vec3) * (points.size() + colors.size()), NULL,
               GL_STATIC_DRAW);

  GLuint pointDataOffset = 0;
  GLuint colorDataOffset =
      static_cast<GLuint>(sizeof(glm::vec3) * points.size());
  glBufferSubData(GL_ARRAY_BUFFER, pointDataOffset,
                  sizeof(glm::vec3) * points.size(), &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, colorDataOffset,
                  sizeof(glm::vec3) * colors.size(), &colors[0]);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  std::string fshaderPath = "shaders/fshader.fsh";
  std::string vshaderPath = "shaders/vshader.vsh";
  GlobalVariables::program = getShaderProgram(fshaderPath, vshaderPath);
  glUseProgram(GlobalVariables::program);  // shader


  GLuint vlocation = glGetAttribLocation(GlobalVariables::program, "vPosition");  // vPosition
  glEnableVertexAttribArray(vlocation);
  glVertexAttribPointer(vlocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  GLuint clocation = glGetAttribLocation(GlobalVariables::program, "vColor");
  glEnableVertexAttribArray(clocation);
  glVertexAttribPointer(clocation, 3, GL_FLOAT, GL_FALSE, 0,
                        (GLvoid*)(sizeof(glm::vec3) * points.size()));


  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  NodeData::UpdateNumOfNode(static_cast<GLsizei>(points.size()));

  MVP::Camera::SetCamera(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                         glm::vec4{glm::radians(70.0f), 1.0f, 0.1f, 100.0f});
  MVP::ModelMatrix::CreateModelMatrix();
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
  glutCreateWindow("Control");

#ifdef __APPLE__
#else
  glewInit();
#endif

  init();

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
