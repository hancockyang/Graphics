#pragma once
#include "GlobalVariables.hpp"
#include <GL/freeglut.h>
#include <iostream>
#include <unordered_map>
#include <functional>

namespace Control {

using namespace GlobalVariables;


std::function<void(float const&, float const&, float const&, float const&)> MouseMoveCallback;

enum class KeyboadMove {Forward, Backward, Left, Right, Down, Up};

std::unordered_map < KeyboadMove, std::function <
                                      void(float)>> KeyboardMoveCallbacks;

void rotationMouse(int x, int y) {
  
  MouseMoveCallback(static_cast<float>(windowWidth),
                    static_cast<float>(windowHeight), static_cast<float>(x),
                    static_cast<float>(y));
  glutWarpPointer(windowWidth / 2, windowHeight / 2); //fix at the center
  // re-draw
  glutPostRedisplay();
}

//void scaleMouse(int wheel, int direction, int x, int y) {
//  scaleControl += 1 * direction * 0.1f;
//
//  glutPostRedisplay();
//}

void moveKeyboard() 
{ 

  if (keyBoardState['w']) 
  {
    KeyboardMoveCallbacks[KeyboadMove::Forward](0.05f);
  } else if (keyBoardState['s']) 
  {
    KeyboardMoveCallbacks[KeyboadMove::Backward](0.05f);
  } else if (keyBoardState['a']) 
  {
    KeyboardMoveCallbacks[KeyboadMove::Left](0.05f);
  } else if (keyBoardState['d']) 
  {
    KeyboardMoveCallbacks[KeyboadMove::Right](0.05f);
  } else if (keyBoardState[GLUT_KEY_CTRL_L]) 
  {
    KeyboardMoveCallbacks[KeyboadMove::Down](0.05f);
  } else if (keyBoardState[' ']) 
  {
    KeyboardMoveCallbacks[KeyboadMove::Up](0.05f);
  }
}

void downKeyboard(unsigned char key, int x, int y) 
{
  keyBoardState[key] = true;
  glutPostRedisplay();

}

void downKeyboardSpecial(int key, int x, int y) 
{
  keyBoardState[key] = true;
  glutPostRedisplay();

}

void upKeyboard(unsigned char key, int x, int y) 
{
  keyBoardState[key] = false;
}

void upKeyboardSpecial(int key, int x, int y) 
{
  keyBoardState[key] = false;
}

}; // namespace Control
