#include "Control.hpp"
#include "GlobalVariables.hpp"
#include <GL/freeglut.h>


namespace KeyboardAndMouse
{

    using namespace GlobalVariables;

    Control::Control(std::shared_ptr<MVP::Camera>&& camera, std::shared_ptr<Model>&& lightsource)
        : m_camera {std::move(camera)}, m_lightsource {std::move(lightsource)}, m_keyBoardState {}
    {
        assert(m_camera && m_lightsource);
    };

    void Control::updateLightSourcePosition(glm::vec3&& translation)
    {
        m_lightsource->MoveModel(std::move(translation));
    }

    void Control::rotationMouse(int x, int y)
    {

        m_camera->RotateCamera(static_cast<float>(x), static_cast<float>(y),
                               static_cast<float>(windowWidth),
                               static_cast<float>(windowHeight));
        glutWarpPointer(windowWidth / 2, windowHeight / 2); // fix at the center
        // re-draw
        glutPostRedisplay();
    }

    // void scaleMouse(int wheel, int direction, int x, int y) {
    //   scaleControl += 1 * direction * 0.1f;
    //
    //   glutPostRedisplay();
    // }


    void Control::moveLightSource()
    {
        float cameraspeed = 0.05f;
        if ( m_keyBoardState['i'] )
        {
           m_lightsource->MoveModel(glm::vec3{cameraspeed, 0.0f, 0.0f});
        }
        else if ( m_keyBoardState['k'] )
        {
           m_lightsource->MoveModel(glm::vec3{-cameraspeed, 0.0f, 0.0f});
        }
        else if ( m_keyBoardState['j'] )
        {
           m_lightsource->MoveModel(glm::vec3{0.0f, cameraspeed, 0.0f});
        }
        else if ( m_keyBoardState['l'] )
        {
           m_lightsource->MoveModel(glm::vec3{0.0f, -cameraspeed, 0.0f});
        }

    }

    void Control::moveKeyboard()
    {

        float cameraspeed = 0.05f;
        if ( m_keyBoardState['w'] )
        {
            m_camera->MoveCamera(cameraspeed, KeyboardMove::Forward);
        }
        else if ( m_keyBoardState['s'] )
        {
            m_camera->MoveCamera(cameraspeed, KeyboardMove::Backward);
        }
        else if ( m_keyBoardState['a'] )
        {
            m_camera->MoveCamera(cameraspeed, KeyboardMove::Left);
        }
        else if ( m_keyBoardState['d'] )
        {
            m_camera->MoveCamera(cameraspeed, KeyboardMove::Right);
        }
        else if ( m_keyBoardState[GLUT_KEY_CTRL_L] )
        {
            m_camera->MoveCamera(cameraspeed, KeyboardMove::Down);
        }
        else if ( m_keyBoardState[' '] )
        {
            m_camera->MoveCamera(cameraspeed, KeyboardMove::Up);
        }
    }


    Control& GetOrSetControl(std::shared_ptr<MVP::Camera>&& camera, std::shared_ptr<Model> && lightsource)
    {
        static Control control {std::move(camera), std::move(lightsource)};
        return control;
    }

    void Control::downKeyboard(unsigned char key, int x, int y)
    {
        m_keyBoardState[key] = true;
        glutPostRedisplay();
    }

    void Control::downKeyboardSpecial(int key, int x, int y)
    {
        m_keyBoardState[key] = true;
        glutPostRedisplay();
    }

    void Control::upKeyboard(unsigned char key, int x, int y)
    {
        m_keyBoardState[key] = false;
    }

    void Control::upKeyboardSpecial(int key, int x, int y)
    {
        m_keyBoardState[key] = false;
    }

    void ForwardRotationMouseFptr(int x, int y)
    {
        GetOrSetControl().rotationMouse(x, y);
    }

    void ForwardMoveKeyboardFptr()
    {
        GetOrSetControl().moveKeyboard();
    }


    void ForwardDownKeyboardFptr(unsigned char key, int x, int y)
    {
        GetOrSetControl().downKeyboard(key, x, y);

    }

    void ForwardDownKeyboardSpecialFptr(int key, int x, int y)
    {
        GetOrSetControl().downKeyboardSpecial(key, x, y);
    }


    void ForwardUpKeyboardFptr(unsigned char key, int x, int y)
    {
        GetOrSetControl().upKeyboard(key, x, y);
    }

    void ForwardUpKeyboardSpecialFptr(int key, int x, int y)
    {
        GetOrSetControl().upKeyboardSpecial(key, x, y);
    }


} // namespace KeyboardAndMouse
