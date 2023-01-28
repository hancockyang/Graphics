#pragma once


#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include <memory>

namespace KeyboardAndMouse
{

    enum class KeyboardMove
    {
        Forward, Backward, Left, Right, Down, Up
    };

}

namespace MVP
{

    glm::mat4 const unit {glm::vec4{1, 0, 0, 0}, glm::vec4{0, 1, 0, 0},
                         glm::vec4{0, 0, 1, 0}, glm::vec4{0, 0, 0, 1}};



    class Camera
    {

    public:

        void MoveCamera(float const&, KeyboardAndMouse::KeyboardMove);
        void RotateCamera(float const&, float const&, float const&, float const&);
        Camera(glm::vec3&&, glm::vec3&&, glm::vec3&&, glm::vec4&&);

        glm::mat4 const& GetViewMatrix();
        glm::mat4 const& GetProjectionMatrix();
        glm::vec3 const& GetPosition();

    private:
        void UpdateCameraCoordinate();

        glm::vec3 m_cameraPosition;
        glm::vec3 m_cameraFacingDirection;
        //  glm::vec3 cameraUpDirection;
        glm::vec3 m_cameraThirdDirection;
        glm::vec4 m_cameraFrustum; // fovy, aspect, zNear, zFar
        glm::vec3 m_globalUpDirection;
        glm::vec3 m_eulerAngle; // pitch, yaw, roll

        bool m_isviewupdated, m_isfrustumupdated;
        glm::mat4 m_viewmatrix;
        glm::mat4 m_projectionmatrix;

        glm::mat4 const CalViewMatrix();
        glm::mat4 const CalProjectionMatrix();
    };



    class ModelMatrix
    {
    public:
        ModelMatrix(glm::vec3&& scaling, glm::vec3&& translation, glm::vec3&& rotation);
        glm::mat4 const& GetModelMatrix() const;
        void UpdateTranslation(glm::vec3 && translation);

    private:
        glm::vec3 m_scaling;
        glm::vec3 m_translation;
        glm::vec3 m_rotation;
        bool m_ismodelmatrixupdated;
        glm::mat4 m_modelmatrix;

        glm::mat4 const CalModelMatrix();
    };




} // namespace MVP
