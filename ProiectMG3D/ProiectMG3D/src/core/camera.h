#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 cameraPos;

    // direction vectors
    glm::vec3 cameraUp;
    glm::vec3 cameraForward;
    glm::vec3 cameraRight;

    // world vectors
    glm::vec3 worldUp;
    glm::vec3 worldForward;
    glm::vec3 worldRight;

    // orientation
    float yaw; // x-axis
    float pitch; // y-axis

    float fov;
    float zNear;
    float zFar;

    Camera(glm::vec3 position);

    void Rotate(double dx, double dy);
    void Move(CameraDirection direction, float amount);
    void Zoom(double dy);

    glm::mat4 GetViewMatrix();

private:
    void UpdateCameraVectors();
};