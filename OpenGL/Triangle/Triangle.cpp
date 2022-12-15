#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Shaders.hpp"


void init()
{

    std::vector<glm::vec3> points = {
        glm::vec3(-0.5, -0.5, 0),
        glm::vec3(0.5, -0.5, 0),
        glm::vec3(0, 0.5, 0)
    };
    std::vector<glm::vec3> colors = {
        glm::vec3(1, 0, 0), // red
        glm::vec3(0, 1, 0), // green
        glm::vec3(0, 0, 1)  // blue
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (points.size() + colors.size()), NULL, GL_STATIC_DRAW);

    GLuint pointDataOffset = 0;
    GLuint colorDataOffset =
        static_cast<GLuint>(sizeof(glm::vec3) * points.size());
    glBufferSubData(GL_ARRAY_BUFFER, pointDataOffset, sizeof(glm::vec3) * points.size(), &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, colorDataOffset, sizeof(glm::vec3) * colors.size(), &colors[0]);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    std::string fshaderPath = "shaders/fshader.fsh";
    std::string vshaderPath = "shaders/vshader.vsh";
    GLuint program = getShaderProgram(fshaderPath, vshaderPath);
    glUseProgram(program);  // shader

    GLuint vlocation = glGetAttribLocation(program, "vPosition");    // vPosition 
    glEnableVertexAttribArray(vlocation);
    glVertexAttribPointer(vlocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  

    GLuint clocation = glGetAttribLocation(program, "vColor");  
    glEnableVertexAttribArray(clocation);
    glVertexAttribPointer(clocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec3) * points.size()));  

    glClearColor(1.0, 1.0, 1.0, 1.0);  
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);       
    glDrawArrays(GL_TRIANGLES, 0, 3);    
    glutSwapBuffers();                  
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);              
    glutInitDisplayMode(GLUT_RGBA);     
    glutInitWindowSize(512, 512);       
    glutCreateWindow("hello triangle");

#ifdef __APPLE__
#else
	glewInit();
#endif

    init();

    glutDisplayFunc(display);          
    glutMainLoop();                    

    return 0;
}