#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

glm::mat4 Camera::view() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::projection(float aspect) const {
    return glm::perspective(glm::radians(fovDegrees), aspect, 0.1f, 200.0f);
}

void Camera::orbit(float horizontal, float vertical) {
    glm::vec3 offset = position - target;
    float radius = glm::length(offset);
    if (radius < 0.001f) return;

    float theta = std::atan2(offset.z, offset.x);
    float phi = std::asin(std::clamp(offset.y / radius, -1.0f, 1.0f));

    theta += horizontal;
    phi += vertical;

    // Limita phi per evitare singolarità ai poli (gimbal lock)
    const float maxPhi = glm::radians(88.0f);
    phi = std::clamp(phi, -maxPhi, maxPhi);

    position = target + radius * glm::vec3(
        std::cos(phi) * std::cos(theta),
        std::sin(phi),
        std::cos(phi) * std::sin(theta)
    );
}

void Camera::zoom(float amount) {
    glm::vec3 offset = position - target;
    float radius = glm::length(offset);
    if (radius < 0.001f) return;

    float newRadius = std::clamp(radius - amount, 1.5f, 80.0f);
    position = target + (offset / radius) * newRadius;
}

void Camera::moveLocal(float forward, float right, float vertical) {
    glm::vec3 front = target - position;
    if (glm::length(front) < 0.0001f) return;
    front = glm::normalize(front);

    glm::vec3 side = glm::normalize(glm::cross(front, up));
    glm::vec3 actualUp = glm::normalize(glm::cross(side, front));

    glm::vec3 movement = forward * front + right * side + vertical * actualUp;
    position += movement;
    target += movement;
}
