#include "core/camera.h"

Camera::Camera(glm::vec3 position)
{
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 45.0f;
    zNear = 0.1f;
    zFar = 100.0f;
    cameraPos = position;

    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
    worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

    UpdateCameraVectors();
}

void Camera::Rotate(double dx, double dy) {
    yaw += dx;
    pitch += dy;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::Move(CameraDirection direction, float amount)
{
    switch (direction) {
    case CameraDirection::FORWARD:
        cameraPos += cameraForward * amount;
        break;
    case CameraDirection::BACKWARD:
        cameraPos -= cameraForward * amount;
        break;
    case CameraDirection::RIGHT:
        cameraPos += cameraRight * amount;
        break;
    case CameraDirection::LEFT:
        cameraPos -= cameraRight * amount;
        break;
    case CameraDirection::UP:
        cameraPos += cameraUp * amount;
        break;
    case CameraDirection::DOWN:
        cameraPos -= cameraUp * amount;
        break;
    }
}

void Camera::Zoom(double dy) {
    if (fov >= 1.0f && fov <= 45.0f) {
        fov -= dy;
    }
    else if (fov < 1.0f) {
        fov = 1.0f;
    }
    else { // > 45.0f
        fov = 45.0f;
    }
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraForward, cameraUp);
}

void Camera::UpdateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraForward = glm::normalize(direction);

    cameraRight = glm::normalize(glm::cross(cameraForward, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));
}