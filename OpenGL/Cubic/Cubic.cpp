#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Colors.hpp"
#include "Pointers.hpp"
#include "Faces.hpp"
#include "Shaders.hpp"
#include "Constants.hpp"
#include 


void loadVertexAndColors(std::vector<glm::vec3> &points,
                         std::vector<glm::vec3> &colors) {
  for (auto const &face : Faces::faces) {
    const auto idx1 = face[0];
    const auto idx2 = face[1];
    const auto idx3 = face[2];

    points.push_back(Points::points[idx1]);
    points.push_back(Points::points[idx2]);
    points.push_back(Points::points[idx3]);

    colors.push_back(Colors::colors[idx1]);
    colors.push_back(Colors::colors[idx2]);
    colors.push_back(Colors::colors[idx3]);
  }
}

namespace NodeData {
    GLsizei numOfNode{0};

    void UpdateNumOfNode(GLsizei num) 
    { 
        numOfNode = num; 
    };
}


void init() {

  std::vector<glm::vec3> points, colors;

  loadVertexAndColors(points, colors);

  

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
  GLuint program = getShaderProgram(fshaderPath, vshaderPath);
  glUseProgram(program);  // shader


  GLuint vlocation = glGetAttribLocation(program, "vPosition");  // vPosition
  glEnableVertexAttribArray(vlocation);
  glVertexAttribPointer(vlocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  GLuint clocation = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(clocation);
  glVertexAttribPointer(clocation, 3, GL_FLOAT, GL_FALSE, 0,
                        (GLvoid*)(sizeof(glm::vec3) * points.size()));

  //tranform
  // scale
  glm::mat4 scale = glm::scale(Constants::unit, glm::vec3{0.6, 0.6, 0.6});
  // translate
  glm::mat4 translate = glm::translate(Constants::unit, glm::vec3{-.3, 0.0, 0.0});
  //rotation
  glm::mat4 rotate =
      glm::rotate(Constants::unit, glm::radians(-30.0f), glm::vec3{1, 0, 0});
  rotate =
      glm::rotate(rotate, glm::radians(20.0f), glm::vec3{0, 1, 0});
  rotate =
      glm::rotate(rotate, glm::radians(0.0f), glm::vec3{0, 1, 1});

  glm::mat4 model = translate * rotate * scale;
  //pass uniform 
  GLuint mlocation = glGetUniformLocation(program, "model");
  glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(model));

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  NodeData::UpdateNumOfNode(static_cast<GLsizei>(points.size()));
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, NodeData::numOfNode);
  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Hello Cubic");

#ifdef __APPLE__
#else
  glewInit();
#endif

  init();

  glutDisplayFunc(&display);
  glutMainLoop();

  return 0;
}
