#pragma once
#include "GlobalVariables.hpp"
#include <GL/freeglut.h>

namespace Control {

using namespace GlobalVariables;


void rotationMouse(int x, int y) {
  rotationControl.y = -100 * (x - static_cast<float>(windowWidth) / 2.0f) /
                      static_cast<float>(windowWidth);
  rotationControl.x = -100 * (y - static_cast<float>(windowHeight) / 2.0f) /
                      static_cast<float>(windowHeight);

  // re-draw
  glutPostRedisplay();
}

void scaleMouse(int wheel, int direction, int x, int y) {
  scaleControl += 1 * direction * 0.1f;

  glutPostRedisplay();
}

void moveKeyboard() 
{ 

  if (keyBoardState['w']) 
  {
      translationControl.y += 0.005f;
  }
  else if (keyBoardState['s']) 
  {
      translationControl.y -= 0.005f;
  }
  else if (keyBoardState['a']) 
  {
      translationControl.x -= 0.005f;
  }
  else if (keyBoardState['d']) 
  {
      translationControl.x += 0.005f;
  }
}

void downKeyboard(unsigned char key, int x, int y) 
{
  keyBoardState[key] = true;
  glutPostRedisplay();

}
void upKeyboard(unsigned char key, int x, int y) 
{
  keyBoardState[key] = false;
}


}; // namespace Control
