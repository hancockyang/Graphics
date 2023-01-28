#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

#include "IMVP.hpp"
#include "Model.hpp"

namespace KeyboardAndMouse
{

    enum class LightMove
    {
        Forward, Backward, Left, Right, Down, Up
    };

    class Control
    {
    public:
        Control(std::shared_ptr<MVP::Camera>&& camera, std::shared_ptr<Model>&& lightsource);
        void rotationMouse(int x, int y);

        // void scaleMouse(int wheel, int direction, int x, int y) {
        //   scaleControl += 1 * direction * 0.1f;
        //
        //   glutPostRedisplay();
        // }

        void moveKeyboard();

        void moveLightSource();

        void downKeyboard(unsigned char key, int x, int y);

        void downKeyboardSpecial(int key, int x, int y);

        void upKeyboard(unsigned char key, int x, int y);

        void upKeyboardSpecial(int key, int x, int y);

    private:
        std::shared_ptr<MVP::Camera> m_camera;
        std::shared_ptr<Model> m_lightsource;
        bool m_keyBoardState[1024];

        void updateLightSourcePosition(glm::vec3&& translation);
    };

    Control& GetOrSetControl(std::shared_ptr<MVP::Camera>&& camera = nullptr, std::shared_ptr<Model> && lightsource = nullptr);


    void ForwardRotationMouseFptr(int x, int y);

    // void scaleMouse(int wheel, int direction, int x, int y) {
    //   scaleControl += 1 * direction * 0.1f;
    //
    //   glutPostRedisplay();
    // }


    void ForwardMoveKeyboardFptr();

    void ForwardDownKeyboardFptr(unsigned char key, int x, int y);

    void ForwardDownKeyboardSpecialFptr(int key, int x, int y);

    void ForwardUpKeyboardFptr(unsigned char key, int x, int y);

    void ForwardUpKeyboardSpecialFptr(int key, int x, int y);

}; // namespace KeyboardAndMouse
