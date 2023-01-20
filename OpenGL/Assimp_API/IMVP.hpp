#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include <memory>

#include "Control.hpp"

namespace MVP {

glm::mat4 const unit{glm::vec4{1, 0, 0, 0}, glm::vec4{0, 1, 0, 0},
                     glm::vec4{0, 0, 1, 0}, glm::vec4{0, 0, 0, 1}};

class Camera;
class ModelMatrix;

std::unique_ptr<Camera> camera_ptr;
std::unique_ptr<ModelMatrix> model_ptr;

class Camera {
public:
  static void SetCamera(glm::vec3 &&, glm::vec3 &&, glm::vec3 &&, glm::vec4 &&);

  void MoveCamera(float const &, Control::KeyboadMove);
  void RotateCamera(float const &, float const &, float const &, float const &);
  Camera(glm::vec3 &&, glm::vec3 &&, glm::vec3 &&, glm::vec4 &&);

  glm::mat4 const &GetViewMatrix();
  glm::mat4 const &GetProjectionMatrix();

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

class ModelMatrix {
public:
  static void CreateModelMatrix(glm::vec3 &&, glm::vec3 &&, glm::vec3 &&);
  ModelMatrix(glm::vec3 &&, glm::vec3 &&, glm::vec3 &&);
  glm::mat4 const &GetModelMatrix();

private:
  bool m_ismodelmatrixupdated;
  glm::vec3 m_scaling;
  glm::vec3 m_translation;
  glm::vec3 m_rotation;
  glm::mat4 const CalModelMatrix();

  glm::mat4 m_modelmatrix;
};

Camera::Camera(glm::vec3 &&cameraPosition, glm::vec3 &&cameraDirection,
               glm::vec3 &&globalUp, glm::vec4 &&frustum)
    : m_cameraPosition{std::move(cameraPosition)},
      m_cameraFacingDirection{std::move(cameraDirection)},
      m_globalUpDirection{std::move(globalUp)}, m_cameraFrustum{std::move(
                                                    frustum)},
      m_cameraThirdDirection{glm::normalize(
          glm::cross(m_cameraFacingDirection, m_globalUpDirection))},
      m_eulerAngle{0.0f, 0.0f, 0.0f}, m_isviewupdated{false},
      m_isfrustumupdated{false}, m_viewmatrix{CalViewMatrix()},
      m_projectionmatrix{CalProjectionMatrix()} {
  // register callback from camera parameters update
  Control::MouseMoveCallback = [&](float const &w, float const &h,
                                   float const &x, float const &y) {
    RotateCamera(x, y, w, h);
  };
  Control::KeyboardMoveCallbacks[Control::KeyboadMove::Forward] =
      [this](const float &val) { MoveCamera(val, Control::KeyboadMove::Forward); };
  Control::KeyboardMoveCallbacks[Control::KeyboadMove::Backward] =
      [this](const float &val) {
        MoveCamera(val, Control::KeyboadMove::Backward);
      };
  Control::KeyboardMoveCallbacks[Control::KeyboadMove::Left] =
      [this](const float &val) { MoveCamera(val, Control::KeyboadMove::Left); };
  Control::KeyboardMoveCallbacks[Control::KeyboadMove::Right] =
      [this](const float &val) { MoveCamera(val, Control::KeyboadMove::Right); };
  Control::KeyboardMoveCallbacks[Control::KeyboadMove::Up] =
      [this](const float &val) { MoveCamera(val, Control::KeyboadMove::Up); };
  Control::KeyboardMoveCallbacks[Control::KeyboadMove::Down] =
      [this](const float &val) { MoveCamera(val, Control::KeyboadMove::Down); };
};

void Camera::SetCamera(glm::vec3 &&cameraPosition, glm::vec3 &&cameraDirection,
                       glm::vec3 &&globalUp, glm::vec4 &&frustum) {
  camera_ptr = std::move(std::make_unique<Camera>(
      std::move(cameraPosition), std::move(cameraDirection),
      std::move(globalUp), std::move(frustum)));
}

void Camera::UpdateCameraCoordinate() {
  m_cameraFacingDirection.x =
      cos(glm::radians(m_eulerAngle.x)) * sin(glm::radians(m_eulerAngle.y));
  m_cameraFacingDirection.y = sin(glm::radians(m_eulerAngle.x));
  m_cameraFacingDirection.z =
      -cos(glm::radians(m_eulerAngle.x)) * cos(glm::radians(m_eulerAngle.y));

  m_cameraThirdDirection =
      glm::normalize(glm::cross(m_cameraFacingDirection, m_globalUpDirection));
};

void Camera::MoveCamera(float const &moveAmound,
                        Control::KeyboadMove moveDirection) {
  std::shared_ptr<void> ptr(nullptr,
                            [this](void *) { m_isviewupdated = false; });
  switch (moveDirection) {

  case Control::KeyboadMove::Forward:
    m_cameraPosition += moveAmound * m_cameraFacingDirection;
    m_isviewupdated = true;
    break;
  case Control::KeyboadMove::Backward:
    m_cameraPosition -= moveAmound * m_cameraFacingDirection;
    m_isviewupdated = true;
    break;
  case Control::KeyboadMove::Left:
    m_cameraPosition += moveAmound * m_cameraThirdDirection;
    m_isviewupdated = true;
    break;
  case Control::KeyboadMove::Right:
    m_cameraPosition -= moveAmound * m_cameraThirdDirection;
    m_isviewupdated = true;
    break;
  case Control::KeyboadMove::Up:
    m_cameraPosition += moveAmound * m_globalUpDirection;
    m_isviewupdated = true;
    break;
  case Control::KeyboadMove::Down:
    m_cameraPosition -= moveAmound * m_globalUpDirection;
    m_isviewupdated = true;
    break;
  default:
    break;
  }
}

void Camera::RotateCamera(float const &x, float const &y, float const &windowW,
                          float const &windowH) {
  std::shared_ptr<void> ptr(nullptr,
                            [this](void *) { m_isviewupdated = false; });
  // EulerAgnle
  m_eulerAngle.y += 35.0f * (x - windowW / 2.0f) / windowW; // yaw
  m_eulerAngle.y =
      glm::mod(m_eulerAngle.y + 180.0f, 360.0f) - 180.0f; // -180 ~ 180

  m_eulerAngle.x += -35.0f * (y - windowH / 2.0f) / windowH; // pitch
  m_eulerAngle.x = glm::clamp(m_eulerAngle.x, -89.0f, 89.0f);
  UpdateCameraCoordinate();
}

glm::mat4 const Camera::CalViewMatrix() {
  std::shared_ptr<void> ptr(nullptr,
                            [this](void *) { m_isviewupdated = true; });
  return glm::lookAt(m_cameraPosition,
                     m_cameraPosition + m_cameraFacingDirection,
                     m_globalUpDirection);
}

glm::mat4 const &Camera::GetViewMatrix() {
  return m_isviewupdated ? m_viewmatrix : m_viewmatrix = CalViewMatrix();
}

glm::mat4 const Camera::CalProjectionMatrix() {
  std::shared_ptr<void> ptr(nullptr,
                            [this](void *) { m_isfrustumupdated = true; });
  return glm::perspective(m_cameraFrustum.x, m_cameraFrustum.y,
                          m_cameraFrustum.z, m_cameraFrustum.w);
}

glm::mat4 const &Camera::GetProjectionMatrix() {
  return m_isfrustumupdated ? m_projectionmatrix
                            : m_projectionmatrix = CalProjectionMatrix();
}

ModelMatrix::ModelMatrix(glm::vec3 &&scaling, glm::vec3 &&translation,
                         glm::vec3 &&rotation)
    : m_scaling{std::move(scaling)}, m_translation{std::move(translation)},
      m_rotation{std::move(rotation)}, m_ismodelmatrixupdated{false},
      m_modelmatrix{CalModelMatrix()} {}


void ModelMatrix::CreateModelMatrix(
    glm::vec3 &&scaling = glm::vec3{1.0f, 1.0f, 1.0f},
    glm::vec3 &&translation = glm::vec3{0.0f, 0.0f, 0.0f},
    glm::vec3 &&rotation = glm::vec3{0.0f, 0.0f, 0.0f}) {
  model_ptr = std::move(std::make_unique<ModelMatrix>(
      std::move(scaling), std::move(translation), std::move(rotation)));
}

glm::mat4 const ModelMatrix::CalModelMatrix() {
  std::shared_ptr<void> ptr(nullptr, [this](void *) {m_ismodelmatrixupdated = true;});

  glm::mat4 scale = glm::scale(unit, m_scaling);
  // translate
  glm::mat4 translate = glm::translate(unit, m_translation);
  // rotation
  glm::mat4 rotate =
      glm::rotate(unit, glm::radians(m_rotation.x), glm::vec3{1, 0, 0});
  rotate = glm::rotate(rotate, glm::radians(m_rotation.y), glm::vec3{0, 1, 0});
  rotate = glm::rotate(rotate, glm::radians(m_rotation.z), glm::vec3{0, 0, 1});

  return translate * rotate * scale;
}

glm::mat4 const &ModelMatrix::GetModelMatrix() {
  return m_ismodelmatrixupdated ? m_modelmatrix
                                : m_modelmatrix = CalModelMatrix();
}


} // namespace MVP
