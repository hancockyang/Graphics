#include "GlobalVariables.hpp"
#include "Model.hpp"
#include "IMVP.hpp"
#include <iostream>


namespace MVP
{

    Camera::Camera(glm::vec3&& cameraPosition, glm::vec3&& cameraDirection,
                   glm::vec3&& globalUp, glm::vec4&& frustum)
        : m_cameraPosition {std::move(cameraPosition)},
        m_cameraFacingDirection {std::move(cameraDirection)},
        m_globalUpDirection {std::move(globalUp)}, m_cameraFrustum {std::move(frustum)},
        m_cameraThirdDirection {glm::normalize(
            glm::cross(m_cameraFacingDirection, m_globalUpDirection))},
        m_eulerAngle {0.0f, 0.0f, 0.0f}, m_isviewupdated {false},
        m_isfrustumupdated {false}, m_viewmatrix {CalViewMatrix()},
        m_projectionmatrix {CalProjectionMatrix()}
    {};

    glm::vec3 const& Camera::GetPosition()
    {
        return m_cameraPosition;
    }

    void Camera::UpdateCameraCoordinate()
    {
        m_cameraFacingDirection.x =
            cos(glm::radians(m_eulerAngle.x)) * sin(glm::radians(m_eulerAngle.y));
        m_cameraFacingDirection.y = sin(glm::radians(m_eulerAngle.x));
        m_cameraFacingDirection.z =
            -cos(glm::radians(m_eulerAngle.x)) * cos(glm::radians(m_eulerAngle.y));

        m_cameraThirdDirection =
            glm::normalize(glm::cross(m_cameraFacingDirection, m_globalUpDirection));
    };

    void Camera::MoveCamera(float const& moveAmound,
                            KeyboardAndMouse::KeyboardMove moveDirection)
    {
        std::shared_ptr<void> ptr(nullptr, [this](void*)
                                  {
                                      m_isviewupdated = false;
                                  });
        switch ( moveDirection )
        {
            case KeyboardAndMouse::KeyboardMove::Forward:
                m_cameraPosition += moveAmound * m_cameraFacingDirection;
                m_isviewupdated = true;
                break;
            case KeyboardAndMouse::KeyboardMove::Backward:
                m_cameraPosition -= moveAmound * m_cameraFacingDirection;
                m_isviewupdated = true;
                break;
            case KeyboardAndMouse::KeyboardMove::Left:
                m_cameraPosition -= moveAmound * m_cameraThirdDirection;
                m_isviewupdated = true;
                break;
            case KeyboardAndMouse::KeyboardMove::Right:
                m_cameraPosition += moveAmound * m_cameraThirdDirection;
                m_isviewupdated = true;
                break;
            case KeyboardAndMouse::KeyboardMove::Up:
                m_cameraPosition += moveAmound * m_globalUpDirection;
                m_isviewupdated = true;
                break;
            case KeyboardAndMouse::KeyboardMove::Down:
                m_cameraPosition -= moveAmound * m_globalUpDirection;
                m_isviewupdated = true;
                break;
            default:
                break;
        }
    }

    void Camera::RotateCamera(float const& x, float const& y, float const& windowW,
                              float const& windowH)
    {
        std::shared_ptr<void> ptr(nullptr, [this](void*)
                                  {
                                      m_isviewupdated = false;
                                  });
        // EulerAgnle
        m_eulerAngle.y += 35.0f * (x - windowW / 2.0f) / windowW; // yaw
        m_eulerAngle.y =
            glm::mod(m_eulerAngle.y + 180.0f, 360.0f) - 180.0f; // -180 ~ 180

        m_eulerAngle.x += -35.0f * (y - windowH / 2.0f) / windowH; // pitch
        m_eulerAngle.x = glm::clamp(m_eulerAngle.x, -89.0f, 89.0f);
        UpdateCameraCoordinate();
    }

    glm::mat4 const Camera::CalViewMatrix()
    {
        std::shared_ptr<void> ptr(nullptr, [this](void*)
                                  {
                                      m_isviewupdated = true;
                                  });
        return glm::lookAt(m_cameraPosition,
                           m_cameraPosition + m_cameraFacingDirection,
                           m_globalUpDirection);
    }

    glm::mat4 const& Camera::GetViewMatrix()
    {
        return m_isviewupdated ? m_viewmatrix : m_viewmatrix = CalViewMatrix();
    }

    glm::mat4 const Camera::CalProjectionMatrix()
    {
        std::shared_ptr<void> ptr(nullptr, [this](void*)
                                  {
                                      m_isfrustumupdated = true;
                                  });
        return glm::perspective(m_cameraFrustum.x, m_cameraFrustum.y,
                                m_cameraFrustum.z, m_cameraFrustum.w);
    }

    glm::mat4 const& Camera::GetProjectionMatrix()
    {
        return m_isfrustumupdated ? m_projectionmatrix
            : m_projectionmatrix = CalProjectionMatrix();
    }
}
