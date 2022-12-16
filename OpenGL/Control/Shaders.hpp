#pragma once

#include "readFiles.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

GLuint getShaderProgram(std::string fshader, std::string vshader)
{
    // shader source file
    std::string vSource = readShaderFile(vshader);
    std::string fSource = readShaderFile(fshader);
    const char* vpointer = vSource.c_str();
    const char* fpointer = fSource.c_str();
   
    // error
    GLint success;
    GLchar infoLog[512];

    // create, pass GL languague to shader and compile vertex shading
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)(&vpointer), NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);   // error
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader error\n" << infoLog << std::endl;
        exit(-1);
    }

    // create, pass GL language to shader and compile fragment shading
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)(&fpointer), NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   // error
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader error\n" << infoLog << std::endl;
        exit(-1);
    }

    // bind shaders to program obj
    GLuint shaderProgram = glCreateProgram(); 
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program error\n" << infoLog << std::endl;
        exit(-1);
    }

    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
} 
