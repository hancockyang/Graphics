#pragma once
#include "Model.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

namespace GlobalVariables {
GLuint program;


int windowWidth = 512;
int windowHeight = 512;

bool keyBoardState[1024];

//views
float left{-1.0f}, right{1.0f}, bottom{-1.0f}, top{1.0f};

std::unique_ptr<Model> model;

}; // namespace GlobalVaribles
