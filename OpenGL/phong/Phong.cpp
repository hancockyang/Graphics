
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "GlobalVariables.hpp"
#include "IMVP.hpp"
#include "Control.hpp"
#include "Shaders.hpp"
#include "FileException.hpp"

#include <GL/freeglut.h>
#include <SOIL2/SOIL2.h>

std::shared_ptr<MVP::Camera> camera_ptr;
std::shared_ptr<Model> lightsource_ptr;

std::vector<std::unique_ptr<Model>> models;

void init(glm::vec3&& cameralocation, glm::vec3&& camerafacingdirection,
          glm::vec3&& cameralookupdirection)
{
    std::string fshaderPath = "shaders/fshader.fsh";
    std::string vshaderPath = "shaders/vshader.vsh";
    try
    {
        Shader::GetShaderProgram(fshaderPath, vshaderPath);
    }
    catch ( ... )
    {
        throw;
    }
    glUseProgram(Shader::GetShaderProgram()); // shader

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    camera_ptr = std::make_shared<MVP::Camera>(std::move(cameralocation),
                                               std::move(camerafacingdirection),
                                               std::move(cameralookupdirection),
                                               glm::vec4 {glm::radians(70.0f), 1.0f, 0.01f, 100.0f});
    try
    {
        models.emplace_back(LoadAModel(
            glm::vec3 {1.0f, 1.0f, 1.0f}, // scaling
            glm::vec3 {0.0f, -.1f, 0.0f},   // translating
            glm::vec3 {0.0f, -90.0f, 0.0f},    // rotation
            "models/plane/plane.obj"));

        models.emplace_back(LoadAModel(
            glm::vec3 {.00025f, 0.00025f, 0.00025f}, // scaling
            glm::vec3 {0.25f, 0.0f, -1.0f},   // translating
            glm::vec3 {0.0f, 0.0f, 0.0f},    // rotation
            "models/tree/tree02.obj"));

        models.emplace_back(LoadAModel(
            glm::vec3 {.00015f, 0.00015f, 0.00015f}, // scaling
            glm::vec3 {1.0f, 0.0f, -1.0f},   // translating
            glm::vec3 {0.0f, 0.0f, 0.0f},    // rotation
            "models/tree/tree02.obj"));

        lightsource_ptr = std::move(LoadAModel(
            glm::vec3 {0.003f, 0.003f, 0.003f}, // scaling
            glm::vec3 {.0f, 1.0f, .0f},   // translating
            glm::vec3 {0.0f, 0.0f, 0.0f},    // rotation
            "models/duck/12248_Bird_v1_L2.obj"));
        KeyboardAndMouse::GetOrSetControl(std::shared_ptr<MVP::Camera>{camera_ptr}, std::shared_ptr<Model>{lightsource_ptr});
    }
    catch ( ... )
    {
        throw;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    KeyboardAndMouse::ForwardMoveKeyboardFptr();
    KeyboardAndMouse::GetOrSetControl().moveLightSource();

    // view matrix
    auto viewmatrix = camera_ptr->GetViewMatrix();
    GLuint viewLocation = glGetUniformLocation(Shader::GetShaderProgram(), "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewmatrix));
    // projection matrix
    auto projectionmatrix = camera_ptr->GetProjectionMatrix();
    GLuint projectionLocation =
        glGetUniformLocation(Shader::GetShaderProgram(), "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE,
                       glm::value_ptr(projectionmatrix));

    glUniform3fv(glGetUniformLocation(Shader::GetShaderProgram(), "lightpos"), 1, glm::value_ptr(lightsource_ptr->GetPosition()));

    glUniform3fv(glGetUniformLocation(Shader::GetShaderProgram(), "camerapos"), 1, glm::value_ptr(camera_ptr->GetPosition()));

    // draw
    for ( auto const& model : models )
    {
        model->Draw(Shader::GetShaderProgram());
    }
    lightsource_ptr->Draw(Shader::GetShaderProgram());
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(GlobalVariables::windowWidth,
                       GlobalVariables::windowHeight);
    glutCreateWindow("Texture");

#ifdef __APPLE__
#else
    glewInit();
#endif
    glm::vec3 cameralocation {0.0f, 0.0f, 0.0f};
    glm::vec3 camerafacingdirection {0.0f, 0.0f, -1.0f};
    glm::vec3 cameralookupdirection {0.0f, 1.0f, 0.0f};
    try
    {
        init(std::move(cameralocation), std::move(camerafacingdirection),
             std::move(cameralookupdirection));
    }
    catch ( FileException const& e )
    {
        std::cout << "Failed to open file: " << e.GetFileName() << std::endl;
        std::cout << "Current working folder is: " << e.GetCurrentFolder()
            << std::endl;
        exit(-1);
    }
    catch ( Shader::ShaderException const& e )
    {
        auto const& ErrorID = e.GetErrorID();
        switch ( ErrorID )
        {
            case Shader::ShaderErrorID::ShaderProgramError:
                std::cout << "Shader program error\n";
            case Shader::ShaderErrorID::VertexShaderError:
                std::cout << "Shader file: " << e.GetFilePath() << "has " << std::endl;
                std::cout << "Vertex shader error: " << e.GetInfoLog() << std::endl;
                break;
            case Shader::ShaderErrorID::FragmentShaderError:
                std::cout << "Shader file: " << e.GetFilePath() << "has " << std::endl;
                std::cout << "Fragment shader error: " << e.GetInfoLog() << std::endl;
                break;
            default:
                break;
        }
        exit(-1);
    }
    catch ( ... )
    {
        std::cout << "unknonw error" << std::endl;
    }

    // register control events to CL
    //  glutMouseWheelFunc(&Control::scaleMouse);
    glutKeyboardFunc(&KeyboardAndMouse::ForwardDownKeyboardFptr);
    glutKeyboardUpFunc(&KeyboardAndMouse::ForwardUpKeyboardFptr);

    glutPassiveMotionFunc(&KeyboardAndMouse::ForwardRotationMouseFptr);
    // ctrl
    glutSpecialFunc(&KeyboardAndMouse::ForwardDownKeyboardSpecialFptr);
    glutSpecialUpFunc(&KeyboardAndMouse::ForwardUpKeyboardSpecialFptr);

    glutDisplayFunc(&display);
    glutMainLoop();

    return 0;
}
