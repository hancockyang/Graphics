#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace GlobalVariables {
GLuint program;

glm::vec3 scaleControl{1, 1, 1};
glm::vec3 rotationControl{0, 0, 0};
glm::vec3 translationControl{0, 0, 0};


int windowWidth = 512;
int windowHeight = 512;

bool keyBoardState[1024];

}; // namespace GlobalVaribles
